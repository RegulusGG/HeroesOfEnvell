// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseAbility.generated.h"

class ABaseCharacter;

UCLASS(Abstract, Blueprintable)
class HEROESOFENVELL_API UBaseAbility : public UObject
{
	GENERATED_BODY()
	

public:

	void Init(ABaseCharacter* Character);

	UFUNCTION()
	virtual bool ActivateAbility();

	UFUNCTION()
	virtual void DeactivateAbility();

	UPROPERTY(EditAnywhere)
	float ManaCost;

protected:

	UPROPERTY(BlueprintReadOnly)
	ABaseCharacter* CharacterOwner;
	
};
