// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\InterfaceVisual\Stances\RadialMenuWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Public\InterfaceVisual\Stances\UStanceSlotWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void URadialMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	int32 CurrentIndex = GetSelectedStanceIndex(GetMouseDirection(), CachedAngleStep);

	if (CurrentIndex != LastHoveredIndex)
	{
		if (DynamicBackgroundMaterial)
		{
			DynamicBackgroundMaterial->SetScalarParameterValue(TEXT("StancesCount"), (float)CachedStancesCount);
			DynamicBackgroundMaterial->SetScalarParameterValue(TEXT("SelectedIndex"), (float)CurrentIndex);
		}
		LastHoveredIndex = CurrentIndex;
	}
}
void URadialMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LastHoveredIndex = -1;

	if (BackgroundMaterialAsset && BackgroundImage)
	{
		DynamicBackgroundMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), BackgroundMaterialAsset);
		BackgroundImage->SetBrushFromMaterial(DynamicBackgroundMaterial);
	}
}

void URadialMenuWidget::SetupMenu(TArray<FStanceStruct> Stances)
{
	int StancesCount = Stances.Num();
	if (StancesCount <= 0 || !SlotWidgetClass) return;

	float AngleStep = 360.0f / StancesCount;
	float Radius = 200.0f;
	float StartAngle = -90.0f;
	
	CachedAngleStep = AngleStep;
	CachedStancesCount = StancesCount;
	 
	for (int i = 0; i < StancesCount; i++)
	{
		UUStanceSlotWidget* NewSlot = CreateWidget<UUStanceSlotWidget>(this, SlotWidgetClass);
		if (!NewSlot) { continue; }

		UPanelSlot* PanelSlot = SlotContainer->AddChild(NewSlot);

		float CurrentAngle = (i * AngleStep) + StartAngle;
		float Rad = FMath::DegreesToRadians(CurrentAngle);

		float X = FMath::Cos(Rad) * Radius;
		float Y = FMath::Sin(Rad) * Radius;

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot);
		if (CanvasSlot)
		{
			CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
			CanvasSlot->SetPosition(FVector2D(X, Y));
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		}

		NewSlot->K2_InitSlotData(Stances[i].Data);
	}
}

int32 URadialMenuWidget::GetSelectedStanceIndex(FVector2D MouseDirection, float AngleStep)
{
	if (MouseDirection.Size() < 20.0f) return -1;

	float AngleRadians = FMath::Atan2(MouseDirection.Y, MouseDirection.X);
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	
	if (AngleDegrees < 0) { AngleDegrees += 360; }

	float NormalizedAngle = AngleDegrees + 90.0f;
	NormalizedAngle += AngleStep * 0.5f;

	if (NormalizedAngle >= 360) NormalizedAngle -= 360.0f;
	if (NormalizedAngle < 0) NormalizedAngle += 360.0f;

	int32 Index = FMath::FloorToInt(NormalizedAngle / AngleStep);
	return Index % CachedStancesCount;
}

int32 URadialMenuWidget::GetIndex(FVector2D MouseDirection,float AngleStep)
{
	if (MouseDirection.Size() < 20.0f) return -1;

	float AngleRadians = FMath::Atan2(MouseDirection.X, -MouseDirection.Y);
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	if (AngleDegrees < 0) AngleDegrees += 360.0f;

	float HalfStep = AngleStep / 2.0f;
	float FinalAngle = AngleDegrees + HalfStep;

	if (FinalAngle >= 360.0f) FinalAngle -= 360.0f;

	int32 Index = FMath::FloorToInt(FinalAngle / AngleStep);
	return Index;
}

FVector2D URadialMenuWidget::GetMouseDirection()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC || !BackgroundImage) return FVector2D::ZeroVector;

	FVector2D MousePosition;
	if (PC->GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		FGeometry ImageGeom = BackgroundImage->GetCachedGeometry();

		FVector2D CenterOnScreen = ImageGeom.LocalToAbsolute(ImageGeom.GetLocalSize() * 0.5f);

		FVector2D Direction = MousePosition - CenterOnScreen;

		return Direction;
	}
	return FVector2D::ZeroVector;
}