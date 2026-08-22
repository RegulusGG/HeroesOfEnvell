// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "UStatBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API UUStatBarWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:

	UPROPERTY(meta = (BindWidget))
	class UImage* BarImage;
	
	UPROPERTY(EditAnywhere, Category = "Appearance")
	class UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	FLinearColor BarColor = FLinearColor::White;
 



	//Обновление информации в барах
	void UpdatePercent(float NewPercent);
	

protected:

	void SmoothUpdateGhost();

private:

	UPROPERTY(EditAnywhere)
	float CurrentGhostPercent;

	UPROPERTY(EditAnywhere)
	float TargetGhostPercent;

	FTimerHandle GhostUpdateTimerHandle;

};
