// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IVGameResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class INTERVERSE_API UIVGameResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UIVGameResultWidget(const FObjectInitializer& ObjectInitializer);

public:
	void GameResultVictory();
	void GameResultDefeat();
	UWidgetAnimation* GetWidgetAnim() const { return AnimFadeIn; }

protected:
	virtual void NativeConstruct() override;

private:
	class UTextBlock* UITextVictory = nullptr;
	class UTextBlock* UITextDefeat = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* AnimFadeIn;
};
