#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveableInterface.generated.h"

class USaveGame;

UINTERFACE(MinimalAPI)
class USaveableInterface : public UInterface
{
    GENERATED_BODY()
};

class HEROESOFENVELL_API ISaveableInterface
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save System")
    void OnSaveActor(USaveGame* SaveGameObject);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save System")
    void OnLoadActor(USaveGame* SaveGameObject);
};