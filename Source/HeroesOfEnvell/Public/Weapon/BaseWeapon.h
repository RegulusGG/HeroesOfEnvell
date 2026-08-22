#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public\Service\ProjectTypes.h"
#include "BaseWeapon.generated.h"

UCLASS()
class HEROESOFENVELL_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;

public:

	void AttachToCharacter(ACharacter* TargetCharacter, bool bEquip);
	void PlayWeaponMontage(UAnimMontage* Montage, ACharacter* TargetCharacter);
	void ExecuteAttack(ACharacter* TargetCharacter);
	void OwnerArmedState(); //Меняет состояние после проигрывания монтажа удара
	virtual UAnimMontage* GetAttackMontage();
//Сокеты

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Placement")
	FName ActiveWeaponSocket;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement")
	FName RestingWeaponSocket;

//Анимации

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CombatAnim")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CombatAnim")
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CombatAnim")
	UAnimMontage* SheathingMontage;

//Общие характеристики

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	EWeaponAnimationStance StanceToApply;			//Показывает тип хвата оружия(Одноруч,двуруч и тд)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attr	ibutes")
	float Damage; //Урон самого оружия
};
