#include "Public\Character\Enemy\AI\EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Public\Character\Player\BasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	if (GetAIPerceptionComponent())
	{
		GetAIPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
	}
}

void AEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ABasePlayer* Player = Cast<ABasePlayer>(Actor);
	ABaseEnemy* MyPawn = Cast<ABaseEnemy>(GetPawn());

	if (Player && MyPawn)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			MyPawn->CurrentTargetPlayer = Player;
			Player->Execute_RegisterAggro(Player,GetPawn());
		}
		else
		{
			MyPawn->CurrentTargetPlayer = nullptr;
			Player->Execute_UnregisterAggro(Player,GetPawn());
			
		}
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}