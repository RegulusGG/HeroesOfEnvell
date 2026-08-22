// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public\Service\ProjectTypes.h"
#include "Public\Ability\BaseAbility.h"
#include "Public\Ability\Movement\RollAbility.h"
#include "Public\Ability\Protection\ShieldAbility.h"
#include "Public\Interfaces\DamageableInterface.h"
#include "Public\Weapon\Equipment\Shield\BaseShield.h"
#include "BaseCharacter.generated.h"


UCLASS()
class HEROESOFENVELL_API ABaseCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


//Базовые параметры каждого перосонажа

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float CurrentHealth;

public:
	UPROPERTY(EditAnywhere, Category = "Attributes")
	FDefaultCharacterStats DefaultCharacterStats;
	
	UPROPERTY(EditAnywhere, Category = "Attributes")
	FStanceCharacterStats CharacterStats;

	int ArmorConstant = 35; //Отвечает за ценность единцицы брони. При понижении броня становится эффективнее

//Базовое передвижение всех юнитов

protected:
	
	void Internal_MoveForward(float Value);
	void Internal_MoveRight(float Value);
	void Internal_Look(FVector2D Value);

	UPROPERTY(EditAnywhere)
	float SprintMovespeed; //Спринт

	UPROPERTY(EditAnywhere)
	float RunMovespeed; // Обычное передвижение без спринта

	UPROPERTY(EditAnywhere)
	float WalkMovespeed; // Медленная ходьба

	UPROPERTY(EditAnywhere)
	float ShieldMovespeed;

//Анимации

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	EWeaponState WeaponState; //Проверяет общее состояние игрока. Нужен чтобы не спамить атакой бесконечно возвращая анимацию в начало или бесконечно спамить извлечение и убирание меча

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	EWeaponAnimationStance WeaponAnimationState; //Меняет проигрываемые анимации. Например с арбалетом в руке проигрывется одна Idle, с двуручным мечом другая.

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CombatAnim")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CombatAnim")
	UAnimMontage* HitReactionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CombatAnim")
	UAnimMontage* ShieldHitReactionMontage;

//Боевка
protected:
	
	//Функции
	void SpawnWeapon();
	void ToggleWeapon();
	float CountDamage(float Amount, AActor* DamageCauser);

	UPROPERTY()
	float LastReceivedDamage = 0.0f;;
	
	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Placement")
	void HandleWeaponAttach(bool bEquip);

	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void OnDeath();


	//Переменные
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<class ABaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	ABaseWeapon* CurrentWeapon;

public:

	UPROPERTY(EditAnywhere, Category = "Shield")
	TSubclassOf<ABaseShield>ShieldClass;

	UPROPERTY(EditAnywhere, Category = "Shield")
	ABaseShield* CurrentShield;

public:

	UFUNCTION(BlueprintCallable)
	void StartAttackState();

	UFUNCTION(BlueprintCallable)
	void EndAttackState();

public:
	ABaseWeapon* GetCurrentWeapon();

	virtual void ProcessDamage_Implementation(float Amount, AActor* DamageCauser) override;

//Способности

	bool bIsAbilityActive = false;

	virtual void InitAbilities();

	//Щит

		void SpawnShield();
		
		UPROPERTY(BlueprintReadOnly)
		bool bIsShieldBlocking = false;
	
		UPROPERTY()
		UBaseAbility* ShieldAbility;

		UPROPERTY(EditAnywhere, Category = "Abilities")
		TSubclassOf<UBaseAbility> ShieldAbilityClass;

		UPROPERTY(EditAnywhere, Category = "Shield")
		float BlockMovementSpeed = 200.0f;
};
