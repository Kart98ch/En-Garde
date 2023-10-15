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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void DecreaseHealth(float Input) { Health -= Input; }

	UFUNCTION(BlueprintCallable)
		void DecreaseStamina(float Input) { Stamina -= Input; }

	UFUNCTION(BlueprintCallable)
		void IncreaseHealth(float Input) { Health += Input; }

	UFUNCTION(BlueprintCallable)
		void IncreaseStamina(float Input) { Stamina += Input; }

	//RPCs
	// TODO: need to create these RPCs and make corresponding Multicast RPCs
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void BlockSuccess();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void BlockDecrement();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void SwingDecrement();

#pragma endregion
		
#pragma region VARIABLES
public:


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Stamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StaminaRegenRate = 5.f;

	AEG_BaseCharacter* OwningPlayer;

#pragma endregion

};
