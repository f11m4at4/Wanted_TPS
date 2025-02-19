// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName="IdleState"),
	Move,
	Attack,
	Damage,
	Die
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

	// 현재 enemy 의 상태 변수 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;
	// 필요속성 : Idle 대기시간, 경과시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float idleDelayTime = 2;
	float currentTime = 0;

	// 필요속성 : 타겟, 이동속도, 부모
	UPROPERTY()
	class ATPSPlayer* target;
	UPROPERTY()
	class AEnemy* me;

	// 필요속성 : 공격범위
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackRange = 200;
	// 필요속성 : 공격대기시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 3;
	
	// 디버그모드 활성화 여부
	UPROPERTY(EditAnywhere, Category=DEBUG)
	bool bDebugPlay = false;
	
	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();

	// 피격 당할때 호출될 이벤트 함수
	void OnDamageProcess();

	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM)
	int32 hp = 3;

	// 필요속성 : 피격대기시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM)
	float damageDelayTime = 2;

	// 필요속성 : 이동속도
	UPROPERTY(EditAnywhere, Category=FSM)
	float dieMoveSpeed = 50.0f;

	UPROPERTY()
	class UEnemyAnim* anim;

	// Enemy 를 소유하고 있는 AIController
	UPROPERTY()
	class AAIController* ai;

	// 길찾기 수행시 랜덤 위치 기억 변수
	FVector randomPos;
	// 랜덤 위치 가져오는 함수
	/**
	 * 
	 * @param centerLoc 기준점위치
	 * @param raidus 검색범위
	 * @param dest 검색된 위치
	 * @return 
	 */
	bool GetRandomPositionInNavMesh(FVector centerLoc, float radius, FVector& dest);
	
};
