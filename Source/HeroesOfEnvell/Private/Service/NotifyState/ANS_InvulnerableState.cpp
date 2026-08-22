// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Service\NotifyState\ANS_InvulnerableState.h"
#include "Public\Character\BaseCharacter.h"

void UANS_InvulnerableState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->CharacterStats.bIsInvulnerable = true;
	}
}

void UANS_InvulnerableState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->CharacterStats.bIsInvulnerable = false;
	}
}