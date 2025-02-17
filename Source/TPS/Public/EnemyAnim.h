// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFSM.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState animState;

	// 공격 애니메이션 재생 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=FSM)
	bool bAttackPlay = false;

	UFUNCTION()
	void AnimNotify_AttackEnd();
};
