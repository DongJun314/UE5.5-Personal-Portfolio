// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IVManaOrbWidget.h"
#include "Components/ProgressBar.h"
#include "CharacterStat/IVCharacterStatComponent.h"

UIVManaOrbWidget::UIVManaOrbWidget(const FObjectInitializer& ObjectInitializer)
{
	m_fMaxMana = 100.0f;
}

void UIVManaOrbWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackManaBarOrb = Cast<UProgressBar>(GetWidgetFromName(TEXT("Back_ManaBarOrb")));
	ensure(BackManaBarOrb);

	FrontManaBarOrb = Cast<UProgressBar>(GetWidgetFromName(TEXT("Front_ManaBarOrb")));
	ensure(FrontManaBarOrb);
}

void UIVManaOrbWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BackManaBarOrb)
	{
		m_fBackManaRatio = FMath::FInterpTo(m_fBackManaRatio, m_fTargetManaRatio, InDeltaTime, 3.f);
		BackManaBarOrb->SetPercent(m_fBackManaRatio);

		// 목표값 근처에 오면 보간 종료
		if (FMath::IsNearlyEqual(m_fBackManaRatio, m_fTargetManaRatio, 0.001f))
		{
			m_fBackManaRatio = m_fTargetManaRatio;
			BackManaBarOrb->SetPercent(m_fBackManaRatio);
			//m_bInterpActive = false; // 루프 비활성화
			return;
		}
	}
}

void UIVManaOrbWidget::UpdateManaOrb(float NewCurMana)
{
	m_fCurrentMana = NewCurMana;
	ensure(m_fMaxMana > 0.0f);

	m_fTargetManaRatio = m_fCurrentMana / m_fMaxMana;

	if (FrontManaBarOrb)
	{
		FrontManaBarOrb->SetPercent(m_fTargetManaRatio);
	}
}



//void UIVManaOrbWidget::UpdateStat(const FIVCharacterStat& BaseStat, const FIVCharacterStat& ModifierStat)
//{
//	MaxHp = (BaseStat + ModifierStat).MaxHp;
//
//	if (HpBarOrb)
//	{
//		HpBarOrb->SetPercent(CurrentHp / MaxHp);
//	}
//
//}