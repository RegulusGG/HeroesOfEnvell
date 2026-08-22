// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Ability\Protection\ShieldAbility.h"
#include "Public\Character\Player\BasePlayer.h"
#include "Public\Character\BaseCharacter.h"

bool UShieldAbility::ActivateAbility()
{
	if (!Super::ActivateAbility()) return false;
	
	if (!CharacterOwner) return false;

	LastState = CharacterOwner->WeaponState;

	CharacterOwner->WeaponState = EWeaponState::Shield;

	if (CharacterOwner->CurrentShield->SetShieldVisible(true))
	{
		CharacterOwner->bIsShieldBlocking = true;
		return true;
	}
	return false;
}

void UShieldAbility::DeactivateAbility()
{
	Super::DeactivateAbility();

	if (!CharacterOwner || !CharacterOwner->CurrentShield) return;

	CharacterOwner->CurrentShield->SetShieldVisible(false);
	CharacterOwner->bIsShieldBlocking = false;
	CharacterOwner->WeaponState = LastState;
}

void UShieldAbility::RegenShield()
{
	if (!CharacterOwner || !CharacterOwner->CurrentShield)	return;
	
	ABasePlayer* PlayerOwner;
	PlayerOwner = Cast<ABasePlayer>(CharacterOwner);
	if (!PlayerOwner) return;

	float RegenManaCost = ShieldRegenManaCost;
	float StrengthDelta = PlayerOwner->CurrentShield->MaxStrength - PlayerOwner->CurrentShield->CurrentStrength;
	
	if (StrengthDelta < ShieldRegenManaCost)
	{
		RegenManaCost = StrengthDelta;
	}
	
	if (PlayerOwner->CurrentMana < RegenManaCost) { GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Недостаточно маны"));return; }
	
	PlayerOwner->CurrentMana -= RegenManaCost;
	PlayerOwner->CurrentShield->CurrentStrength += RegenManaCost;
}