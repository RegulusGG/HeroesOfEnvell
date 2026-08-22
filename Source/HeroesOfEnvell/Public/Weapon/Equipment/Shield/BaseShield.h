// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public\Weapon\BaseWeapon.h"
#include "BaseShield.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API ABaseShield : public ABaseWeapon
{
	GENERATED_BODY()
	
public:

	bool SetShieldVisible(bool bVisible); //Возвращает результат экипировки щита
	bool CanBlockAttack(const FVector& DamageOrigin);

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName ShieldSocket;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float CurrentStrength;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxStrength;

protected:
	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* SpawnEffect;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float BlockThreshold;
};
