#pragma once

#include "CoreMinimal.h"
#include "Public/Interfaces/SaveableInterface.h"
#include "Public/Interfaces/CombatTargetInterface.h"
#include "Public/Character/Enemy/BaseEnemy.h"
#include "InputActionValue.h"
#include "Public/Service/ProjectTypes.h"
#include "Public\InterfaceVisual\Stances\RadialMenuWidget.h"
#include "BasePlayer.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldStrengthChanged, float, CurrentStrength, float, MaxStrength);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, float, CurrentMana, float, MaxMana);


UCLASS()
class HEROESOFENVELL_API ABasePlayer : public ABaseCharacter, public ICombatTargetInterface,public ISaveableInterface
{
	GENERATED_BODY()
	
class ABaseCharacter;

public:
	ABasePlayer();

protected:

	//База
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;


	virtual void OnSaveActor_Implementation(USaveGame* SaveGameObject) override;
	virtual void OnLoadActor_Implementation(USaveGame* SaveGameObject) override;


	UFUNCTION(BlueprintCallable)
	void Escape(const FInputActionValue& Value);

	UPROPERTY(BlueprintReadOnly, Category = "Menu")
	UUserWidget* ActiveMenuWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Menu")
	EPlayerMenuState ActiveMenu = EPlayerMenuState::Gameplay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu")
	TSubclassOf<UUserWidget> PlayerMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	//Камера

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float TargetArmLenght;

	bool bIsAiming = false;
	float DefaultFOV = 90.0f;
	float ZoomFOV = 60.f;


	//Статы игрока

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	FDefaultPlayerStats DefaultPlayerStats;

	UPROPERTY(EditAnywhere,	BlueprintReadOnly,	Category= "Attributes")
	FPlayerStats PlayerStats;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float CurrentMana;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxMana;
	
public:
	//Интерактивный мир

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class UInteractionComponent* InteractionComponent;

//Инпуты
// 
		//Базовое передвижение
	protected:

		void Move(const FInputActionValue& Value);
		void Look(const FInputActionValue& Value);
		void Roll(const FInputActionValue& Value);
		void StartSprint(const FInputActionValue& Value);
		void StopSprint(const FInputActionValue& Value);
		void ToggleWalk(const FInputActionValue& Value); 
		void StartAiming(const FInputActionValue& Value);
		
		UPROPERTY()
		bool bWalk = false;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* EscapeAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* MoveAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* SprintAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* LookAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* ZoomAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* JumpAction;

		UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Input")
		class UInputAction* RollAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* WalkAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* InteractAction;

		float LastForwardTapTime = -1.0f;
		float LastRightTapTime = -1.0f;

		UPROPERTY(EditAnywhere, Category = "Input")
		float DoubleTapTreshold = 0.25f;
	
		//Боевка

		void ShieldPressed(const FInputActionValue& Value);
		void ShieldReleased(const FInputActionValue& Value);
		void ShieldRegen(const FInputActionValue& Value);
		void OnMoveStarted(const FInputActionValue& Value);
		void Dodge(const FInputActionValue& Value);

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* EquipAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* AttackAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* ShieldAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* ShieldRegenAction;

		//Стойки
		
		virtual void EnterStanceSelection(const FInputActionValue& Value); //При зажатии бинда стоек
		void ConfirmStanceSelection(const FInputActionValue& Value); //При отпускании бинда стоек
		virtual TArray<FStanceStruct> PrepareStanceMenu(); //Подбирает актуальные стойки
		void OpenStanceMenu(TArray<FStanceStruct> Stances); //Отрисовывает меню выбора стойки
		void CloseStanceMenu(); //Закрывает виджет и прячет курсор
		void ToggleSlowmotion(bool bEnabled);//При зажатии кнопки стойки замедляет время
		virtual void ActivateStance();
			
		UPROPERTY()
		bool bIsSlowmotionEnabled = false;
			   
		UPROPERTY(EditAnywhere,BlueprintReadOnly)
		URadialMenuWidget* RadialMenu;
			   	
		UPROPERTY(EditAnywhere,BlueprintReadOnly)
		TSubclassOf<URadialMenuWidget> RadialMenuClass;
			  	  	
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* StanceMenuAction;

		FStanceStruct ActiveStanceStruct;

public:

	virtual void ProcessDamage_Implementation(float Amount, AActor* DamageCauser) override;

	//Стримы для актуальных данных в полосках Хп,Маны и тд.\

		UPROPERTY(BlueprintAssignable, Category = "Events") 
		FOnHealthChanged OnHealthChanged;

		UPROPERTY(BlueprintAssignable, Category = "Events") 
		FOnShieldStrengthChanged OnShieldStrengthChanged;

		UPROPERTY(BlueprintAssignable, Category = "Events") 
		FOnManaChanged OnManaChanged;
	


//Способности

	void InitAbilities() override;
	//Мувмент

		UPROPERTY()
		UBaseAbility* RollAbility;

		UPROPERTY(EditAnywhere, Category = "Abilities")
		TSubclassOf<UBaseAbility> RollAbilityClass;


		UPROPERTY()
		UBaseAbility* DodgeAbility;

		UPROPERTY(EditAnywhere, Category = "Abilities")
		TSubclassOf<UBaseAbility> DodgeAbilityClass;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Dodge")
	float DodgeRightValue; //Направление отпрыгивания при ДаблКлике W/A/S/D

	UPROPERTY(BlueprintReadOnly, Category = "Dodge")
	float DodgeForwardValue;  //Направление отпрыгивания при ДаблКлике W/A/S/D

	FVector2D LastMoveVector; // Хранит вектор последнего нажатия OnMoveStarted

//Стойки

protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "CombatState")
	bool bInFight = false;

public:

	UPROPERTY(VisibleInstanceOnly, Category = "CombatState")
	TSet<AActor*> AggroedEnemies;

public:
	void RegisterAggro_Implementation(AActor* Enemy) override;
	void UnregisterAggro_Implementation(AActor* Enemy) override;
	void UpdateCombatState();
	virtual void ResetAttackMontageIndex(EKnightStancesVariants StanceVariant);

	//Реген

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
	float HealthRegenAmount = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
	float ManaRegenAmount= 1.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Regeneration")
	float ShieldRegenAmount = 1.0f;

	float RegenRate = 0.2f;

	void RegenerateHealth();
	void RegenerateMana();
	void RegenerateShield();

	void StartHealthRegenerate();
	void StartManaRegenerate();
	void StartShieldRegenerate();

	void StopHealthRegen();
	void StopManaRegen();
	void StopShieldRegen();

	FTimerHandle HealthRegenTimerHandle;
	FTimerHandle ManaRegenTimerHandle;
	FTimerHandle ShieldRegenTimerHandle;
};
