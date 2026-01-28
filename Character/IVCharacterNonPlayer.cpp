// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IVCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/IVAIController.h"
#include "CharacterStat/IVCharacterStatComponent.h"
#include "Interface/IVGameInterface.h"
#include "GameFramework/GameModeBase.h"
#include "UI/IVWidgetComponent.h"
#include "UI/IVHUDWidget.h"

AIVCharacterNonPlayer::AIVCharacterNonPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_GDC.Kwang_GDC"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	GetMesh()->SetHiddenInGame(false);

	AIControllerClass = AIVAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AIVCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	 
	ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AIVCharacterNonPlayer::NPCMeshLoadCompleted));
}

void AIVCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	AIVAIController* IVAIController = Cast<AIVAIController>(GetController());
	if (IVAIController)
	{
		IVAIController->StopAI();

		IIVGameInterface* IVGameMode = Cast<IIVGameInterface>(GetWorld()->GetAuthGameMode());
		if (IVGameMode)
		{
			IVGameMode->OnPlayerScoreChanged(1);
		}
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]() {
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AIVCharacterNonPlayer::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NPCMeshHandle->ReleaseHandle();
}

float AIVCharacterNonPlayer::GetAIPatrolRadius()
{
	return 1000.0f;
}

float AIVCharacterNonPlayer::GetAIDetectRange()
{
	return 800.0f;
}

float AIVCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AIVCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AIVCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AIVCharacterNonPlayer::AttackByAI()
{
	ProcessComboCommand();
}

void AIVCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}

void AIVCharacterNonPlayer::ShowWidgetComp(bool _bIsVisible)
{
	FindComponentByClass<UIVWidgetComponent>()->SetVisibility(_bIsVisible);
}

void AIVCharacterNonPlayer::SetupAIHUDWidget(UIVHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());
		
		Stat->OnStatChanged.AddUObject(InHUDWidget, &UIVHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UIVHUDWidget::UpdateHpBar);
	}
}
