// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"


UCLASS(ClassGroup=(PlayerComp), meta=(BlueprintSpawnableComponent))
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

	virtual  void SetupInputBinding(class UEnhancedInputComponent* pi) override;

	// InputAction 회전
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_turn;
	void Turn(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Lookup;
	void Lookup(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category=Input)
    class UInputAction* ia_move;
	UPROPERTY()
	FVector direction;
	
	// 이동처리 함수
	void Move(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_jump;
	void InputJump(const struct FInputActionValue& inputValue);

	// 달리기 입력
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_run;
	void InputRun(const struct FInputActionValue& inputValue);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Settings)
	float walkSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Settings)
	float runSpeed = 600;
	

};
