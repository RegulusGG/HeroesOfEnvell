#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Public/Service/ProjectTypes.h"
#include "Public/Service/SaveSystem/EnvellSaveGame.h"
#include "SaveSystemBPFunctionLibrary.generated.h"



UCLASS()
class HEROESOFENVELL_API USaveSystemBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	static TArray<FSaveSlotInfo> GetSaveGameSlots();

	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	static UEnvellSaveGame* GetSaveInstance(FString Filename);
};
