// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\InterfaceVisual\HUD\MainHudWidget.h"

void UMainHudWidget::UpdateHealth(float Percent)
{
	if (HealthBar)
	{
		HealthBar->UpdatePercent(Percent);
	}
}

void UMainHudWidget::UpdateShield(float Percent)
{
	if (ShieldBar)
	{
		ShieldBar->UpdatePercent(Percent);
	}
}

void UMainHudWidget::UpdateMana(float Percent)
{
	if (ManaBar)
	{
		ManaBar->UpdatePercent(Percent);
	}
}
