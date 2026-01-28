// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/IVUserWidget.h"
#include "IVManaOrbWidget.generated.h"

/**
 * 
 */
UCLASS()
class INTERVERSE_API UIVManaOrbWidget : public UIVUserWidget
{
	GENERATED_BODY()

public:
	UIVManaOrbWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void UpdateManaOrb(float NewCurrentMana);
	//void UpdateStat(const FIVCharacterStat& BaseStat, const FIVCharacterStat& ModifierStat);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> BackManaBarOrb;

	UPROPERTY()
	TObjectPtr<class UProgressBar> FrontManaBarOrb;

	UPROPERTY()
	float m_fCurrentMana;

	UPROPERTY()
	float m_fMaxMana;

	float m_fTargetManaRatio = 1.0f;
	float m_fBackManaRatio = 1.0f;
	bool m_bInterpActive = false;
};
