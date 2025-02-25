// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSPlayer.h"
#include "Components/ActorComponent.h"
#include "PlayerBaseComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPS_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerBaseComponent();

protected:
	virtual  void InitializeComponent() override;
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// 소유주 액터
	UPROPERTY()
	ATPSPlayer* me;
	// 이동컴포넌트
	UPROPERTY()
	UCharacterMovementComponent* moveComp;

public:
	// 사용자 입력 매핑 함수
	virtual void SetupInputBinding(
		class UEnhancedInputComponent* pi) {};
};
