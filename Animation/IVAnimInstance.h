// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IVAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INTERVERSE_API UIVAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UIVAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float FwdBwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float LeftRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float JumpingThreshold;
};
