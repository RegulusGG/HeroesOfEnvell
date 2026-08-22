#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface
{
    GENERATED_BODY()
};

class HEROESOFENVELL_API IDamageableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    void ProcessDamage(float Amount, AActor* DamageCauser);
};