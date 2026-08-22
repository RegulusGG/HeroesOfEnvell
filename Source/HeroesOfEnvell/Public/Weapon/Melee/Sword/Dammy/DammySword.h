// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Melee/Sword/BaseSword.h"
#include "DammySword.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API ADammySword : public ASword
{
	GENERATED_BODY()
	

public:

	UAnimMontage* GetAttackMontage() override { return AttackMontage; };
};
