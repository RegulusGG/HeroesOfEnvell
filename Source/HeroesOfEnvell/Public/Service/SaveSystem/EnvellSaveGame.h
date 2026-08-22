// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Public/Service/ProjectTypes.h"
#include "Public/Character/Player/BasePlayer.h"
#include "Public/Weapon/BaseWeapon.h"
#include "EnvellSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API UEnvellSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<FDefaultCharactersSaveData> DefaultCharactersSaveDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	FPlayerSaveData PlayerSaveData;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void SpawnSavedEnemy(const UObject* WorldContextObject);
};
