// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Environment/Interactable/Actors/Door.h"

void ADoor::OnInteract(AActor* Interactor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("OnInteract from Door"));
}