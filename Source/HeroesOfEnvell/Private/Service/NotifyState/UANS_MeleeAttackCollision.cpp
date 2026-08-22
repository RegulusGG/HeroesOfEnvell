// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Service\NotifyState\UANS_MeleeAttackCollision.h"
#include "Public\Character\BaseCharacter.h"

AMeleeWeapon* UUANS_MeleeAttackCollision::GetMeleeWeapon(USkeletalMeshComponent* MeshComp)
{
	if (!MeshComp) return nullptr;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return nullptr;

	ABaseCharacter* Character = Cast<ABaseCharacter>(Owner);
	if (Character && Character->GetCurrentWeapon())
	{
		ABaseWeapon* CurrentWeapon = Character->GetCurrentWeapon();
		AMeleeWeapon* MeleeWeapon = Cast<AMeleeWeapon>(CurrentWeapon);
		return MeleeWeapon;
	}

	return nullptr;
}


void UUANS_MeleeAttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{

	if (!MeshComp) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->StartAttackState();

		AMeleeWeapon* MeleeWeapon = GetMeleeWeapon(MeshComp);
		if (MeleeWeapon)
		{
			MeleeWeapon->ActivateCollision();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("collision activated"));
		}
	}
}

void UUANS_MeleeAttackCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->EndAttackState();

		AMeleeWeapon* MeleeWeapon = GetMeleeWeapon(MeshComp);
		if (MeleeWeapon)
		{
			MeleeWeapon->DeactivateCollision();
		}
	}
}