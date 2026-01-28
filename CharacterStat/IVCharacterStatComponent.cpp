// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/IVCharacterStatComponent.h"
#include "GameData/IVGameSingleton.h"
#include "Player/IVPlayerController.h"
#include "UI/IVManaOrbWidget.h"
#include "UI/IVHUDWidget.h"

// Sets default values for this component's properties
UIVCharacterStatComponent::UIVCharacterStatComponent()
{  
	CurrentLevel = 1;
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true;
}

void UIVCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	//GetWorld()->GetTimerManager().SetTimer(ManaRegenHandle, this, &UIVCharacterStatComponent::RecoverMana, 0.2f, true);

	SetLevelStat(CurrentLevel);
	SetHpBroadcast(BaseStat.MaxHp);
	SetManaBroadcast(100.0f);
}

void UIVCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UIVGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(UIVGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
}

float UIVCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PreHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHpBroadcast(PreHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

bool UIVCharacterStatComponent::SpendManaOnAttack(float fCost)
{
	const float PreMan = CurrentMana;
	const float ActualCostMana = FMath::Clamp<float>(fCost, 0, fCost);

	if (PreMan - ActualCostMana >= 0.0f)
	{
		SetManaBroadcast(PreMan - ActualCostMana);
		RestartManaRegenWithDelay();
		return true;
	}
	return false;
}

void UIVCharacterStatComponent::RecoverMana()
{
	if (CurrentMana < 100.0f)
	{
		CurrentMana += 1.0f;
		SetManaBroadcast(CurrentMana);
	}
}

void UIVCharacterStatComponent::StartManaRegen()
{	
	// 주기적으로 회복 시작
	GetWorld()->GetTimerManager().SetTimer(
		ManaRegenHandle,
		this,
		&UIVCharacterStatComponent::RecoverMana,
		ManaRegenInterval,
		true
	);
}

void UIVCharacterStatComponent::StopManaRegen()
{
	// 즉시 회복 중단
	GetWorld()->GetTimerManager().ClearTimer(ManaRegenHandle);
}

void UIVCharacterStatComponent::RestartManaRegenWithDelay()
{
	// 기존 회복 중단
	StopManaRegen();

	// 기존 딜레이 타이머 제거
	GetWorld()->GetTimerManager().ClearTimer(ManaDelayHandle);

	// 지정된 시간 후에 회복 재개
	GetWorld()->GetTimerManager().SetTimer(
		ManaDelayHandle,
		this,
		&UIVCharacterStatComponent::StartManaRegen,
		ManaRegenDelay,
		false
	);
}

void UIVCharacterStatComponent::SetHpBroadcast(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}

void UIVCharacterStatComponent::SetManaBroadcast(float NewMana)
{
	CurrentMana = FMath::Clamp<float>(NewMana, 0.0f, 100.0f);
	OnManaChanged.Broadcast(CurrentMana);
}