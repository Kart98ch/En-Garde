// Fill out your copyright notice in the Description page of Project Settings.


#include "EG_BasePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EG_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// TODO: print out the mouse coordinates to see its format and edit it and try and find a way to determine the center vector for the screen dynamically if needed.

void AEG_BasePlayerController::MoveCharacter(const FInputActionValue& Value)
{
	const FRotator MovementRotation(0.0f, this->GetControlRotation().Yaw, 0.0f);

	if (Value.GetMagnitude() != 0.f)
	{
		if (AEG_BaseCharacter* PC = Cast<AEG_BaseCharacter>(GetPawn()))
		{
			if (!PC->bAttack)
			{
				if (PC->GetBlock())
				{
					// add forward movement
					GetPawn()->AddMovementInput(MovementRotation.RotateVector(FVector::ForwardVector), Value[1] * BlockMovementScalar);
					// add right movement
					GetPawn()->AddMovementInput(MovementRotation.RotateVector(FVector::RightVector), Value[0] * BlockMovementScalar);
				}
				else
				{
					if (Value[1] < 0)
					{
						// add forward movement
						GetPawn()->AddMovementInput(MovementRotation.RotateVector(FVector::ForwardVector), Value[1] * BackwardsMovementScalar);

						// add right movement
						GetPawn()->AddMovementInput(MovementRotation.RotateVector(FVector::RightVector), Value[0] * BackwardsMovementScalar);

						return;
					}
					else 
					{
						GetPawn()->AddMovementInput(MovementRotation.RotateVector(FVector::ForwardVector), Value[1]);
					}

					// add right movement
					GetPawn()->AddMovementInput(MovementRotation.RotateVector(FVector::RightVector), Value[0] * StrafeMovementScalar);
				}
			}
		}
	}
}

void AEG_BasePlayerController::TurnCamera(const FInputActionValue& Value)
{

	if (Value.GetMagnitude() != 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("TurnCamera")));

		AddYawInput(Value.GetMagnitude() * CameraRotationMultiplier);
	}

}

void AEG_BasePlayerController::BlockTriggered()
{
	if (AEG_BaseCharacter* PC = Cast<AEG_BaseCharacter>(GetPawn()))
	{
		if (!PC->GetAttack())
		{
			if (!PC->GetBlock())
			{
				PC->Server_UpdateBlock(true);
			}
		}
	}
}

void AEG_BasePlayerController::BlockReleased()
{
	if (AEG_BaseCharacter* PC = Cast<AEG_BaseCharacter>(GetPawn()))
	{
		PC->Server_UpdateBlock(false);
	}
}

void AEG_BasePlayerController::Attack()
{
	if (AEG_BaseCharacter* PC = Cast<AEG_BaseCharacter>(GetPawn()))
	{
		PC->AttackEvent();
	}
}

void AEG_BasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);

		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEG_BasePlayerController::MoveCharacter);

			EIC->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &AEG_BasePlayerController::TurnCamera);

			EIC->BindAction(BlockAction, ETriggerEvent::Triggered, this, &AEG_BasePlayerController::BlockTriggered);

			EIC->BindAction(BlockAction, ETriggerEvent::Completed, this, &AEG_BasePlayerController::BlockReleased);

			EIC->BindAction(AttackAction, ETriggerEvent::Started, this, &AEG_BasePlayerController::Attack);
		}
	}

}

void AEG_BasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float mouseX;
	float mouseY;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouseX, mouseY);

	int32 viewX;
	int32 viewY;
	GetViewportSize(viewX, viewY);

	mouseX -= viewX / 2;
	mouseY -= viewY / 2;
	mouseY *= -1;

	if (AEG_BaseCharacter* PC = Cast<AEG_BaseCharacter>(GetPawn()))
	{
		PC->SetCharacterState(mouseX, mouseY);
	}

}

void AEG_BasePlayerController::BeginPlay()
{
	// TODO: Find solution for showing the mouse cursor while playing
	//		 Having it show seems to cause problems with inputs when playing
	bShowMouseCursor = false;
}
