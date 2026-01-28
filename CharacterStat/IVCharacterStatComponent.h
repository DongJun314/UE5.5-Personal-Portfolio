// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/IVCharacterStat.h"
#include "IVCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnManaChangedDelegate, float /*CurrentMana*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FIVCharacterStat& /*BaseStat*/, const FIVCharacterStat& /*ModifierStat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERVERSE_API UIVCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIVCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnManaChangedDelegate OnManaChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void AddBaseStat(const FIVCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetBaseStat(const FIVCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetModifierStat(const FIVCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());}
	
	FORCEINLINE const FIVCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FIVCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FIVCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetCurrentMana() { return CurrentMana; }
	FORCEINLINE void HealHp(float InHealAmount) { CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	float ApplyDamage(float InDamage);
	bool SpendManaOnAttack(float fCost);
	void RecoverMana();
	void StartManaRegen();
	void StopManaRegen();
	void RestartManaRegenWithDelay();


protected:
	void SetHpBroadcast(float NewHp);
	void SetManaBroadcast(float NewMana);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float CurrentMana;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FIVCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FIVCharacterStat ModifierStat;

	FTimerHandle ManaRegenHandle;
	FTimerHandle ManaDelayHandle;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float ManaRegenInterval = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float ManaRegenDelay = 3.0f; // 마나를 소모한 후 회복 재개까지의 지연시간
};
