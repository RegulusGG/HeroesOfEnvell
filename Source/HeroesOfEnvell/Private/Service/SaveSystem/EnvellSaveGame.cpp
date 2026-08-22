// Fill out your copyright notice in the Description page of Project Settings.


#include "Service/SaveSystem/EnvellSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"


void UEnvellSaveGame::SpawnSavedEnemy(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;

	APawn* PlayerActor = UGameplayStatics::GetPlayerPawn(World, 0);
	if (PlayerActor && PlayerActor->Implements<USaveableInterface>())
	{
		ISaveableInterface::Execute_OnLoadActor(PlayerActor, this);
	}


	TArray<FDefaultCharactersSaveData> TempDefaultCharacters = DefaultCharactersSaveDataArray;
	for (const FDefaultCharactersSaveData& Data : TempDefaultCharacters)
	{
		if (!Data.ActorClass) continue;

		AActor* SpawnedActor = World->SpawnActor<AActor>(Data.ActorClass, Data.Transform);
		if (SpawnedActor && SpawnedActor->Implements<USaveableInterface>())
		{
			ISaveableInterface::Execute_OnLoadActor(SpawnedActor, this);
		}
	}
}