// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Character\Enemy\BaseEnemy.h"
#include "Public\Character\Player\BasePlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SaveGame.h"
#include "Public/Service/SaveSystem/EnvellSaveGame.h"
#include "Public/Service/ProjectTypes.h"

ABaseEnemy::ABaseEnemy() 
{
};

void ABaseEnemy::OnDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ondeath in the baseenemy"));
	Super::OnDeath_Implementation();

	if (CurrentTargetPlayer)
	{
		CurrentTargetPlayer->Execute_UnregisterAggro(CurrentTargetPlayer, this);
		CurrentTargetPlayer = nullptr;
	}
}

void ABaseEnemy::OnSaveActor_Implementation(USaveGame* SaveGameObject)
{
	if (!SaveGameObject) return;

	UEnvellSaveGame* EnvellSaveGame = Cast<UEnvellSaveGame>(SaveGameObject);
	if (!EnvellSaveGame) return;
	
	FDefaultCharactersSaveData EnemySaveData;
	
	EnemySaveData.ActorClass = GetClass();
	EnemySaveData.ActorName = GetName();
	EnemySaveData.Transform = GetTransform();
	EnemySaveData.CurrentHealth = CurrentHealth;
	
	EnvellSaveGame->DefaultCharactersSaveDataArray.Add(EnemySaveData);
}

void ABaseEnemy::OnLoadActor_Implementation(USaveGame* SaveGameObject)
{		
	if (!SaveGameObject) return;

	UEnvellSaveGame* EnvellSaveGame = Cast<UEnvellSaveGame>(SaveGameObject);
	if (!EnvellSaveGame) return;

	CurrentHealth = EnvellSaveGame->DefaultCharactersSaveDataArray[0].CurrentHealth;

	EnvellSaveGame->DefaultCharactersSaveDataArray.RemoveAt(0);
}

void ABaseEnemy::Destroyed()
{
	Super::Destroyed();

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}
}