// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/IVCharacterStat.h"
#include "IVHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class INTERVERSE_API UIVHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UIVHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FIVCharacterStat& BaseStat, const FIVCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	void UpdateHpOrb(float NewCurrentHp);
	void UpdateManaOrb(float NewCurrentMana);

	TObjectPtr<class UIVManaOrbWidget> GetManaOrbWidget() const { return ManaOrb; }

public:
	void ShowAIHpHUD(class IIVCharacterAIInterface* _AIPawn);

protected:
	UPROPERTY()
	TObjectPtr<class UIVHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UIVHpOrbWidget> HpOrb;

	UPROPERTY()
	TObjectPtr<class UIVManaOrbWidget> ManaOrb;
};
