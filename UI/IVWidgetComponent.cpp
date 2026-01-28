// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IVWidgetComponent.h"
#include "IVHpBarWidget.h"

void UIVWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UIVHpBarWidget* IVHpBarWidget = Cast<UIVHpBarWidget>(GetWidget());
	if (IVHpBarWidget)
	{
		IVHpBarWidget->SetOwningActor(GetOwner());
	}
}
