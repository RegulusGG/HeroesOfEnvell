// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public\Ability\BaseAbility.h"
#include "Public\Weapon\Equipment\Shield\BaseShield.h"
#include "Public\Service\ProjectTypes.h"
#include "ShieldAbility.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API UShieldAbility : public UBaseAbility
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, Category = "Regeneration")
	float ShieldRegenManaCost = 30;
	virtual bool ActivateAbility() override;
	virtual void DeactivateAbility() override;
	void RegenShield();

	EWeaponState LastState;
};
