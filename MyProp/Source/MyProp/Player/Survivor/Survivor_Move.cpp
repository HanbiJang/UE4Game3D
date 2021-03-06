// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor_Move.h"

void ASurvivor::UpDown(float f) {
	Super::UpDown(f);
	UpDown_Server(f);
}

void ASurvivor::LeftRight(float f) {
	Super::LeftRight(f);
	LeftRight_Server(f);
}

void ASurvivor::MyJump() {
	Super::MyJump();
	MyJump_Server();
}

//멀티캐스트
void ASurvivor::UpDown_Multicast_Implementation(float f) {
	if (m_state == EPLAYER_STATE::OBJECT) {
		if (f != 0 && m_PlayerObject->IsSimulatingPhysics()) {
			FRotator r = FRotator(0, GetControlRotation().Yaw, 0);
			//GetForwardVector: r만큼 월드 정방향 벡터를 회전
			m_PlayerObject->AddImpulse(UKismetMathLibrary::GetForwardVector(r) * f * fRunPower, NAME_None, true);
			//m_PlayerObject->AddAngularImpulseInDegrees(UKismetMathLibrary::GetForwardVector(r), NAME_None, true);
			//AddMovementInput(UKismetMathLibrary::GetForwardVector(r), f);

	/*		m_PlayerObject->AddImpulse(FVector(fRunPower * f, 0, 0), NAME_None, true);
			m_PlayerObject->AddAngularImpulseInDegrees(FVector(0, fRoPower * f, 0.f), NAME_None, true);*/
		}
	}
}

void ASurvivor::LeftRight_Multicast_Implementation(float f) {
	if (m_state == EPLAYER_STATE::OBJECT) {
		if (f != 0 && m_PlayerObject->IsSimulatingPhysics()) {
			FRotator r = FRotator(0, GetControlRotation().Yaw, 0);
			m_PlayerObject->AddImpulse(UKismetMathLibrary::GetRightVector(r)*f* fRunPower, NAME_None, true);
			//m_PlayerObject->AddAngularImpulseInDegrees(UKismetMathLibrary::GetForwardVector(r), NAME_None, true);
			//m_PlayerObject->AddImpulse(FVector(0, fRunPower * f, 0.f), NAME_None, true);
			//m_PlayerObject->AddAngularImpulseInDegrees(FVector(fRoPower * f, 0, 0.f), NAME_None, true);
		}
	}
}

void ASurvivor::MyJump_Multicast_Implementation() {
	if (m_state == EPLAYER_STATE::OBJECT) {
		if (JumpCnt < 2 && m_PlayerObject->IsSimulatingPhysics()) {
			JumpCnt++;
			isGround = false;
			m_PlayerObject->AddImpulse(FVector(0, 0, fJumpPower), NAME_None, true);
			UE_LOG(LogTemp, Log, TEXT("Jump!"));
		}
	}
}


void ASurvivor::UpDown_Server_Implementation(float f) {
	UpDown_Multicast(f);
}

void ASurvivor::LeftRight_Server_Implementation(float f) {
	LeftRight_Multicast(f);
}

void ASurvivor::MyJump_Server_Implementation() {
	MyJump_Multicast();
}

void ASurvivor::Dash()
{
	UE_LOG(LogTemp, Log, TEXT("Dasu!!!"));
	SetisDashPressed_Server(true);
	//isDashPressed = true;

	if (m_state != EPLAYER_STATE::OBJECT && GetInfo()->fCurSP > 10) {
		//isDashEnable = true;
		SetisDashEnable_Server(true);
		UE_LOG(LogTemp, Log, TEXT("DashEnable!!!"));
	}
}

void ASurvivor::DashStop()
{
	SetisDashPressed_Server(false);
	//isDashPressed = false;
}
