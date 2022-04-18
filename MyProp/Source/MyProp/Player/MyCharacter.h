// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyInfo.h" //캐릭터 정보 
#include <EngineMinimal.h>
#include "MyCharacterState.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UMyAnimInstance; //중복 헤더 포함 문제 해결
class AMyPlayerObjectPawn; //중복 헤더 포함 문제 해결

UCLASS()
class MYPROP_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/// <summary>
	/// 1. 캐릭터 매시 설정,
	/// </summary>
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input	
	/// <summary>
	/// 입력키를 함수와 연결하기
	/// </summary>
	/// <param name="PlayerInputComponent"></param>
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//매시 변수
	//인간 형태 변수

	//Moving
	void UpDown(float f);
	void LeftRight(float f);

	virtual void Dash();
	virtual void DashStop();

	//Machine Interaction
	void Interaction();

	//Jump
	void Jump();
	void JumpAction();
	bool isDashed;


	//ItemBtn
	void Item1();
	void Item2();

private:
	float jumpCntTime = 0.f; //점프 할 때마다 시간을 잰다 
	const float jumpEnableTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_Arm;

	float fLeftRight;
	float fUpdown;


protected:
	UMyAnimInstance* m_AnimInst; //클래스 객체 받아올 것


public:
	//캐릭터의 상태
	EPLAYER_STATE m_state;

public:
	//캐릭터 상태 전환
	virtual void ChangeState(EPLAYER_STATE newState);

public:
	EPLAYER_STATE GetState() { return m_state; }

protected:
	//데이터 테이블
	UDataTable* m_PlayerTable;

public:
	//캐릭터 정보 옮기기
	virtual void ShareInfoToObject(AMyPlayerObjectPawn* m_PlayerObjectPawn, FTableRowBase data) {}; //인간 -> 사물
	virtual void SetInfoToCharacter(AMyPlayerObjectPawn* m_PlayerObjectPawn,FTableRowBase data) {}; //사물 -> 인간
};
