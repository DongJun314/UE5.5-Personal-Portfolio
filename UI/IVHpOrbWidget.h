// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/IVUserWidget.h"
#include "GameData/IVCharacterStat.h"
#include "IVHpOrbWidget.generated.h"

/**
 * 
 */
UCLASS()
class INTERVERSE_API UIVHpOrbWidget : public UIVUserWidget
{
	GENERATED_BODY()
	
public:
	UIVHpOrbWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void UpdateHpOrb(float NewCurrentHp);
	void UpdateStat(const FIVCharacterStat& BaseStat, const FIVCharacterStat& ModifierStat);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> FrontHpBarOrb;

	UPROPERTY()
	TObjectPtr<class UProgressBar> BackHpBarOrb;

	UPROPERTY()
	float m_fCurrentHp;

	UPROPERTY()
	float m_fMaxHp;


	float m_fTargetHpRatio = 1.0f;
	float m_fBackHpRatio = 1.0f;
	bool m_bInterpActive = false;
};
