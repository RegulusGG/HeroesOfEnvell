// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UStatBarWidget.h"
#include "MainHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UUStatBarWidget* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UUStatBarWidget* ManaBar;

	UPROPERTY(meta = (BindWidget))
	class UUStatBarWidget* ShieldBar;

	void UpdateHealth(float Percent);
	void UpdateShield(float Percent);
	void UpdateMana(float Percent);
};
