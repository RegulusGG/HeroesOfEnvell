// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Weapon\Equipment\Shield\BaseShield.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/PrimitiveComponent.h"

bool ABaseShield::SetShieldVisible(bool bVisible)
{
	if (CurrentStrength <= 0 && bVisible) 
	{ 
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Щит разряжен"));
		return false; 
	}

	SetActorHiddenInGame(!bVisible);
	SetActorTickEnabled(bVisible);

	if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		Primitive->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);

		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			Primitive->IgnoreActorWhenMoving(MyOwner, bVisible);
		}
	}


	return true;
}

bool ABaseShield::CanBlockAttack(const FVector& DamageOrigin)
{
	if (CurrentStrength <= 0) 
	{ 
		SetShieldVisible(false);
		return false; 
	}

	AActor* ShieldOwner = GetOwner();  
	if (!ShieldOwner) return false;

	APawn* OwnerPawn = Cast<APawn>(ShieldOwner);
	if (!OwnerPawn) return false;

	FVector ViewVector = UKismetMathLibrary::GetForwardVector(OwnerPawn->GetControlRotation());
	ViewVector.Z = 0.f;
	ViewVector.Normalize();

	FVector ToDamage = (DamageOrigin - OwnerPawn->GetActorLocation());
	ToDamage.Z = 0.f;

	if (ToDamage.IsNearlyZero(0.001f)) return true;
	ToDamage.Normalize();

	float Dot = FVector::DotProduct(ViewVector, ToDamage);

	return Dot >= BlockThreshold;
}