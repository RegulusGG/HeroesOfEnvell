#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/Interfaces/InteractableInterface.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEROESOFENVELL_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

	UInteractionComponent();

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


//Мой код

public:

	void PrimaryInteract(); //Нажатие кнопки взаимодействия
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class USphereComponent* InteractionSphere_New;

protected:
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceDistance = 250.0f;

	UPROPERTY()
	AActor* LastBestTarget;

private:

	UPROPERTY()
	TArray<AActor*> OverlappingActors;
	
	IInteractable* BestTarget;

	FTimerHandle TimerHandle_Interaction;

	void UpdateBestTarget();
};