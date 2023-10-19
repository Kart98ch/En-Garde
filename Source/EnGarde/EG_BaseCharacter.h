// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EG_BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class ESwingState : uint8
{
	ES_Default		UMETA(DisplayName = "Default"),
	ES_BottomLeft	UMETA(DisplayName = "BottomLeft"),
	ES_Left			UMETA(DisplayName = "Left"),
	ES_TopLeft		UMETA(DisplayName = "TopLeft"),
	ES_Top			UMETA(DisplayName = "Top"),
	ES_TopRight		UMETA(DisplayName = "TopRight"),
	ES_Right		UMETA(DisplayName = "Right"),
	ES_BottomRight	UMETA(DisplayName = "BottomRight")
};

UCLASS()
class ENGARDE_API AEG_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Functions
public:
	// Sets default values for this character's properties
	AEG_BaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Set Character block and attack states
	void SetCharacterState(float X, float Y);

	// Attack event
	UFUNCTION(BlueprintImplementableEvent)
		bool AttackEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayerDeath();

#pragma region RPCs
	// Update block
	UFUNCTION(Server, Reliable)
		void Server_UpdateBlock(bool State);
	UFUNCTION(NetMulticast, Reliable)
		void Multicast_SetBlock(bool State);

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Server_UpdateAttack(bool State);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void Multicast_SetAttack(bool State);

	// Update swing state
	UFUNCTION(Server, Reliable)
		void Server_UpdateSwingState(ESwingState State);
	UFUNCTION(NetMulticast, Reliable)
		void Multicast_SetSwingState(ESwingState State);
#pragma endregion

#pragma region Getters & Setters
	// Swing State
	ESwingState GetSwingState() const { return SwingState; }
	void SetSwingState(ESwingState NewState) { CurrentSwingState = NewState; }
	
	// Block
	bool GetBlock() const { return bBlock; }
	void SetBlock(bool NewBool) { bBlock = NewBool; }

	// Attack
	bool GetAttack() const { return bAttack; }
	void SetAttack(bool NewBool) { bAttack = NewBool; }

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckForStateChange();
	

#pragma region Variables
public:
	friend class AEG_BasePlayerController;

protected:
	// Swing states
	UPROPERTY (BlueprintReadWrite, Replicated)
		ESwingState SwingState = ESwingState::ES_Default;

	UPROPERTY(BlueprintReadWrite, Replicated)
		ESwingState CurrentSwingState = ESwingState::ES_Default;

	UPROPERTY(BlueprintReadWrite, Replicated)
		ESwingState PreviousSwingState = ESwingState::ES_Default;

	UPROPERTY(BlueprintReadWrite, Replicated)
		ESwingState AttackSwingState = ESwingState::ES_Default;

	// Attack and Block Variables
	UPROPERTY(BlueprintReadWrite, Replicated)
		bool bBlock = false;

	UPROPERTY(BlueprintReadWrite, Replicated)
		bool bAttack = false;

	// Actor Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UHealthAndStaminaComponent* HSComp;

#pragma endregion

};
