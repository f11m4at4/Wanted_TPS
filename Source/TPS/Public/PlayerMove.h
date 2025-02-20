// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPS_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerMove();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

	virtual  void SetupInputBinding(class UEnhancedInputComponent* playerInput) override;
};
