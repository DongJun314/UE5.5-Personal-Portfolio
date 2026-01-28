// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/IVAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IIVAnimationAttackInterface* AttackPawn = Cast<IIVAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
