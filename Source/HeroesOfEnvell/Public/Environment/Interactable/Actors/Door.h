// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/Environment/Interactable/BaseInteractable.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API ADoor : public ABaseInteractable
{
	GENERATED_BODY()
	
public:

	void OnInteract(AActor* Interactor) override;
};
