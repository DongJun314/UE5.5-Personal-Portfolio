// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/IVGameSingleton.h"

DEFINE_LOG_CATEGORY(LogIVGameSingleton);

UIVGameSingleton::UIVGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(
		TEXT("/Game/ArenaBattle/GameData/IVCharacterStatTable.IVCharacterStatTable"));
	if (DataTableRef.Object) 
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value) {
				return *reinterpret_cast<FIVCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UIVGameSingleton& UIVGameSingleton::Get()
{
	UIVGameSingleton* Singleton = CastChecked<UIVGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogIVGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UIVGameSingleton>();
}

