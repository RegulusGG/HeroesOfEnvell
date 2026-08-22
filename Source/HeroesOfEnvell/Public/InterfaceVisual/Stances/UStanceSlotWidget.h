// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UStanceSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API UUStanceSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget),BlueprintReadOnly )
	class UImage* StanceIcon;

	UPROPERTY(meta = (BindWidget),BlueprintReadOnly)
	class UTextBlock* StanceName;

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Stance")
	void K2_InitSlotData(FStanceData Data);

};
