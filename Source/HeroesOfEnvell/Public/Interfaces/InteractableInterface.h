#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

class HEROESOFENVELL_API IInteractable
{
    GENERATED_BODY()

public:
    virtual void OnInteract(AActor* Interactor) = 0;
};