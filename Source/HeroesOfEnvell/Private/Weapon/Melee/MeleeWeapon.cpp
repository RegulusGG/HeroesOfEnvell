// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Weapon\Melee\MeleeWeapon.h"
#include "Public\Character\BaseCharacter.h"
#include "Public\Interfaces\DamageableInterface.h"
#include "Kismet/GameplayStatics.h"
 
AMeleeWeapon::AMeleeWeapon()
{
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));

	WeaponCollision->SetupAttachment(GetRootComponent());
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnWeaponOverlap);
}

void AMeleeWeapon::ActivateCollision()
{
	if (WeaponCollision)
	{
		AlreadyHitActors.Empty();
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AMeleeWeapon::DeactivateCollision()
{
	if (WeaponCollision)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMeleeWeapon::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner() && !AlreadyHitActors.Contains(OtherActor))
	{
		if (OtherActor->GetClass()->ImplementsInterface(UDamageableInterface::StaticClass()))
		{
			AlreadyHitActors.Add(OtherActor);
			IDamageableInterface::Execute_ProcessDamage(OtherActor, Damage, GetOwner());
		}
	}
}