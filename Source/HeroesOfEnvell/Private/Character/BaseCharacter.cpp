#include "Public\Character\BaseCharacter.h"
#include "CoreMinimal.h"
#include "Public\Service\ProjectTypes.h"
#include "Public\Weapon\BaseWeapon.h"
#include "Public\Character\Player\BasePlayer.h"
#include <random>
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	GetCharacterMovement()->MaxWalkSpeed = RunMovespeed;
	SpawnWeapon();
	DefaultCharacterStats.DefaultDamage = CurrentWeapon->Damage;

	InitAbilities();
}


//Передвижение

void ABaseCharacter::Internal_MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::Internal_MoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::Internal_Look(FVector2D Value)
{
	if (Controller)
	{
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
}

//Боевка

	//Отвечают за какие либо механики

void ABaseCharacter::SpawnWeapon()
{
	if (WeaponClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FVector Location = GetActorLocation();
			FRotator Rotation = GetActorRotation();

			CurrentWeapon = World->SpawnActor<ABaseWeapon>(WeaponClass, Location, Rotation, SpawnParams);

			if (CurrentWeapon)
			{
				FAttachmentTransformRules Rules(
					EAttachmentRule::SnapToTarget, // Хватаем точно в сокет
					EAttachmentRule::SnapToTarget, // Вращаем точно как сокет
					EAttachmentRule::KeepWorld,    // Масштаб лучше оставить как есть
					true                           // Сварка тел (для физики)
				);


				CurrentWeapon->AttachToComponent(GetMesh(), Rules, CurrentWeapon->RestingWeaponSocket);
			}
		}
	}
}

void ABaseCharacter::SpawnShield()
{
	if (ShieldClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;

			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FVector Location = GetActorLocation();
			FRotator Rotation = GetActorRotation();

			CurrentShield = World->SpawnActor<ABaseShield>(ShieldClass, Location, Rotation, SpawnParams);

			CurrentShield->SetShieldVisible(false);

			if (CurrentShield)
			{
				FAttachmentTransformRules Rules(
					EAttachmentRule::SnapToTarget, // Хватаем точно в сокет
					EAttachmentRule::SnapToTarget, // Вращаем точно как сокет
					EAttachmentRule::KeepWorld,    // Масштаб лучше оставить как есть
					true                           // Сварка тел (для физики)
				);


				CurrentShield->AttachToComponent(GetMesh(), Rules, CurrentShield->ShieldSocket);
			}
		}
	}
}

void ABaseCharacter::ToggleWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("toggle weapon"));
	if (WeaponState == EWeaponState::Equipping || WeaponState == EWeaponState::Sheathing) { return; } 
	// Не даем доставать\убирать оружие когда соответствующая анимация уже проигрывается

	if (!CurrentWeapon) return;

	if (CurrentWeapon->EquipMontage && WeaponState == EWeaponState::Unarmed )
	{
		WeaponState = EWeaponState::Equipping;
		CurrentWeapon->PlayWeaponMontage(CurrentWeapon->EquipMontage, this);
	}
	else if (CurrentWeapon->SheathingMontage && WeaponState == EWeaponState::Armed)
	{
		WeaponState = EWeaponState::Sheathing;
		CurrentWeapon->PlayWeaponMontage(CurrentWeapon->SheathingMontage, this);
	}
	
}

void ABaseCharacter::HandleWeaponAttach(bool bEquip)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("HandleAttach Called!"));
	if (!CurrentWeapon) { return; }

	CurrentWeapon->AttachToCharacter(this,bEquip);

	if (bEquip)
	{
		WeaponAnimationState = CurrentWeapon->StanceToApply;
		WeaponState = EWeaponState::Armed;
	}
	else
	{
		WeaponAnimationState = EWeaponAnimationStance::Unarmed;
		WeaponState = EWeaponState::Unarmed;
	}
}

void ABaseCharacter::Attack()
{
	if (CurrentWeapon && WeaponState == EWeaponState::Armed)
	{
		WeaponState = EWeaponState::Attacking;
		CurrentWeapon->ExecuteAttack(this);
	} 
}

void ABaseCharacter::OnDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ondeath in the basecharacter"));
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	if (GetMesh())
	{
		GetMesh()->SetSimulatePhysics(true);
	}
}

void ABaseCharacter::ProcessDamage_Implementation(float Amount, AActor* DamageCauser)
{
	if (Amount <= 0 || !DamageCauser || CharacterStats.bIsInvulnerable) return;
	bIsAbilityActive = false;

	float AmountDamage = CountDamage(Amount, DamageCauser);
	if (AmountDamage <= 0) { return; }
	
	LastReceivedDamage = AmountDamage;

	if (bIsShieldBlocking && CurrentShield && CurrentShield->CanBlockAttack(DamageCauser->GetActorLocation()))
	{
		if (ShieldHitReactionMontage) PlayAnimMontage(ShieldHitReactionMontage);

		float BlockingAmount = FMath::Min(AmountDamage, CurrentShield->CurrentStrength);
		CurrentShield->CurrentStrength -= BlockingAmount;
		AmountDamage -= BlockingAmount;

		if (CurrentShield->CurrentStrength <= 0) { ShieldAbility->DeactivateAbility(); }
	}

	if (AmountDamage > 0)
	{
		if (WeaponState == EWeaponState::Attacking) WeaponState = EWeaponState::Armed;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("damage>0"));
		CurrentHealth -= AmountDamage;
		if (CurrentHealth <= 0) { OnDeath_Implementation(); }
		else { if (HitReactionMontage) PlayAnimMontage(HitReactionMontage); }
	}
}

float ABaseCharacter::CountDamage(float Amount, AActor* DamageCauser)
{
	float FinalDamage = Amount * CharacterStats.DamageMultiplier;

	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<int32> RandomResult(0, 100);

	ABaseCharacter* EnemyCauser = Cast<ABaseCharacter>(DamageCauser); 

	if (!EnemyCauser)
	{
		return FinalDamage;
	}

	float MissChance = CharacterStats.MissChance + DefaultCharacterStats.DefaultMissChance;
	if (MissChance >= RandomResult(gen))
	{
		if (!EnemyCauser->CharacterStats.bIsIgnoreMiss)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Мисс //ABaseCharacter.CountDamage()"));
			return 0;
		}
	}

	float CritChance = EnemyCauser->CharacterStats.CritChance + EnemyCauser->DefaultCharacterStats.DefaultCritChance;
	if (CritChance >= RandomResult(gen))
	{
		float CritDamage = EnemyCauser->CharacterStats.CritDamage + EnemyCauser->DefaultCharacterStats.DefaultCritDamage;
		FinalDamage *= (1.0f + ( CritDamage/ 100.0f));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Крит //ABaseCharacter.CountDamage()"));
	}

	float CharacterArmor = DefaultCharacterStats.DefaultArmor * CharacterStats.ArmorMultiplier;
	float EffectiveArmor = (CharacterArmor * (100 - EnemyCauser->CharacterStats.ArmorIgnore)) / 100.0f;
	EffectiveArmor = FMath::Max(0.f, EffectiveArmor);

	FinalDamage = FinalDamage * (ArmorConstant / (ArmorConstant + EffectiveArmor));
	
	if (FinalDamage < 1) { FinalDamage = 1;}

	return FinalDamage;
}


//Вспомогательные

void ABaseCharacter::StartAttackState()
{
	WeaponState = EWeaponState::Attacking;
}

void ABaseCharacter::EndAttackState()
{
	WeaponState = EWeaponState::Armed;
}

ABaseWeapon* ABaseCharacter::GetCurrentWeapon() 
{
	if (CurrentWeapon)
	{
		return CurrentWeapon;
	}
	return nullptr;
}

//Способности
void ABaseCharacter::InitAbilities()
{
}