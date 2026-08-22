#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatTargetInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCombatTargetInterface : public UInterface
{
    GENERATED_BODY()
};

class HEROESOFENVELL_API ICombatTargetInterface
{
    GENERATED_BODY()
 
public:
    UFUNCTION(BlueprintNativeEvent, Category = "CombatState")
    void RegisterAggro(AActor* Enemy);

    UFUNCTION(BlueprintNativeEvent,Category = "CombatState")
    void UnregisterAggro(AActor* Enemy);
};