// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/BaseAbility.h"
#include "DodgeAbility.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API UDodgeAbility : public UBaseAbility
{
	GENERATED_BODY()
	
public:

	virtual bool ActivateAbility() override;

protected:
	
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* BackwardMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* LeftMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* RightMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* ForwardMontage;
};
