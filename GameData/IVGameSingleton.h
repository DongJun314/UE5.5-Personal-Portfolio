// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IVCharacterStat.h"
#include "IVGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogIVGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class INTERVERSE_API UIVGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UIVGameSingleton();
	static UIVGameSingleton& Get();

// Character Stat Data Section
public:
	FORCEINLINE FIVCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FIVCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel; 

private:
	TArray<FIVCharacterStat> CharacterStatTable ;
};
