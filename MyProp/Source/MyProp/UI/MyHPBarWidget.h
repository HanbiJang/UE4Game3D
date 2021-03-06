// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <components/progressbar.h>
#include <components/TextBlock.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API UMyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UProgressBar* m_HPBar;
	UTextBlock* m_PlayerName;
	UTextBlock* m_CurHP;
	UTextBlock* m_MaxHP;
	
public:
	void SetHP(float _CurHPRatio);
	void SetText_PlayerName(const FString& _Name);
	void SetText_CurHP(const FString& _Name);
	void SetText_MaxHP(const FString& _Name);


public:
	/*virtual */void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};
