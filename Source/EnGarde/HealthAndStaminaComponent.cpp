// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAndStaminaComponent.h"
#include "EG_BaseCharacter.h"

// Sets default values for this component's properties
UHealthAndStaminaComponent::UHealthAndStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthAndStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayer = Cast<AEG_BaseCharacter>(GetOwner());
	
}

void UHealthAndStaminaComponent::BlockSuccess_Implementation()
{
}

void UHealthAndStaminaComponent::BlockDecrement_Implementation()
{
}

void UHealthAndStaminaComponent::SwingDecrement_Implementation()
{
}


// Called every frame
void UHealthAndStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwningPlayer->GetBlock())
	{
		Stamina += StaminaRegenRate * DeltaTime;
	}
}

