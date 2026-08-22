#include "Public\Weapon\BaseWeapon.h"
#include "GameFramework/Character.h"
#include "Public/Character/BaseCharacter.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	//Скелет оружия
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//мой код

void ABaseWeapon::AttachToCharacter(ACharacter* TargetCharacter, bool bEquip)
{
	if (!TargetCharacter) return;

	FName TargetSocket = bEquip ? ActiveWeaponSocket : RestingWeaponSocket;

	FAttachmentTransformRules Rules(
		EAttachmentRule::SnapToTarget, // Хватаем точно в сокет
		EAttachmentRule::SnapToTarget, // Вращаем точно как сокет
		EAttachmentRule::KeepWorld,    // Масштаб лучше оставить как есть
		true                           // Сварка тел (для физики)
	);

	AttachToComponent(TargetCharacter->GetMesh(), Rules, TargetSocket);
}

void ABaseWeapon::ExecuteAttack(ACharacter* TargetCharacter)
{
	if (!TargetCharacter) return;

	float Duration = TargetCharacter->PlayAnimMontage(GetAttackMontage());
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseWeapon::OwnerArmedState, Duration, false);
}

void ABaseWeapon::OwnerArmedState()
{
	AActor* ActorOwner = GetOwner();
	ABaseCharacter* CharacterOwner = Cast<ABaseCharacter>(ActorOwner);

	if (!ActorOwner || !CharacterOwner) return;

	CharacterOwner->WeaponState = EWeaponState::Armed;
} 

void ABaseWeapon::PlayWeaponMontage(UAnimMontage* Montage, ACharacter* TargetCharacter)
{
	if (TargetCharacter && Montage) 
	{
		TargetCharacter->PlayAnimMontage(Montage);
	}
}

UAnimMontage* ABaseWeapon::GetAttackMontage() { return nullptr; }