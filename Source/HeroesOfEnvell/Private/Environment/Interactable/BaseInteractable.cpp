// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Environment/Interactable/BaseInteractable.h"
#include "Public\Interfaces\InteractableInterface.h"
#include "Public/InterfaceVisual/Interact/InteractWidget.h"
#include "Components/ArrowComponent.h"

ABaseInteractable::ABaseInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SceneRoot);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComponent->SetupAttachment(SceneRoot); //Привязка к корню
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); //Всегда смотрит на камеру и имеет фикс размер
	InteractionWidgetComponent->SetDrawAtDesiredSize(true); //Сам определяет свой размер 
	InteractionWidgetComponent->SetVisibility(false); //Изначально не виден

	InteractableCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableCollision"));
	InteractableCollision->SetupAttachment(GetRootComponent());

	// Устанавливаем тип генерации событий. 
	// QueryOnly означает, что компонент используется только для поиска (лучи, оверлапы), 
	// но не участвует в физических столкновениях (Physics).
	InteractableCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Устанавливаем профиль или настраиваем каналы вручную.
	// Игнорируем всё по умолчанию.
	InteractableCollision->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Включаем Overlap только для тех, кто может взаимодействовать.
	// Обычно это канал Pawn (игрок) или Camera (если взаимодействие идет по центру экрана).
	InteractableCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractableCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	InteractableCollision->SetCollisionObjectType(ECC_WorldDynamic);

	// Обязательно включаем генерацию событий оверлапа
	InteractableCollision->SetGenerateOverlapEvents(true);
}

void ABaseInteractable::OnInteract(AActor* Interactor)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("OnInteract"));
	}
}

void ABaseInteractable::ShowTooltip(bool bShow)
{
	if (!InteractionWidgetComponent) return;

	if (bShow)
	{
		UInteractWidget* WidgetInstance = Cast<UInteractWidget>(InteractionWidgetComponent->GetUserWidgetObject());
		if (WidgetInstance)
		{
			WidgetInstance->SetActionText(InteractionActionName);
		}
	}

	InteractionWidgetComponent->SetVisibility(bShow);
}