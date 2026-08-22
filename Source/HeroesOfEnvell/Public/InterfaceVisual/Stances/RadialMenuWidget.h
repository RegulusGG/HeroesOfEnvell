// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public\Service\ProjectTypes.h"
#include "Public\InterfaceVisual\Stances\UStanceSlotWidget.h"
#include "RadialMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API URadialMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUStanceSlotWidget> SlotWidgetClass;

	void SetupMenu(TArray<FStanceStruct> Stances);

	int32 GetSelectedStanceIndex(FVector2D MouseDirection, float AngleStep);
	FVector2D GetMouseDirection();
	float CachedAngleStep = 0.0f;
	int32 CachedStancesCount = 0;

	UPROPERTY(EditAnywhere)
	float Offset =0.0f;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* SlotContainer;
		
	UPROPERTY(EditAnywhere, Category = "UI")
	UMaterialInterface* BackgroundMaterialAsset;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicBackgroundMaterial;

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage;

	int32 LastHoveredIndex;
	int32 IndexForBacklight;

	int32 GetCurrentIndex() const { return LastHoveredIndex; }
	int32 GetIndex(FVector2D MouseDirection, float AngleStep);
};
