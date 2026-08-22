// Fill out your copyright notice in the Description page of Project Settings.

#include "Public\InterfaceVisual\HUD\MainHUD.h"
#include "Blueprint/UserWidget.h"
#include "Public\Character\Player\BasePlayer.h"
#include "Public\InterfaceVisual\HUD\MainHudWidget.h"
#include "GameFramework/PlayerController.h"

void AMainHUD::BeginPlay()
{

	Super::BeginPlay();

	if (MainHUDWidgetClass)
	{
		MainHUDWidget = CreateWidget<UMainHudWidget>(GetWorld(), MainHUDWidgetClass);
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport();
		} 
	}

	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		ABasePlayer* Player = Cast<ABasePlayer>(PC->GetPawn());
		if (Player)
		{
			Player->OnHealthChanged.AddDynamic(this, &AMainHUD::HandleHealthChanged);
			Player->OnShieldStrengthChanged.AddDynamic(this, &AMainHUD::HandleShieldStrengthChanged);
			Player->OnManaChanged.AddDynamic(this, &AMainHUD::HandleManaChanged);
		}
	}
}


void AMainHUD::HandleHealthChanged(float CurrentHealth, float MaxHealth)
{
	if (MainHUDWidget)
	{
		float Percent = CurrentHealth / MaxHealth;
		MainHUDWidget->UpdateHealth(Percent);
	}
}

void AMainHUD::HandleShieldStrengthChanged(float CurrentStrenght, float MaxStrenght)
{
	if (MainHUDWidget)
	{
		float Percent = CurrentStrenght / MaxStrenght;
		MainHUDWidget->UpdateShield(Percent);
	}
}

void AMainHUD::HandleManaChanged(float CurrentMana, float MaxMana)
{
	if (MainHUDWidget)
	{
		float Percent = CurrentMana / MaxMana;
		MainHUDWidget->UpdateMana(Percent);
	}
}