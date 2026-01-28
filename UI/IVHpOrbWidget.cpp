// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IVHpOrbWidget.h"
#include "Components/ProgressBar.h"

UIVHpOrbWidget::UIVHpOrbWidget(const FObjectInitializer& ObjectInitializer)
{
	m_fMaxHp = -1.0f;
}

void UIVHpOrbWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FrontHpBarOrb = Cast<UProgressBar>(GetWidgetFromName(TEXT("Front_HpBarOrb")));
	ensure(FrontHpBarOrb);

	BackHpBarOrb = Cast<UProgressBar>(GetWidgetFromName(TEXT("Back_HpBarOrb")));
	ensure(BackHpBarOrb);
}

void UIVHpOrbWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BackHpBarOrb && m_bInterpActive)
	{
		m_fBackHpRatio = FMath::FInterpTo(m_fBackHpRatio, m_fTargetHpRatio, InDeltaTime, 3.f);
		BackHpBarOrb->SetPercent(m_fBackHpRatio);

		// 목표값 근처에 오면 보간 종료
		if (FMath::IsNearlyEqual(m_fBackHpRatio, m_fTargetHpRatio, 0.001f))
		{
			m_fBackHpRatio = m_fTargetHpRatio;
			BackHpBarOrb->SetPercent(m_fBackHpRatio);
			m_bInterpActive = false; // 루프 비활성화
		}
	}
}

void UIVHpOrbWidget::UpdateHpOrb(float NewCurrentHp)
{
	m_fCurrentHp = NewCurrentHp;
	ensure(m_fMaxHp > 0.0f);

	m_fTargetHpRatio = m_fCurrentHp / m_fMaxHp;

	if (FrontHpBarOrb)
	{
		FrontHpBarOrb->SetPercent(m_fTargetHpRatio);
	}

	m_bInterpActive = true;
}

void UIVHpOrbWidget::UpdateStat(const FIVCharacterStat& BaseStat, const FIVCharacterStat& ModifierStat)
{
	m_fMaxHp = (BaseStat + ModifierStat).MaxHp;

	m_fTargetHpRatio = m_fCurrentHp / m_fMaxHp;

	if (FrontHpBarOrb)
	{
		FrontHpBarOrb->SetPercent(m_fTargetHpRatio);
	}
}