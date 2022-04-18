// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../MyPropGameModeBase.h"

#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Survivor.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API ASurvivor : public AMyCharacter
{
	GENERATED_BODY()

public:
	ASurvivor();

private:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Interaction();

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

public:

	//캐릭터 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FSurvivorInfo m_Info;

	float m_PrevHP;
	float m_PrevSP;

	FSurvivorInfo* GetInfo() { return &m_Info; }
	void SetInfo(FSurvivorInfo new_Info) { m_Info = new_Info; }

//변신 기능
//캐릭터의 오브젝트 전환
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		AMyPlayerObjectPawn* m_PlayerObjectPawn; //사물형 폰

	//변신 초기화 각도, 초기화 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FVector FVChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FRotator FRChange;

	void ChangeToObject(UStaticMesh* mesh, FVector fscale);
};
