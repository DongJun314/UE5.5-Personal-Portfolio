// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/IVCharacterStat.h"
#include "IVCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class INTERVERSE_API UIVCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdateStat(const FIVCharacterStat& BaseStat, const FIVCharacterStat& ModifierStat);

	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;

};
