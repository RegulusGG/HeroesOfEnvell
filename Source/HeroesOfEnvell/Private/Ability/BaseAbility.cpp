// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Ability\BaseAbility.h"
#include "Public\Character\Player\BasePlayer.h"
#include "Public\Character\BaseCharacter.h"

void UBaseAbility::Init(ABaseCharacter* Character)
{
	if (Character)
	{
		CharacterOwner = Character;
	}
}

bool UBaseAbility::ActivateAbility()
{
	if (CharacterOwner && !CharacterOwner->bIsAbilityActive)
	{
		ABasePlayer* PlayerOwner;

		if (Cast<ABasePlayer>(CharacterOwner))
		{
		
			PlayerOwner = Cast<ABasePlayer>(CharacterOwner);
			if (!PlayerOwner) return false;

			if (ManaCost > PlayerOwner->CurrentMana) { GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Недостаточно маны")); return false; }
			
			PlayerOwner->bIsAbilityActive = true;
			PlayerOwner->CurrentMana -= ManaCost;
			PlayerOwner->OnManaChanged.Broadcast(PlayerOwner->CurrentMana, PlayerOwner->MaxMana);

			PlayerOwner->StartManaRegenerate();
			return true;
		}
	
		CharacterOwner->bIsAbilityActive = true;
		return true;
	}
	return false;
}

void UBaseAbility::DeactivateAbility()
{
	if (CharacterOwner)
	{
		CharacterOwner->bIsAbilityActive = false;
	}
}