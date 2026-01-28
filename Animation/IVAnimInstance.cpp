// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/IVAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UIVAnimInstance::UIVAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void UIVAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UIVAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Movement || !Owner)
		return;

	Velocity = Movement->Velocity;
	GroundSpeed = Velocity.Size2D();
	bIsIdle = GroundSpeed < MovingThreshold;
	bIsFalling = Movement->IsFalling();
	bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);

	if (Owner->GetController())
	{
		const FRotator YawRotation(0, Owner->GetController()->GetControlRotation().Yaw, 0);
		FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		FVector NormalizedVelocity = Velocity.GetSafeNormal();

		FwdBwd = FVector::DotProduct(NormalizedVelocity, Forward) * 500.0f;
		LeftRight = FVector::DotProduct(NormalizedVelocity, Right) * 500.0f;

		// Direction °è»ê
		Direction = CalculateDirection(Velocity, Owner->GetActorRotation());
	}
}
