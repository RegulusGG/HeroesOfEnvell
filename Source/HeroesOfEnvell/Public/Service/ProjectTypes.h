#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "ProjectTypes.generated.h"

UENUM(BlueprintType)
enum class EWeaponAnimationStance : uint8 //Отвечает за визуальную составляющую. 
	//Пример: Если игрок встал на месте и персонаж держит арбалеты 
	// то его WeaponAnimationStance = TwoHanded
	// И начнется соответствующая Idle анимация. Если же руки пусты 
	// то соответственно анимка для этого случая 
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	OneHanded UMETA(DisplayName = "OneHanded"),
	TwoHanded UMETA(DisplayName = "TwoHanded"),
	DualWield UMETA(DisplayName = "DualWield")
};

UENUM(BlueprintType)
enum class EKnightCurrentWeapon : uint8
{
	Shotgun UMETA(DisplayName = "Shotgun"),
	Sword UMETA(DisplayName = "Sword")
};

UENUM(BlueprintType)
enum class EWeaponState : uint8 //Отвечает за общее состояние персонажа. Помогает в логике. 
	//Пример: Если в функции ToggleWeapon вызывать ABaseCharacter->WeaponState 
	// и оно окажется Unarmed - он извлечет оружие, в противном случае уберет.
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	Equipping UMETA(DisplayName = "Equipping"),
	Armed UMETA(DisplayName = "Armed"),
	Shield UMETA(DisplayName = "Shield"),
	Attacking UMETA(DisplayName = "Attacking"),
	Sheathing UMETA(DisplayName = "Sheathing")
};

USTRUCT(BlueprintType)
struct FStanceModifiers
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ArmorMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovespeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere)
	float JumpForce = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ArmorIgnoreBonus = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackSpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIgnoreMiss = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CritChanceBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CritDamageBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TargetPerHit = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MissChanceBonus = 0.0f;

};

USTRUCT(BlueprintType)
struct FStanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* StanceIcon = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText StanceName;

};

UENUM(BlueprintType)
enum class EKnightStancesVariants : uint8//Для определения какую анимацию удара проигрывать(Массовую, Молниеносную, Силовую)
{
	DefaultExploration UMETA(DisplayName = "DefaultExploration"),
	SpeedExploration UMETA(DisplayName = "SpeedExploration"),
	JumpExploration UMETA(DisplayName = "JumpExploration"),
	MassSword UMETA(DisplayName = "MassSword"),
	FastSword UMETA(DisplayName = "FastSword"),
	StrengthSword UMETA(DisplayName = "StrengthSword")
};

USTRUCT(BlueprintType)
struct FStanceStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FStanceData Data;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FStanceModifiers Modifiers;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	EKnightStancesVariants ActiveStance;
};


USTRUCT(BlueprintType)
struct FDefaultCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float DefaultMovespeed;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float DefaultDamage = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float DefaultArmor = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float DefaultMissChance = 0.0f; //Шанс что по тебе промажут

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float DefaultArmorIgnore = 0.0f; // Процент игнорируемого армора

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float DefaultCritChance = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Attibutes")
	float DefaultCritDamage = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	bool bIsIgnoreMiss = false;

	int32 DefaultTargetPerHit = 1.0f;

	bool bIsInvulnerable = false; //Неуязвим ли? Включается в кувырках и тп.
};

USTRUCT(BlueprintType)
struct FStanceCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float ArmorMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MovespeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MissChance = 0.0f; //Шанс что по тебе промажут

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float ArmorIgnore = 0.0f; // Процент игнорируемого армора

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float CritChance = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Attibutes")
	float CritDamage = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	bool bIsIgnoreMiss = false;

	int32 TargetPerHit =1.0f;

	bool bIsInvulnerable = false; //Неуязвим ли? Включается в кувырках и тп.

};

USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float Experience; //Количество накопленного опыта

	UPROPERTY(EditAnywhere, Category = "Attributes")
	int32 TargetPerHit = 1.0f; //Количесто врагов по которым можно попасть за удар.
};

USTRUCT(BlueprintType)
struct FDefaultPlayerStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float AttackCooldown = 1.0f;
};

UENUM(BlueprintType)
enum class EPlayerMenuState : uint8 //Показывает какое действие совершить при нажатии Ecs(выйти из меню персонажа, открыть/закрыть меню паузы)
{
	Gameplay UMETA(DisplayName = "Gameplay"),
	PauseMenu UMETA(DisplayName = "PauseMenu"),
	PlayerMenu UMETA(DisplayName = "PlayerMenu")
};

USTRUCT(BlueprintType)
struct FDefaultCharactersSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString ActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float CurrentHealth;
};

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	FTransform PlayerTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	float PlayerCurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	float PlayerMaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	float PlayerMaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	float PlayerCurrentMana;
};

USTRUCT(BlueprintType)
struct FSaveSlotInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "SaveData")
	FString SlotName;

	UPROPERTY(BlueprintReadOnly, Category = "SaveData")
	FString SaveDateTime;
};
