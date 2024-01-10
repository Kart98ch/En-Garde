// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "EG_BasePlayerController.generated.h"

UCLASS()
class ENGARDE_API AEG_BasePlayerController : public APlayerController
{
	GENERATED_BODY()
	

#pragma region INPUTS
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input|Input Action")
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input|Input Action")
		UInputAction* RotateCameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input|Input Action")
		UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input|Input Action")
		UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input|Mapping Context")
		UInputMappingContext* InputMappingContext;

	void MoveCharacter(const FInputActionValue& Value);
	void TurnCamera(const FInputActionValue& Value);
	void BlockTriggered();
	void BlockReleased();

	UFUNCTION()
	void Attack();

public:
	virtual void SetupInputComponent() override;


#pragma endregion

#pragma region Variables
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float CameraRotationMultiplier =  0.90f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float BlockMovementScalar = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float StrafeMovementScalar = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float BackwardsMovementScalar = 0.4f;

#pragma endregion


#pragma region Functions
protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
		void Server_UpdateMovement(FVector WorldDirection, float ScaleValue);
	UFUNCTION(NetMulticast, Reliable)
		void Multicast_UpdateMovement(FVector WorldDirection, float ScaleValue);
#pragma endregion
};
