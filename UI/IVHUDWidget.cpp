// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IVHUDWidget.h"
#include "UI/IVHpBarWidget.h"
#include "UI/IVHpOrbWidget.h"
#include "UI/IVManaOrbWidget.h"
#include "Interface/IVCharacterHUDInterface.h"
#include "Interface/IVCharacterAIInterface.h"

UIVHUDWidget::UIVHUDWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

void UIVHUDWidget::UpdateStat(const FIVCharacterStat& BaseStat, const FIVCharacterStat& ModifierStat)
{
	HpOrb->UpdateStat(BaseStat, ModifierStat);
	HpBar->UpdateStat(BaseStat, ModifierStat);
}

void UIVHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UIVHUDWidget::UpdateHpOrb(float NewCurrentHp)
{
	HpOrb->UpdateHpOrb(NewCurrentHp);
}

void UIVHUDWidget::UpdateManaOrb(float NewCurrentMana)
{
	ManaOrb->UpdateManaOrb(NewCurrentMana);
}

void UIVHUDWidget::ShowAIHpHUD(IIVCharacterAIInterface* _AIPawn)
{
	HpBar->SetVisibility(ESlateVisibility::Visible);

	if (_AIPawn)
	{
		_AIPawn->SetupAIHUDWidget(this);
	}
}

void UIVHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UIVHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	HpOrb = Cast<UIVHpOrbWidget>(GetWidgetFromName(TEXT("WidgetHpOrb")));
	ensure(HpOrb);

	ManaOrb = Cast<UIVManaOrbWidget>(GetWidgetFromName(TEXT("WidgetManaOrb")));
	ensure(ManaOrb);

	IIVCharacterHUDInterface* HUDPawn = Cast<IIVCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}

	HpBar->SetVisibility(ESlateVisibility::Collapsed);
}
