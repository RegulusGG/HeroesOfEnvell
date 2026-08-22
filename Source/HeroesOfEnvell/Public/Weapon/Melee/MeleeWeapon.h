// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public\Weapon\BaseWeapon.h"
#include "Components/BoxComponent.h"
#include "MeleeWeapon.generated.h"


UCLASS()
class HEROESOFENVELL_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	AMeleeWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* WeaponCollision;
	

	void ActivateCollision();
	void DeactivateCollision();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	
	TArray<AActor*> AlreadyHitActors;
};
