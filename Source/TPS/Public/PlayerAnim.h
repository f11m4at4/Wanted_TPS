// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	float speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	float direction = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	bool bIsInAir = false;
	

	// 매 프레임 갱신되는 함수
	virtual  void NativeUpdateAnimation(float DeltaSeconds) override;
};
