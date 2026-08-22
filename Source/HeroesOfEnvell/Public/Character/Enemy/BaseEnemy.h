#pragma once

#include "CoreMinimal.h"
#include "Public\Character\BaseCharacter.h"
#include "Public/Interfaces/SaveableInterface.h"
#include "BaseEnemy.generated.h"



UCLASS()
class HEROESOFENVELL_API ABaseEnemy : public ABaseCharacter, public ISaveableInterface
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	void OnDeath_Implementation() override;
	virtual void Destroyed() override;
	virtual void OnSaveActor_Implementation(USaveGame* SaveGameObject) override;
	virtual void OnLoadActor_Implementation(USaveGame* SaveGameObject) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (MakeEditWidget = true))
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 CurrentPatrolIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	class ABasePlayer* CurrentTargetPlayer;

};
