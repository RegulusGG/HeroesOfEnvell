// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Public\Interfaces\InteractableInterface.h"
#include "Components/BoxComponent.h"
#include "Public/Character/Player/InteractionComponent.h"
#include "BaseInteractable.generated.h"

class USceneComponent;
class UArrowComponent;
class UStaticMeshComponent;

UCLASS()
class HEROESOFENVELL_API ABaseInteractable : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ABaseInteractable();

	virtual void OnInteract(AActor* Interactor) override;

	UFUNCTION()
	void ShowTooltip(bool bShow);
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* InteractableCollision;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	UWidgetComponent* InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	FText InteractionActionName;

};
