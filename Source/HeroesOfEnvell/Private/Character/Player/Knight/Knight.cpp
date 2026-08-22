// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Character\Player\Knight\Knight.h"
#include "Public\Service\ProjectTypes.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Public/Weapon/Melee/Sword/Knight/KnightSword.h"
#include "GameFramework/CharacterMovementComponent.h"

void AKnight::BeginPlay()
{
	Super::BeginPlay();

	KnightWeapon = EKnightCurrentWeapon::Sword;
}

TArray<FStanceStruct> AKnight::PrepareStanceMenu()
{
	if (!bInFight) { return ExplorationStances; }
	else 
	{
		if (KnightWeapon == EKnightCurrentWeapon::Sword) return SwordStances;
		else if (KnightWeapon == EKnightCurrentWeapon::Shotgun) return ShotgunStances;
	}	
	return TArray<FStanceStruct>();
}

void AKnight::ActivateStance()
{
	CharacterStats.ArmorMultiplier = ActiveStanceStruct.Modifiers.ArmorMultiplier;
	CharacterStats.DamageMultiplier = ActiveStanceStruct.Modifiers.DamageMultiplier;
	CharacterStats.ArmorIgnore = DefaultCharacterStats.DefaultArmorIgnore + ActiveStanceStruct.Modifiers.ArmorIgnoreBonus;
	CharacterStats.MissChance = ActiveStanceStruct.Modifiers.MissChanceBonus;
	CharacterStats.CritChance = ActiveStanceStruct.Modifiers.CritChanceBonus;
	CharacterStats.CritDamage = ActiveStanceStruct.Modifiers.CritDamageBonus;
	CharacterStats.bIsIgnoreMiss = ActiveStanceStruct.Modifiers.bIgnoreMiss;
	CharacterStats.TargetPerHit = ActiveStanceStruct.Modifiers.TargetPerHit;
	CharacterStats.MovespeedMultiplier = ActiveStanceStruct.Modifiers.MovespeedMultiplier;
	GetCharacterMovement()->JumpZVelocity = ActiveStanceStruct.Modifiers.JumpForce;
	GetCharacterMovement()->MaxWalkSpeed = RunMovespeed * ActiveStanceStruct.Modifiers.MovespeedMultiplier;

	FText FormattedText = FText::Format(
		NSLOCTEXT("YourNamespace", "StanceKey", "Stance changed: {0}"),
		ActiveStanceStruct.Data.StanceName
	);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FormattedText.ToString());
	ActiveStance = ActiveStanceStruct.ActiveStance;
}

void AKnight::ResetAttackMontageIndex(EKnightStancesVariants StanceVariant)
{	
	if (ActiveStance == StanceVariant) return;

	AKnightSword* KnightSword = Cast<AKnightSword>(CurrentWeapon);
	if (!KnightSword) return; 
	
	KnightSword->MontageIndex = 0;
}
