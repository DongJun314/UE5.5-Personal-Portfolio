// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/IVPlayerController.h"
#include "UI/IVHUDWidget.h"
#include "UI/IVGameResultWidget.h"

AIVPlayerController::AIVPlayerController()
{
	static ConstructorHelpers::FClassFinder<UIVHUDWidget> IVHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_IVHUD_Orb.WBP_IVHUD_Orb_C"));
	if (IVHUDWidgetRef.Class) 
	{
		IVHUDWidgetClass = IVHUDWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UIVGameResultWidget> IVGameResultWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_GameResult.WBP_GameResult_C"));
	if (IVGameResultWidgetRef.Class)
	{
		IVGameResultWidgetClass = IVGameResultWidgetRef.Class;
	}
}

void AIVPlayerController::GameScoreChanged(int32 NewScore)
{
	AddKilledScore(NewScore);
}

void AIVPlayerController::GameResult()
{
	IVHUDWidget->SetVisibility(ESlateVisibility::Collapsed);

	IVGameResultWidget = CreateWidget<UIVGameResultWidget>(this, IVGameResultWidgetClass);
	if (IVGameResultWidget)
	{
		IVGameResultWidget->AddToViewport(true);
		IVGameResultWidget->PlayAnimation(IVGameResultWidget->GetWidgetAnim());
	}
}

void AIVPlayerController::GameClear()
{
	//GameResult();
	//IVGameResultWidget->GameResultVictory();
}

void AIVPlayerController::GameOver()
{
	//GameResult();
	//IVGameResultWidget->GameResultDefeat();
}

void AIVPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//FInputModeGameOnly GameOnlyInputMode;
	//SetInputMode(GameOnlyInputMode);
	//
	//IVHUDWidget = CreateWidget<UIVHUDWidget>(this, IVHUDWidgetClass);
	//if (IVHUDWidget)
	//{
	//	IVHUDWidget->AddToViewport(true);
	//}
}
