// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//UI
#include <MyProp/UI/MyMainHUD.h>
#include <MyProp/UI/MyHPBarWidget.h>
#include <MyProp/UI/MySPWidget.h>

#include <MyProp/UI/MyStartGameWidget.h>
#include <MyProp/UI/Killer/MyKillerMainHUD.h>

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPropGameModeBase.generated.h"

class AKiller;
class ASurvivor;
class AMyPlayerController;

/**
 * 
 */
UCLASS()
class MYPROP_API AMyPropGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	//MyCharacter에게 폰 빙의 시키기
	AMyPropGameModeBase();

	void BeginPlay();
	void Tick(float DeltaTime) override;
	
	//서버장이 킬러가 되고 나머지 클라는 생존자가 됨
	TArray<AMyPlayerController*> PCSurvivorArr;
	AMyPlayerController* PCKiller;

	//AKiller* pKillerPlayer;
	//ASurvivor* pSurvivorPlayer1;
	//AMyPlayerController* Survivor1Controller;
	//AMyPlayerController* Killer1Controller;

	//플레이어 스폰 시작 위치 (y값에 값 서로 간의 거리 100씩 더해서 스폰될 것)
	FVector vKillerSpawnLocation = FVector(-6270, -450, 0);
	FVector vSurvivor1SpawnLocation = FVector(2000, 1750, 200);

private:
	/*virtual */void PostLogin(APlayerController* NewPlayer) override; //로그인이 성공한뒤에 호출

	//타이머=============================================================
private:
	float GameLeftTimeSec; //게임 남은 시간을 초로 환산한 시간 : 7분
	UMyGameInstance* GI; //게임 인스턴스
public:
	float GetGameLeftTimeSec() { return GameLeftTimeSec; }
	void SetGameLeftTimeSec(float value) { GameLeftTimeSec = value; }
	bool IsGameStartEnable; //게임 스타트가 가능한지 변수

	UFUNCTION(Reliable, Server)
		void UpdateTimerUI(float DeltaTime);

	//발전기 수==========================================================
private:
	int DoneMachineNum; //돌린 발전기 수
public:
	int GetDoneMachineNum() { return DoneMachineNum; }
	void SetDoneMachineNum(int value) { DoneMachineNum = value; }

	//플레이어 이름======================================================
private:
	TArray<FString> PlayerNames;
public:
	TArray<FString> GetPlayerNames() { return PlayerNames; }
	void SetPlayerNames(TArray<FString> value) { PlayerNames = value; }
	
};
