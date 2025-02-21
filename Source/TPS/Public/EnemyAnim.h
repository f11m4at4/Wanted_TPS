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

	// 피격 애니메이션 재생 함수
	UFUNCTION(BlueprintImplementableEvent, Category=FSMEvent)
	void PlayDamageAnimation(FName sectionName);

	UFUNCTION()
	void AnimNotify_DieEnd();

	// 다이 상태에서 아래로 내려가기 시작할지 여부
	bool bDieMovingStart = false;

	// 애니메이션이 플레이어에 피격 이벤트를 호출할 타이밍
	UFUNCTION()
	void AnimNotify_AttackStart();
};
