// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public\Character\BaseCharacter.h"
#include "Public\Character\Player\BasePlayer.h"
#include "Public\Service\ProjectTypes.h"
#include "Knight.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECombatStands : uint8
{
	StrengthStyle UMETA(DisplayName = "StrengthStyle"),
	FulminantStyle UMETA(DisplayName = "FulminantStyle"),
	MassStyle UMETA(DisplayName = "MassStyle")
};

UENUM(BlueprintType)
enum class EExplorationStands: uint8
{
	DefaultStyle UMETA(DisplayName = "DefaultStyle"),
	ExpressStyle UMETA(DisplayName = "ExpressStyle"),
	JumpStyle UMETA(DisplayName = "JumpStyle")
};

UCLASS()
class HEROESOFENVELL_API AKnight : public ABasePlayer
{
	GENERATED_BODY()

public:
	
	void BeginPlay() override;

protected:

	//void ToggleWeapon() override;

//Стойки
	UPROPERTY()
	EKnightCurrentWeapon KnightWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stance")
	TArray<FStanceStruct> SwordStances;
		   			  		    		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stance")
	TArray<FStanceStruct> ShotgunStances;
		   					 			
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stance")
	TArray<FStanceStruct> ExplorationStances;

	UFUNCTION()
	TArray<FStanceStruct> PrepareStanceMenu() override;
	
	UFUNCTION()
	void ActivateStance() override;

	UFUNCTION()
	void ResetAttackMontageIndex(EKnightStancesVariants StanceVariant) override;

public:
	UPROPERTY()
	EKnightStancesVariants ActiveStance;
};