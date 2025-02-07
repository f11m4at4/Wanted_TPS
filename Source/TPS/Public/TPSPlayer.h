// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Settings)
	float speed = 500;
	UPROPERTY()
	FVector direction;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* imc_tps;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_move;

	// 이동처리 함수
	void Move(const struct FInputActionValue& inputValue);

	// InputAction 회전
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_turn;
	void Turn(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Lookup;
	void Lookup(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_jump;
	void InputJump(const struct FInputActionValue& inputValue);
};
