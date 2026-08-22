// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public\Ability\BaseAbility.h"
#include "RollAbility.generated.h"

class UAnimMontage;

UCLASS()
class HEROESOFENVELL_API URollAbility : public UBaseAbility
{
	GENERATED_BODY()
	
public:

	virtual bool ActivateAbility() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* RollMontage;
};
