// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthAndStaminaComponent.generated.h"

class AEG_BaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENGARDE_API UHealthAndStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthAndStaminaComponent();

#pragma region FUNCTIONS
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool CheckDeath() const;

	UFUNCTION(BlueprintCallable)
		float GetHealth() { return Health; }

	UFUNCTION(BlueprintCallable)
		float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
		float GetStamina() { return Stamina; }

	UFUNCTION(BlueprintCallable)
		float GetMaxStamina() { return MaxStamina; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void DecreaseHealth(float Input);

	UFUNCTION(BlueprintCallable)
		void DecreaseStamina(float Input);

	UFUNCTION(BlueprintCallable)
		void IncreaseHealth(float Input);

	UFUNCTION(BlueprintCallable)
		void IncreaseStamina(float Input);

	//RPCs
	// TODO: need to create these RPCs and make corresponding Multicast RPCs
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Server_BlockSuccess();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void Multicast_BlockSuccess();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Server_BlockDecrement();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void Multicast_BlockDecrement();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Server_WrongBlockDecrement();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void Multicast_WrongBlockDecrement();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Server_DirectHit();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void Multicast_DirectHit();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void Multicast_PlayerDeath();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Server_SwingDecrement();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void Multicast_SwingDecrement();

#pragma endregion
		
#pragma region VARIABLES
public:


protected:
	// Core Health and Stamina values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float Health = MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float Stamina = MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxStamina = 100.f;


	// Passive variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StaminaRegenRate = 5.f;


	// Swing and Block Stamina affecting values
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlockSuccessStaminaRefundAmount = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ImperfectBlockStaminaDecreaseAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WrongBlockStaminaDecreaseAmount = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WrongBlockHealthDecreaseAmount = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DirectHitHealthDecreaseAmount = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SwingStaminaDecreaseAmount = 15.f;

	AEG_BaseCharacter* OwningPlayer;

#pragma endregion

};
