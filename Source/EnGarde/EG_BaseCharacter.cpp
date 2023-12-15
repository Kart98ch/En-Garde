// Fill out your copyright notice in the Description page of Project Settings.


#include "EG_BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "HealthAndStaminaComponent.h"

// Sets default values
AEG_BaseCharacter::AEG_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	HSComp = CreateDefaultSubobject<UHealthAndStaminaComponent>(TEXT("Health & Stamina Component"));

}

// Set replication of variables here
void AEG_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AEG_BaseCharacter, SwingState, COND_None);

	DOREPLIFETIME_CONDITION(AEG_BaseCharacter, AttackSwingState, COND_None);

	DOREPLIFETIME_CONDITION(AEG_BaseCharacter, bBlock, COND_None);

	DOREPLIFETIME_CONDITION(AEG_BaseCharacter, bAttack, COND_None);

	DOREPLIFETIME_CONDITION(AEG_BaseCharacter, CurrentSwingState, COND_None);

	DOREPLIFETIME_CONDITION(AEG_BaseCharacter, PreviousSwingState, COND_None);

}

// Called when the game starts or when spawned
void AEG_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEG_BaseCharacter::CheckForStateChange()
{
	if (PreviousSwingState != CurrentSwingState)
	{
		PreviousSwingState = CurrentSwingState;
		Server_UpdateSwingState(CurrentSwingState);
	}
}

void AEG_BaseCharacter::SetCharacterState(float X, float Y)
{
	if (X && Y)
	{
		float Alpha = acos(FMath::Abs(X) / FMath::Sqrt(X * X + Y * Y));

		if (Y >= 0)
		{
			// logic for upper states
			if (X >= 0)
			{
				// upper right quadrant
				if (Alpha < PI / 8)
				{
					SetSwingState(ESwingState::ES_Right);
				}
				else if (PI / 8 <= Alpha && Alpha < 3 * PI / 8)
				{
					SetSwingState(ESwingState::ES_TopRight);
				}
				else
				{
					SetSwingState(ESwingState::ES_Top);
				}
			}
			else
			{
				// upper left quadrant
				if (Alpha < PI / 8)
				{
					SetSwingState(ESwingState::ES_Left);
				}
				else if (PI / 8 <= Alpha && Alpha < 3 * PI / 8)
				{
					SetSwingState(ESwingState::ES_TopLeft);
				}
				else
				{
					SetSwingState(ESwingState::ES_Top);
				}
			}
		}
		else
		{
			// logic for lower states
			if (X >= 0)
			{
				// lower right quadrant
				if (Alpha < PI / 8)
				{
					SetSwingState(ESwingState::ES_Right);
				}
				else if (PI / 8 <= Alpha && Alpha < 3 * PI / 8)
				{
					SetSwingState(ESwingState::ES_BottomRight);
				}
				else
				{
					SetSwingState(ESwingState::ES_Default);
				}
			}
			else
			{
				// lower left quadrant
				if (Alpha < PI / 8)
				{
					SetSwingState(ESwingState::ES_Left);
				}
				else if (PI / 8 <= Alpha && Alpha < 3 * PI / 8)
				{
					SetSwingState(ESwingState::ES_BottomLeft);
				}
				else
				{
					SetSwingState(ESwingState::ES_Default);
				}
			}
		}
	}

	if (IsLocallyControlled())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Player State: %d"), SwingState));
	}

	// Update state change
	CheckForStateChange();

}

void AEG_BaseCharacter::Multicast_SetAttack_Implementation(bool State)
{
	bAttack = State;

	AttackSwingState = SwingState;

	if (State)
	{
		Server_UpdateBlock(false);
	}
}

void AEG_BaseCharacter::Server_UpdateAttack_Implementation(bool State)
{
	Multicast_SetAttack(State);
}

void AEG_BaseCharacter::Multicast_SetSwingState_Implementation(ESwingState State)
{
	SwingState = State;
}

void AEG_BaseCharacter::Server_UpdateSwingState_Implementation(ESwingState State)
{
	Multicast_SetSwingState(State);
}

void AEG_BaseCharacter::Multicast_SetBlock_Implementation(bool State)
{
	if (State)
	{
		if (!bAttack)
		{
			SetBlock(State);
		}
	}
	else
	{
		SetBlock(State);
	}
}

void AEG_BaseCharacter::Server_UpdateBlock_Implementation(bool State)
{
	Multicast_SetBlock(State);
}

// Called every frame
void AEG_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEG_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

