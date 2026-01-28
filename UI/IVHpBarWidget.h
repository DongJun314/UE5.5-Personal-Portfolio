// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/IVUserWidget.h"
#include "GameData/IVCharacterStat.h"
#include "IVHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class INTERVERSE_API UIVHpBarWidget : public UIVUserWidget
{
	GENERATED_BODY()

public:
	UIVHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void UpdateStat(const FIVCharacterStat& BaseStat, const FIVCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> FrontHpProgressBar;

	UPROPERTY()
	TObjectPtr<class UProgressBar> BackHpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;
	
	UPROPERTY()
	float m_fCurrentHp;

	UPROPERTY()
	float m_fMaxHp;


	float m_fTargetHpRatio = 1.0f;
	float m_fBackHpRatio = 1.0f;
	bool m_bInterpActive = false;
};
