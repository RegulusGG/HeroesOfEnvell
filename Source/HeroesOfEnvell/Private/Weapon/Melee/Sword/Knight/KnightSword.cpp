// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Melee/Sword/Knight/KnightSword.h"
#include "Public/Service/ProjectTypes.h"
#include "Public\Weapon\BaseWeapon.h"
#include "Components/BoxComponent.h"
#include "Public/Character/Player/Knight/Knight.h"

AKnightSword::AKnightSword()
{
	SetActorHiddenInGame(!bVisible);
}

UAnimMontage* AKnightSword::GetAttackMontage()
{
	AActor* Pawn = GetOwner();
	if (!Pawn) return nullptr;

	AKnight* KnightPawn = Cast<AKnight>(Pawn);
	if (!KnightPawn) return nullptr;

	switch (KnightPawn->ActiveStance)
	{
	case EKnightStancesVariants::StrengthSword:
	
		MontageIndex = (MontageIndex % StrengthMontages.Num()) + 1;
		return StrengthMontages[(MontageIndex - 1) % StrengthMontages.Num()];
	
	case EKnightStancesVariants::FastSword:

		MontageIndex = (MontageIndex % FastMontages.Num()) + 1;
		return FastMontages[(MontageIndex - 1) % FastMontages.Num()];

	case EKnightStancesVariants::MassSword:
		
		MontageIndex = (MontageIndex % MassMontages.Num()) + 1;
		return MassMontages[(MontageIndex - 1) % MassMontages.Num()];

	default:

		MontageIndex = (MontageIndex % ExplorationMontages.Num()) + 1;
		return ExplorationMontages[(MontageIndex - 1) % ExplorationMontages.Num()];
	}
}

void AKnightSword::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* ActorOwner = GetOwner();
	AKnight* KnightOwner = Cast<AKnight>(ActorOwner);
	if (!ActorOwner || !KnightOwner) return;

	if (AlreadyHitActors.Num() >= KnightOwner->CharacterStats.TargetPerHit) return;

	Super::OnWeaponOverlap(OverlappedComponent,OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}