// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IVCharacterPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "IVCharacterControlData.h"
#include "UI/IVHUDWidget.h"
#include "CharacterStat/IVCharacterStatComponent.h"
#include "Interface/IVGameInterface.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/IVWidgetComponent.h"

AIVCharacterPlayer::AIVCharacterPlayer()
{
	// Camera
	{
		CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->TargetArmLength = 400.0f;
		CameraBoom->bUsePawnControlRotation = true;

		FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false;
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// Input Common
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump"));
		if (InputActionJumpRef.Object)
		{
			JumpAction = InputActionJumpRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl"));
		if (InputChangeActionControlRef.Object)
		{
			ChangeControlAction = InputChangeActionControlRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack"));
		if (InputActionAttackRef.Object)
		{
			AttackAction = InputActionAttackRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Run.IA_Run"));
		if (InputActionRunRef.Object)
		{
			RunAction = InputActionRunRef.Object;
		}
	}

	// Input Shoulder
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove"));
		if (InputActionShoulderMoveRef.Object)
		{
			ShoulderMoveAction = InputActionShoulderMoveRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook"));
		if (InputActionShoulderLookRef.Object)
		{
			ShoulderLookAction = InputActionShoulderLookRef.Object;
		}
	}

	// Input Quater
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove"));
		if (InputActionQuaterMoveRef.Object)
		{
			QuaterMoveAction = InputActionQuaterMoveRef.Object;
		}

	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;

	FindComponentByClass<UIVWidgetComponent>()->SetVisibility(false);
}

void AIVCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);
}

void AIVCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);

		IIVGameInterface* IVGameMode = Cast<IIVGameInterface>(GetWorld()->GetAuthGameMode());
		if (IVGameMode)
		{
			IVGameMode->OnPlayerDead();
		}
	}
}

void AIVCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AIVCharacterPlayer::Attack);
		EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AIVCharacterPlayer::ChangeCharacterControl);
		EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AIVCharacterPlayer::ShoulderMove);
		EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AIVCharacterPlayer::ShoulderLook);
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AIVCharacterPlayer::QuaterMove);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AIVCharacterPlayer::StartRun);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AIVCharacterPlayer::StopRun);
	}
}



void AIVCharacterPlayer::ChangeCharacterControl()
{
	if (ECharacterControlType::Quater == CurrentCharacterControlType)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (ECharacterControlType::Shoulder == CurrentCharacterControlType)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AIVCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UIVCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AIVCharacterPlayer::SetCharacterControlData(const UIVCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;


	// 락온 기능
	// {
	//	Pawn
	//	bUseControllerRotationYaw = true;

	//	CharacterMovement;
	//	GetCharacterMovement()->bOrientRotationToMovement = false;// 
	// }
	
}

void AIVCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AIVCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AIVCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AIVCharacterPlayer::Attack()
{
#define COSTMANA 20.0f

	if (Stat->SpendManaOnAttack(COSTMANA))
	{
		ProcessComboCommand();
	}
}

void AIVCharacterPlayer::StartRun()
{
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void AIVCharacterPlayer::StopRun()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void AIVCharacterPlayer::SetupHUDWidget(UIVHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpOrb(Stat->GetCurrentHp());
		InHUDWidget->UpdateManaOrb(Stat->GetCurrentMana());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &UIVHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UIVHUDWidget::UpdateHpOrb);
		Stat->OnManaChanged.AddUObject(InHUDWidget, &UIVHUDWidget::UpdateManaOrb);
	}
}
