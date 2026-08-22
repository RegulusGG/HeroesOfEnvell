// Fill out your copyright notice in the Description page of Project Settings.

#include "Public\InterfaceVisual\HUD\UStatBarWidget.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Image.h"

void UUStatBarWidget::NativeConstruct()
{

	Super::NativeConstruct();
	
	if (BarImage)
	{
		DynamicMaterial = BarImage->GetDynamicMaterial();
		
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue(FName("BarColor"), BarColor);
		}
	}
}

void UUStatBarWidget::UpdatePercent(float NewPercent)
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("Percent"), NewPercent);
		TargetGhostPercent = NewPercent;

		if (!GetWorld()->GetTimerManager().IsTimerActive(GhostUpdateTimerHandle))
		{
			DynamicMaterial->GetScalarParameterValue(FName("GhostPercent"), CurrentGhostPercent);

			GetWorld()->GetTimerManager().SetTimer(GhostUpdateTimerHandle, this, &UUStatBarWidget::SmoothUpdateGhost, 0.01f, true);
		}
	}
}

void UUStatBarWidget::SmoothUpdateGhost()
{
	if (DynamicMaterial)
	{

		CurrentGhostPercent = FMath::FInterpTo(CurrentGhostPercent, TargetGhostPercent, 0.01f, 10.0f);

		DynamicMaterial->SetScalarParameterValue(FName("GhostPercent"), CurrentGhostPercent);

		if (FMath::IsNearlyEqual(CurrentGhostPercent, TargetGhostPercent, 0.001f))
		{
			DynamicMaterial->SetScalarParameterValue(FName("GhostPercent"), TargetGhostPercent);
			GetWorld()->GetTimerManager().ClearTimer(GhostUpdateTimerHandle);
		}

	}
}