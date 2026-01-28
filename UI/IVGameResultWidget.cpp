// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IVGameResultWidget.h"
#include "Components/TextBlock.h"

UIVGameResultWidget::UIVGameResultWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

void UIVGameResultWidget::GameResultVictory()
{
	UITextVictory->SetVisibility(ESlateVisibility::Visible);
	UITextDefeat->SetVisibility(ESlateVisibility::Collapsed);
}

void UIVGameResultWidget::GameResultDefeat()
{
	UITextVictory->SetVisibility(ESlateVisibility::Collapsed);
	UITextDefeat->SetVisibility(ESlateVisibility::Visible);
}

void UIVGameResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UITextVictory = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextVictory")));
	ensure(UITextVictory);

	UITextDefeat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextDefeat")));
	ensure(UITextDefeat);

	UITextVictory->SetVisibility(ESlateVisibility::Collapsed);
	UITextDefeat->SetVisibility(ESlateVisibility::Collapsed);
}