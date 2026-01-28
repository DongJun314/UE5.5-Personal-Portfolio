// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/IVCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/IVCharacterAIInterface.h"
#include "IVCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(Config = Interverse)
class INTERVERSE_API AIVCharacterNonPlayer : public AIVCharacterBase, public IIVCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AIVCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;
	void NPCMeshLoadCompleted();

	UPROPERTY(Config)
	TArray<FSoftObjectPath> NPCMeshes;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;

// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
	virtual void ShowWidgetComp(bool _bIsVisible) override;
	virtual void SetupAIHUDWidget(class UIVHUDWidget* InHUDWidget) override;
};
