// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Ability\Movement\RollAbility.h"
#include "Public\Character\BaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"

bool URollAbility::ActivateAbility()
{
	if (!Super::ActivateAbility()) return false;

	if (CharacterOwner && RollMontage)
	{
		float Duration = CharacterOwner->PlayAnimMontage(RollMontage);

		FTimerHandle TimerHandle;
		CharacterOwner->GetWorldTimerManager().SetTimer(TimerHandle, this, &URollAbility::DeactivateAbility, Duration, false);
		return true;
	}

	DeactivateAbility();
	return false;
}