// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */

class UAIPerceptionComponent;
struct FAIStimulus;

UCLASS()
class HEROESOFENVELL_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

protected:
	virtual void OnPossess(APawn* Pawn) override;
	
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimuls);
	
	void BeginPlay() override;
};
