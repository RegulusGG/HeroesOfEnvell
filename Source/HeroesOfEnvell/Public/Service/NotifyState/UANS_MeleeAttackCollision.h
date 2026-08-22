// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Public\Weapon\Melee\MeleeWeapon.h"
#include "UANS_MeleeAttackCollision.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFENVELL_API UUANS_MeleeAttackCollision : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	AMeleeWeapon* GetMeleeWeapon(USkeletalMeshComponent* MeshComp);
};
