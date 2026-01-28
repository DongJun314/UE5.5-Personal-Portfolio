// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/IVAnimationAttackInterface.h"
#include "Interface/IVCharacterWidgetInterface.h"
#include "Interface/IVCharacterItemInterface.h"
#include "GameData/IVCharacterStat.h"
#include "IVCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogIVCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UIVItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class INTERVERSE_API AIVCharacterBase : public ACharacter, public IIVAnimationAttackInterface, public IIVCharacterWidgetInterface, public IIVCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIVCharacterBase(); 

	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class UIVCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = "CharacterControl", Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UIVCharacterControlData*> CharacterControlManager;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UIVComboActionData> ComboActionData;

// Combo
protected:
	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

////////////////////////////////////////////
// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

////////////////////////////////////////////
// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowAbstract = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 2.0f;

////////////////////////////////////////////
// Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);
	void ApplyStat(const FIVCharacterStat& BaseStat, const FIVCharacterStat& ModifierStat);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowAbstract = "true"))
	TObjectPtr<class UIVCharacterStatComponent> Stat;

////////////////////////////////////////////
// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowAbstract = "true"))
	TObjectPtr<class UIVWidgetComponent> HpBar; 

	virtual void SetupCharacterWidget(UIVUserWidget* InUserWidget) override;

////////////////////////////////////////////
// Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAcess = "true")) 
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UIVItemData* InItemData) override;
	virtual void DrinkPotion(class UIVItemData* InItemData);
	virtual void EquipWeapon(class UIVItemData* InItemData);
	virtual void ReadScroll(class UIVItemData* InItemData);
};
