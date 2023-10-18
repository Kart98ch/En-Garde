// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAndStaminaComponent.h"
#include "Net/UnrealNetwork.h"
#include "EG_BaseCharacter.h"

// Sets default values for this component's properties
UHealthAndStaminaComponent::UHealthAndStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

}

// Replication Variables
void UHealthAndStaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthAndStaminaComponent, Health);
	DOREPLIFETIME(UHealthAndStaminaComponent, Stamina);

}

// Called when the game starts
void UHealthAndStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayer = Cast<AEG_BaseCharacter>(GetOwner());

	Health = MaxHealth;
	Stamina = MaxStamina;
	
}

// Health and Stamina manipulation functions
void UHealthAndStaminaComponent::DecreaseHealth(float Input)
{
	if (Health - Input > 0)
	{
		Health -= Input;
	}
	else
	{
		Health = 0;
	}
}

void UHealthAndStaminaComponent::DecreaseStamina(float Input)
{
	if (Stamina - Input > 0)
	{
		Stamina -= Input;
	}
	else
	{
		Stamina = 0;
	}
}

void UHealthAndStaminaComponent::IncreaseHealth(float Input)
{
	if (Health + Input < MaxHealth)
	{
		Health += Input;
	}
	else
	{
		Health = MaxHealth;
	}
}

void UHealthAndStaminaComponent::IncreaseStamina(float Input)
{
	if (Stamina + Input < MaxStamina)
	{
		Stamina += Input;
	}
	else
	{
		Stamina = MaxStamina;
	}
}

// Multicast RPCs
void UHealthAndStaminaComponent::Multicast_BlockSuccess_Implementation()
{
	IncreaseStamina(BlockSuccessStaminaRefundAmount);
}

void UHealthAndStaminaComponent::Multicast_BlockDecrement_Implementation()
{
	DecreaseStamina(ImperfectBlockStaminaDecreaseAmount);
}

void UHealthAndStaminaComponent::Multicast_WrongBlockDecrement_Implementation()
{
	DecreaseStamina(WrongBlockStaminaDecreaseAmount);
	DecreaseHealth(WrongBlockHealthDecreaseAmount);
}

void UHealthAndStaminaComponent::Multicast_DirectHit_Implementation()
{
	DecreaseHealth(DirectHitHealthDecreaseAmount);
}

void UHealthAndStaminaComponent::Multicast_SwingDecrement_Implementation()
{
	DecreaseStamina(SwingStaminaDecreaseAmount);
}

// Server RPCs
void UHealthAndStaminaComponent::Server_BlockSuccess_Implementation()
{
	Multicast_BlockSuccess();
}

void UHealthAndStaminaComponent::Server_BlockDecrement_Implementation()
{
	Multicast_BlockDecrement();
}

void UHealthAndStaminaComponent::Server_WrongBlockDecrement_Implementation()
{
	Multicast_WrongBlockDecrement();
}

void UHealthAndStaminaComponent::Server_DirectHit_Implementation()
{
	Multicast_DirectHit();
}

void UHealthAndStaminaComponent::Server_SwingDecrement_Implementation()
{
	Multicast_SwingDecrement();
}


// Called every frame
void UHealthAndStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Stamina < MaxStamina)
	{
		if (OwningPlayer->GetBlock())
		{
			Stamina += StaminaRegenRate * DeltaTime;

			if (Stamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
		}
	}

	if (Health <= 0)
	{
		// Kill player here - call RPC for that.
	}
}


