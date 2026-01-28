// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IVPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INTERVERSE_API AIVPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AIVPlayerController();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game", meta = (DisplayName = "OnScoreChangedCpp"))
	void K2_OnScoreChanged(int32 NewScore);
	UFUNCTION(BlueprintImplementableEvent, Category = "Game", meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();
	UFUNCTION(BlueprintImplementableEvent, Category = "Game", meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	void GameScoreChanged(int32 NewScore);

	void GameResult();
	void GameClear();
	void GameOver();
	
	TObjectPtr<class UIVHUDWidget> GetHUDWidget() const { return IVHUDWidget; }

protected:
	virtual void BeginPlay() override;

private:
	void AddKilledScore(uint32 _iNewKilledScore) { m_iCurKilledScore += _iNewKilledScore; }

public:
	int32 GetCurKilledScore() const { return m_iCurKilledScore; }

// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UIVHUDWidget> IVHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<class UIVHUDWidget> IVHUDWidget;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameResult")
	TSubclassOf<class UIVGameResultWidget> IVGameResultWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameResult")
	TObjectPtr<class UIVGameResultWidget> IVGameResultWidget;

private:
	int32 m_iCurKilledScore = 0;
};
