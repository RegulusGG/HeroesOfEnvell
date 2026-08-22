// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHudWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> MainHUDWidgetClass;
	
	UPROPERTY( BlueprintReadOnly, Category = "HUD")
	UMainHudWidget* MainHUDWidget;
	
	UFUNCTION()
	void HandleHealthChanged(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void HandleShieldStrengthChanged(float CurrentStrength, float MaxStrength);

	UFUNCTION()
	void HandleManaChanged(float CurrentMana, float MaxMana);

public:

};
