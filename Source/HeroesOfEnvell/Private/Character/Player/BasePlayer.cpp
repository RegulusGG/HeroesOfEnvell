 // Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Character\Player\BasePlayer.h"
#include "Public\Character\BaseCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "Public\Ability\Movement\DodgeAbility.h"
#include "Public\Ability\Protection\ShieldAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Public/Character/Player/InteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Public/Service/SaveSystem/EnvellSaveGame.h"
#include "GameFramework/SaveGame.h"

ABasePlayer::ABasePlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	SpawnShield(); 

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComp_FINAL_FIX"));
	InteractionComponent->SetupAttachment(GetMesh());
}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	SpawnShield();
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bIsAiming ? ZoomFOV : DefaultFOV;

	float CurrentFOV = FollowCamera->FieldOfView;
	float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 10.0f);

	FollowCamera->SetFieldOfView(NewFOV);
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayer::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ABasePlayer::OnMoveStarted);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayer::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayer::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ABasePlayer::ToggleWeapon);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ABasePlayer::Attack);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ABasePlayer::Roll);
		EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Started, this, &ABasePlayer::ShieldPressed);
		EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Completed, this, &ABasePlayer::ShieldReleased);
		EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Canceled, this, &ABasePlayer::ShieldReleased);
		EnhancedInputComponent->BindAction(ShieldRegenAction, ETriggerEvent::Started, this, &ABasePlayer::ShieldRegen);
		EnhancedInputComponent->BindAction(StanceMenuAction, ETriggerEvent::Started, this, &ABasePlayer::EnterStanceSelection);
		EnhancedInputComponent->BindAction(StanceMenuAction, ETriggerEvent::Completed, this, &ABasePlayer::ConfirmStanceSelection);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABasePlayer::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasePlayer::StopSprint);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &ABasePlayer::ToggleWalk);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &ABasePlayer::StartAiming);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &ABasePlayer::StartAiming);
		EnhancedInputComponent->BindAction(EscapeAction, ETriggerEvent::Started, this, &ABasePlayer::Escape);
	}
}

void ABasePlayer::InitAbilities()
{
	if (RollAbilityClass)
	{
		RollAbility = NewObject<URollAbility>(this, RollAbilityClass);

		if (RollAbility)
		{
			RollAbility->Init(this);
		}
	}

	if (DodgeAbilityClass)
	{
		DodgeAbility = NewObject<UDodgeAbility>(this, DodgeAbilityClass);

		if (DodgeAbility)
		{
			DodgeAbility->Init(this);
		}
	}

	if (ShieldClass)
	{
		ShieldAbility = NewObject<UShieldAbility>(this, ShieldAbilityClass);

		if (ShieldAbility)
		{
			ShieldAbility->Init(this);
		}
	}
}

void ABasePlayer::ProcessDamage_Implementation(float Amount, AActor* DamageCauser)
{
	Super::ProcessDamage_Implementation(Amount,DamageCauser);

	if (bIsShieldBlocking && CurrentShield)
	{
		if (CurrentShield->CanBlockAttack(DamageCauser->GetActorLocation()))
		{
			OnShieldStrengthChanged.Broadcast(CurrentShield->CurrentStrength-LastReceivedDamage, CurrentShield->MaxStrength);
			StartShieldRegenerate();
		}
	}       


	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	StartHealthRegenerate();
}

//Действия

void ABasePlayer::OnSaveActor_Implementation(USaveGame* SaveGameObject)
{
	if (!SaveGameObject) return;

	UEnvellSaveGame* EnvellSaveGame = Cast<UEnvellSaveGame>(SaveGameObject);
	if (!EnvellSaveGame) return;

	EnvellSaveGame->PlayerSaveData.PlayerTransform = GetTransform();
	EnvellSaveGame->PlayerSaveData.PlayerCurrentHealth = CurrentHealth;
	EnvellSaveGame->PlayerSaveData.PlayerMaxHealth = MaxHealth;
	EnvellSaveGame->PlayerSaveData.PlayerCurrentMana = CurrentMana;
	EnvellSaveGame->PlayerSaveData.PlayerMaxMana = MaxMana;

}

void ABasePlayer::OnLoadActor_Implementation(USaveGame* SaveGameObject)
{
	if (!SaveGameObject) return;

	UEnvellSaveGame* EnvellSaveGame = Cast<UEnvellSaveGame>(SaveGameObject);
	if (!EnvellSaveGame) return;
	
	CurrentHealth = EnvellSaveGame->PlayerSaveData.PlayerCurrentHealth;
	MaxHealth = EnvellSaveGame->PlayerSaveData.PlayerMaxHealth;
	CurrentMana = EnvellSaveGame->PlayerSaveData.PlayerCurrentMana;
	MaxMana = EnvellSaveGame->PlayerSaveData.PlayerMaxMana;
	SetActorTransform(EnvellSaveGame->PlayerSaveData.PlayerTransform);



}
void ABasePlayer::Escape(const FInputActionValue& Value)
{
	if (!PauseMenuWidgetClass || !PlayerMenuWidgetClass) return;

	APlayerController* PC = Cast<APlayerController>(GetController());
	
	switch (ActiveMenu)
	{
	case EPlayerMenuState::Gameplay: 
		if (!PC) return;

		ActiveMenuWidget = CreateWidget<UUserWidget>(PC, PauseMenuWidgetClass);
		if (ActiveMenuWidget)
		{ 
			ActiveMenuWidget->AddToViewport();
			ActiveMenu = EPlayerMenuState::PauseMenu;
			PC->SetPause(true);

			if (PC)
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(ActiveMenuWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
			}
		}
		break;

	default:
		if (!ActiveMenuWidget) return;
		ActiveMenuWidget->RemoveFromParent();
		ActiveMenuWidget = nullptr;

		ActiveMenu = EPlayerMenuState::Gameplay; 
		PC->SetPause(false);

		if (PC)
		{

			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = false;
		}

		break;
	}
}

void ABasePlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	Internal_MoveForward(MovementVector.Y);
	Internal_MoveRight(MovementVector.X);
}

void ABasePlayer::OnMoveStarted(const FInputActionValue& Value) //Логика отпрыгивания на даблклик WASD
{
	FVector2D CurrentMoveVector = Value.Get<FVector2D>();
	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentTime - LastForwardTapTime < DoubleTapTreshold && CurrentMoveVector.Equals(LastMoveVector, 0.1f))
	{
		DodgeForwardValue = CurrentMoveVector.Y;
		DodgeRightValue = CurrentMoveVector.X;
		DodgeAbility->ActivateAbility();

		LastForwardTapTime = -1.0f;
		LastMoveVector = FVector2D::ZeroVector;
	}
	else { LastForwardTapTime = CurrentTime;LastMoveVector = CurrentMoveVector;}
}

void ABasePlayer::StartSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = SprintMovespeed * CharacterStats.MovespeedMultiplier;
}  

void ABasePlayer::StopSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = RunMovespeed * CharacterStats.MovespeedMultiplier;
}

void ABasePlayer::ToggleWalk(const FInputActionValue& Value)
{
	bWalk = !bWalk;
	
	if (bWalk) { GetCharacterMovement()->MaxWalkSpeed = WalkMovespeed * CharacterStats.MovespeedMultiplier; }
	if (!bWalk) { GetCharacterMovement()->MaxWalkSpeed = RunMovespeed * CharacterStats.MovespeedMultiplier; }
}

void ABasePlayer::Look(const FInputActionValue& Value)
{
	Internal_Look(Value.Get<FVector2D>());
}

void ABasePlayer::StartAiming(const FInputActionValue& Value)
{
	bIsAiming = Value.Get<bool>();
}



void ABasePlayer::Roll(const FInputActionValue& Value)
{
	if (RollAbility && !bIsAbilityActive)
	{
		RollAbility->ActivateAbility();
	}
}

void ABasePlayer::ShieldPressed(const FInputActionValue& Value)
{
	if (ShieldAbility) ShieldAbility->ActivateAbility();
	DefaultCharacterStats.DefaultMovespeed = ShieldMovespeed;
}
	
void ABasePlayer::ShieldReleased(const FInputActionValue& Value)
{
	if (ShieldAbility) ShieldAbility->DeactivateAbility();
	DefaultCharacterStats.DefaultMovespeed = RunMovespeed;
}

void ABasePlayer::ShieldRegen(const FInputActionValue& Value) //Реген щита за ману
{
	if (!ShieldAbility || !bIsShieldBlocking)	return;

	UShieldAbility* RegenShieldAbility;
	RegenShieldAbility = Cast<UShieldAbility>(ShieldAbility);
	if (!RegenShieldAbility) return;

	RegenShieldAbility->RegenShield();
}

void ABasePlayer::EnterStanceSelection(const FInputActionValue& Value)
{ 
	bIsSlowmotionEnabled = true;
	ToggleSlowmotion(bIsSlowmotionEnabled);

	TArray<FStanceStruct> Stances = PrepareStanceMenu();
	OpenStanceMenu(Stances);
}

void ABasePlayer::ConfirmStanceSelection(const FInputActionValue& Value)
{
	TArray<FStanceStruct> Stances = PrepareStanceMenu();	
	int32 CurrentIndex = RadialMenu->GetCurrentIndex();
	
	if (Stances.IsValidIndex(CurrentIndex))
	{
		ResetAttackMontageIndex(Stances[CurrentIndex].ActiveStance);
		ActiveStanceStruct = Stances[CurrentIndex];
		ActivateStance();
	}	

	CloseStanceMenu();
	bIsSlowmotionEnabled = false;
	ToggleSlowmotion(bIsSlowmotionEnabled);
}

//Стойки

void ABasePlayer::RegisterAggro_Implementation(AActor* Enemy)
{
	if (!Enemy) return;

	int32 OldCount = AggroedEnemies.Num();

	AggroedEnemies.Add(Enemy);
	
	if (OldCount == 0 && AggroedEnemies.Num() > 0)
	{
		UpdateCombatState();
	}
}

void ABasePlayer::UnregisterAggro_Implementation(AActor* Enemy)
{
	if (!Enemy) return;

	int32 OldCount = AggroedEnemies.Num();
	AggroedEnemies.Remove(Enemy);

	if (OldCount > 0 && AggroedEnemies.Num() == 0)
	{
		UpdateCombatState();
	}
}

void ABasePlayer::UpdateCombatState()
{
	bInFight = AggroedEnemies.Num() > 0;
}

void ABasePlayer::ToggleSlowmotion(bool bEnabled)
{
	float TargetTimeDilation;

	if (bEnabled) { TargetTimeDilation = 0.2f; }
	else { TargetTimeDilation = 1.0f; }

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TargetTimeDilation);
}

void ABasePlayer::ResetAttackMontageIndex(EKnightStancesVariants StanceVariant) {};

void ABasePlayer::ActivateStance() {}

TArray<FStanceStruct> ABasePlayer::PrepareStanceMenu() { return TArray<FStanceStruct>(); }

//Пассивная Регенерация статов
void ABasePlayer::RegenerateHealth()
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthRegenAmount, 0.0f, MaxHealth);
	if (CurrentHealth >= MaxHealth)
	{
		StopHealthRegen();
	}

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void ABasePlayer::RegenerateMana()
{
	CurrentMana = FMath::Clamp(CurrentMana + ManaRegenAmount, 0.0f, MaxMana);
	if (CurrentMana >= MaxMana)
	{
		StopManaRegen();
	}
	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void ABasePlayer::RegenerateShield()
{	
	float MaxStrength = CurrentShield->MaxStrength;
	CurrentShield->CurrentStrength = FMath::Clamp(CurrentShield->CurrentStrength + ShieldRegenAmount, 0.0f, MaxStrength);
	if (CurrentShield->CurrentStrength >= MaxStrength)
	{
		StopShieldRegen();
	}
	OnShieldStrengthChanged.Broadcast(CurrentShield->CurrentStrength, MaxStrength);
}

void ABasePlayer::StartHealthRegenerate()
{
	GetWorldTimerManager().SetTimer(HealthRegenTimerHandle, this, &ABasePlayer::RegenerateHealth, RegenRate, true);
}

void ABasePlayer::StartManaRegenerate()
{
	GetWorldTimerManager().SetTimer(ManaRegenTimerHandle, this, &ABasePlayer::RegenerateMana, RegenRate, true);
}

void ABasePlayer::StartShieldRegenerate()
{
	GetWorldTimerManager().SetTimer(ShieldRegenTimerHandle, this, &ABasePlayer::RegenerateShield, RegenRate, true);
}

void ABasePlayer::StopHealthRegen()
{
	GetWorldTimerManager().ClearTimer(HealthRegenTimerHandle);
}

void ABasePlayer::StopManaRegen()
{
	GetWorldTimerManager().ClearTimer(ManaRegenTimerHandle);
}

void ABasePlayer::StopShieldRegen()
{
	GetWorldTimerManager().ClearTimer(ShieldRegenTimerHandle);
}

//Стойки

void ABasePlayer::OpenStanceMenu(TArray<FStanceStruct> Stances)
{
	if (RadialMenu) return;

	if (RadialMenuClass)
	{
		RadialMenu = CreateWidget<URadialMenuWidget>(GetWorld(), RadialMenuClass);
		if (!RadialMenu) return;

		RadialMenu->AddToViewport();

		RadialMenu->SetupMenu(Stances);
		
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->SetShowMouseCursor(true);
			PC->SetInputMode(FInputModeGameAndUI());

			int32 ViewportSizeX, ViewportSizeY;
			PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

			PC->SetMouseLocation(ViewportSizeX / 2, ViewportSizeY / 2);

		}
	}
};

void ABasePlayer::CloseStanceMenu()
{
	if (!RadialMenu) return;

	RadialMenu->RemoveFromParent(); RadialMenu = nullptr;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}