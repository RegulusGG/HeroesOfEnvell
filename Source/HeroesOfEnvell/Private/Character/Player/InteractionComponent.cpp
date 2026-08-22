// Fill out your copyright notice in the Description page of Project Settings.

#include "Public\Character\Player\InteractionComponent.h"
#include "Public\Interfaces\InteractableInterface.h"
#include "Public/Environment/Interactable/BaseInteractable.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    InteractionSphere_New = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere_New"));
    InteractionSphere_New->SetupAttachment(GetAttachParent());

    InteractionSphere_New->InitSphereRadius(250.0f);

    InteractionSphere_New->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionSphere_New->SetGenerateOverlapEvents(true);

    InteractionSphere_New->SetCollisionResponseToAllChannels(ECR_Ignore);

    InteractionSphere_New->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    InteractionSphere_New->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

    InteractionSphere_New->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

    if (InteractionSphere_New)
    {
        if (!InteractionSphere_New->IsRegistered())
        {
            InteractionSphere_New->RegisterComponent();
        }

        InteractionSphere_New->SetRelativeLocation(FVector::ZeroVector);
        InteractionSphere_New->UpdateOverlaps();

        InteractionSphere_New->SetHiddenInGame(false);
        InteractionSphere_New->SetVisibility(true);

        InteractionSphere_New->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnOverlapBegin);
        InteractionSphere_New->OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnOverlapEnd);
    } 

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Interaction, this, &UInteractionComponent::UpdateBestTarget, 0.1f, true);
}



void UInteractionComponent::UpdateBestTarget() {

    AActor* NewBestActor = nullptr;

    float BestScore = -1.f;


    FVector CameraLoc = GetOwner()->GetActorLocation();

    FVector ForwardDir = GetOwner()->GetActorForwardVector();


    for (AActor* Candidate : OverlappingActors)

    {

        if (!Candidate || !Candidate->Implements<UInteractable>())

        {

            continue;

        }


        FVector DirToTarget = (Candidate->GetActorLocation() - CameraLoc).GetSafeNormal();

        float CurrentDot = FVector::DotProduct(ForwardDir, DirToTarget);


        if (CurrentDot > 0.7f)

        {

            if (CurrentDot > BestScore)

            {


                NewBestActor = Candidate;

                BestScore = CurrentDot;

            }

        }

    }

    if (LastBestTarget != NewBestActor)

    {

        if (IsValid(LastBestTarget))

        {

            if (ABaseInteractable* OldTarget = Cast<ABaseInteractable>(LastBestTarget))

            {

                OldTarget->ShowTooltip(false);

            }

        }

    }


    if (IsValid(NewBestActor))

    {

        if (ABaseInteractable* CurrentTarget = Cast<ABaseInteractable>(NewBestActor))

        {

            CurrentTarget->ShowTooltip(true);

        }

    }


    LastBestTarget = NewBestActor;
}

void UInteractionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("OnOVerlapBegin"));
	if (OtherActor && OtherActor->Implements<UInteractable>())
	{
		OverlappingActors.AddUnique(OtherActor);
	}
}

void UInteractionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingActors.Remove(OtherActor);
}