  // Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Ability\Movement\DodgeAbility.h"
#include "GameFramework/PlayerController.h"
#include "Public\Character\BaseCharacter.h"
#include "Public\Character\Player\BasePlayer.h"
#include "Animation/AnimMontage.h"


/*bool UDodgeAbility::ActivateAbility()
{
	if (!Super::ActivateAbility()) return false;

	if (CharacterOwner && DodgeMontage)
	{
		ABasePlayer* PlayerOwner;
		if (!(PlayerOwner = Cast<ABasePlayer>(CharacterOwner))) return false;

		FVector DodgeDirection = FVector::ZeroVector;

		const FRotator Rotation = CharacterOwner->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		float ForwardInput = CharacterOwner->GetInputAxisValue("MoveForward");
		float RightInput = CharacterOwner->GetInputAxisValue("MoveRight");

		PlayerOwner->DodgeForwardValue = ForwardInput;
		PlayerOwner->DodgeRightValue = RightInput;

		DodgeDirection = (Forward * ForwardInput) + (Right * RightInput);
			
		DodgeDirection.Normalize();

		CharacterOwner->PlayAnimMontage(DodgeMontage);
		return true;

	}
	return false;
}*/

bool UDodgeAbility::ActivateAbility()
{
	if (!Super::ActivateAbility()) return false;

	if (CharacterOwner)
	{
		ABasePlayer* PlayerOwner = Cast<ABasePlayer>(CharacterOwner);
		
		if (PlayerOwner)
		{
			float ForwardInput = PlayerOwner->DodgeForwardValue;
			float RightInput = PlayerOwner->DodgeRightValue;

			UAnimMontage* SelectedMontage = nullptr;

			// Логика выбора монтажа (как простой роутинг)
			if (ForwardInput > 0.5f)       SelectedMontage = ForwardMontage;
			else if (ForwardInput < -0.5f) SelectedMontage = BackwardMontage;
			else if (RightInput > 0.5f)    SelectedMontage = RightMontage;
			else if (RightInput < -0.5f)   SelectedMontage = LeftMontage;
			else                           SelectedMontage = BackwardMontage; // Дефолт (назад)

			if (SelectedMontage)
			{
				float Duration = CharacterOwner->PlayAnimMontage(SelectedMontage);

				FTimerHandle TimerHandle;
				CharacterOwner->GetWorldTimerManager().SetTimer(TimerHandle, this, &UDodgeAbility::DeactivateAbility, Duration, false);
				return true;
			}
		}
	}
	DeactivateAbility();
	return false;
}