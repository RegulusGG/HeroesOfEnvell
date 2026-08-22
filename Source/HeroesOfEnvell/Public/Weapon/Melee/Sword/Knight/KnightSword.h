// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Melee/Sword/BaseSword.h"
#include "KnightSword.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API AKnightSword : public ASword
{
	GENERATED_BODY()

	AKnightSword();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	TArray<class UAnimMontage*> StrengthMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	TArray<class UAnimMontage*> FastMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	TArray<class UAnimMontage*> MassMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	TArray<class UAnimMontage*> ExplorationMontages;

	UFUNCTION()
	UAnimMontage* GetAttackMontage() override;

	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	int32 MontageIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	bool bVisible = false;
};
