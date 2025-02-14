// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "TPS.h"


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 현재 상태값 출력
	FString stateStr = UEnum::GetValueAsString(mState);
	PRINTLOGTOSCREEN(TEXT("%s"), *stateStr);
	
	// 목차 만들기
	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

// 일정시간이 지나면 상태를 Move 로 전환하고 싶다.
// 필요속성 : 일정시간, 경과시간
void UEnemyFSM::IdleState()
{
	// 1. 시간이 흘러야한다.
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 경과시간이 대기시간을 지났다면
	if (currentTime > idleDelayTime)
	{
		// 3. 상태를 Move 로 바꾼다.
		mState = EEnemyState::Move;
		currentTime = 0;
	}
}

void UEnemyFSM::MoveState()
{
}

void UEnemyFSM::AttackState()
{
}

void UEnemyFSM::DamageState()
{
}

void UEnemyFSM::DieState()
{
}
