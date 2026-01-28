// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IVHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/IVCharacterWidgetInterface.h"

UIVHpBarWidget::UIVHpBarWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	m_fMaxHp = -1.0f;
}

void UIVHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FrontHpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Front_PbHpBar")));
	ensure(FrontHpProgressBar);

	BackHpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Back_PbHpBar")));
	ensure(BackHpProgressBar);

	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	IIVCharacterWidgetInterface* CharacterWidget = Cast<IIVCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UIVHpBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BackHpProgressBar && m_bInterpActive)
	{
		m_fBackHpRatio = FMath::FInterpTo(m_fBackHpRatio, m_fTargetHpRatio, InDeltaTime, 3.f);
		BackHpProgressBar->SetPercent(m_fBackHpRatio);

		// 목표값 근처에 오면 보간 종료
		if (FMath::IsNearlyEqual(m_fBackHpRatio, m_fTargetHpRatio, 0.001f))
		{
			m_fBackHpRatio = m_fTargetHpRatio;
			BackHpProgressBar->SetPercent(m_fBackHpRatio);
			m_bInterpActive = false; // 루프 비활성화
		}
	}
}

void UIVHpBarWidget::UpdateStat(const FIVCharacterStat& BaseStat, const FIVCharacterStat& ModifierStat)
{
	m_fMaxHp = (BaseStat + ModifierStat).MaxHp;
	
	m_fTargetHpRatio = m_fCurrentHp / m_fMaxHp;

	if (FrontHpProgressBar)
	{
		FrontHpProgressBar->SetPercent(m_fTargetHpRatio);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void UIVHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	m_fCurrentHp = NewCurrentHp;
	ensure(m_fMaxHp > 0.0f);

	m_fTargetHpRatio = m_fCurrentHp / m_fMaxHp;
	m_bInterpActive = true;

	if (FrontHpProgressBar)
	{
		FrontHpProgressBar->SetPercent(m_fTargetHpRatio);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UIVHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), m_fCurrentHp, m_fMaxHp);
}
