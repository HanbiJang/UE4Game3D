// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor.h"
#include "../MyPlayerObjectPawn.h"
#include "../Killer/Killer.h"

ASurvivor::ASurvivor() :
	FVChange(0, 0, 0),
	FRChange(0, 0, 0)
{
}

void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &ASurvivor::Interaction);
}

void ASurvivor::Interaction()
{
	//발전기, 문 등과 상호작용
}

void ASurvivor::BeginPlay() {
	Super::BeginPlay();

	//======블레이어 변신용 사물 오브젝트 블루프린트======
	//가져와서 스폰시키기
	UBlueprintGeneratedClass* LoadBP = LoadObject<UBlueprintGeneratedClass>(GetWorld(),
		TEXT("Blueprint'/Game/Blueprints/Survivor/BP_MyPlayerObjectPawn.BP_MyPlayerObjectPawn_C'"));

	if (LoadBP)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_PlayerObjectPawn = nullptr;

		m_PlayerObjectPawn = GetWorld()->SpawnActor<AMyPlayerObjectPawn>(LoadBP,
			FVector(0, 0, 0),//물체의 대략적 크기 구하기
			FRotator(0, 0, 0),
			SpawnInfo);

		m_PlayerObjectPawn->SetPCharacter(this); //인간폼 정보등록

	}
}

void ASurvivor::Dash()
{
	UE_LOG(LogTemp, Log, TEXT("Dasu!!!"));
	isDashPressed = true;

	if (GetInfo()->fCurSP > 10) {
		isDashEnable = true;
	}
}

void ASurvivor::DashStop()
{
	isDashPressed = false;
}

void ASurvivor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//체력, 스테미나 실시간 업데이트
	if (m_PrevHP != m_Info.fCurHP || m_PrevSP != m_Info.fCurSP)
	{
		// 이전 체력과 현제 체력이 다르다.
		// HUD 갱신
		AMyPropGameModeBase* GM = Cast<AMyPropGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		float HPRatio = m_Info.fCurHP / m_Info.fMaxHP;
		float MPRatio = m_Info.fCurSP / m_Info.fMaxSP;
		GM->UpdatePlayHUD(HPRatio, MPRatio);
	}

	// 현재 체력, 스태미너를 이전프레임 체력으로 갱신
	m_PrevHP = m_Info.fCurHP;
	m_PrevSP = m_Info.fCurSP;

	//대시 가능 여부)
	if (GetInfo()->fCurSP < 0.1) {
		isDashEnable = false;
	}

	//대시 중)
	//[1] 스태미너 감소
	if (isDashEnable && isDashPressed && isMoving) {
		GetCharacterMovement()->MaxWalkSpeed = 1200.f; //속도 증가 (대시)	
		ChangeState(EPLAYER_STATE::DASH); //상태 전환
		isDashed = true;

		GetInfo()->fCurSP -= 0.4f;
		if (GetInfo()->fCurSP < 0) GetInfo()->fCurSP = 0;
	}
	//[2] 대시 아닌 상태) 스태미너 회복
	else{ 
		GetCharacterMovement()->MaxWalkSpeed = 600.f; //속도 원상복귀
		isDashed = false;

		GetInfo()->fCurSP += 0.035f;
		if (GetInfo()->fCurSP > GetInfo()->fMaxSP) GetInfo()->fCurSP = GetInfo()->fMaxSP;
	}

	//======================================================================================

	//살인마와 거리 차이에 따라서 비네팅 효과 & 화면 그레인 (지터) 증가 감소
	//m_Cam->PostProcessSettings.VignetteIntensity 
	//m_Cam->PostProcessSettings.GrainIntensity
	//m_Cam->PostProcessSettings.GrainJitter

	//월드 상의 특정 클래스 Actor을 가져오기
	TArray<AActor*> arrActor;
	TArray<AKiller*> arrKiller;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKiller::StaticClass(), arrActor);
	for (int i = 0; i < arrActor.Num(); i++) {
		arrKiller.Add(Cast<AKiller>(arrActor[i])); //형변환
	}
	//if(arrKiller.Num() != 0)
	//UE_LOG(LogTemp, Log, TEXT("Killer name : %s"), TCHAR_TO_ANSI(*arrKiller[0]->GetName()));

}

void ASurvivor::ChangeToObject(UStaticMesh* mesh, FVector fscale)
{
	//플레이어 상태 변경
	ChangeState(EPLAYER_STATE::OBJECT);

	FVector originalPos = GetActorLocation(); //변신 당시 원래 위치 저장

	//원래 [인간형] 오브젝트 다른 곳으로 치우기 && 각도 시작과 같이 설정하기
	SetActorLocation(FVChange);
	SetActorRotation(FRChange);

	//물리 끄기
	m_PlayerObjectPawn->m_ObjectMesh->SetSimulatePhysics(false);

	//물체 위치를 인간폼이 있던 곳으로 설정
	m_PlayerObjectPawn->m_ObjectMesh->SetAllPhysicsPosition(originalPos + FVector(0, 0, mesh->GetBoundingBox().GetSize().Z * fscale.Z));

	//클릭한 오브젝트 메시의 크기와 같게 설정
	m_PlayerObjectPawn->m_ObjectMesh->SetRelativeScale3D(fscale);
	//매시 설정
	m_PlayerObjectPawn->m_ObjectMesh->SetStaticMesh(mesh);

	//시점 옮기기...
	GetWorld()->GetFirstPlayerController()->Possess(m_PlayerObjectPawn);

	//0.1초 뒤에 오브젝트 물리 켜기
	GetWorld()->GetTimerManager().SetTimer(m_PlayerObjectPawn->FPhysicsTimer, m_PlayerObjectPawn, &AMyPlayerObjectPawn::SetSimulatePhysicsTrue, 0.1f, false);

	//변신 가능 상태 조절
	//1.5초 뒤에 변신 가능해지기
	GetWorld()->GetTimerManager().SetTimer(m_PlayerObjectPawn->FChangeEnableTimer, m_PlayerObjectPawn, &AMyPlayerObjectPawn::SetbChangeEnableTrue, 1.5f, false);

	//캐릭터 정보 옮기기
	m_PlayerObjectPawn->SetInfo(m_Info);

}


