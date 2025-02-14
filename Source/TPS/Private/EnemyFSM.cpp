// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "Enemy.h"
#include "TPS.h"
#include "TPSPlayer.h"
#include "Kismet/GameplayStatics.h"


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

	me = Cast<AEnemy>(GetOwner());
	// target = Cast<ATPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass()));
	target = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 현재 상태값 출력
	FString stateStr = UEnum::GetValueAsString(mState);
	// PRINTLOGTOSCREEN(TEXT("%s"), *stateStr);

	// 디버그모드가 활성화되어 있다면
	if (bDebugPlay)
	{
		// 기획자를 위해서 시각적인 디버깅이 가능하도록 해주자
		DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 20, FColor::Red);
	}
	
	
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

// 타겟쪽으로 이동하고 싶다.
// 필요속성 : 타겟, 이동속도, 부모
// 타겟이 공격범위 안에 들어오면 상태를 공격으로 전환하고 싶다.
// 필요속성 : 공격범위
void UEnemyFSM::MoveState()
{
	if (target == nullptr)
	{
		return;
	}
	FVector direction = target->GetActorLocation() - me->GetActorLocation();

	// 1. 타겟과의 거리가 있어야한다.
	float distance = direction.Size();
	
	direction.Normalize();

	me->AddMovementInput(direction);

	// 타겟이 공격범위 안에 들어오면 상태를 공격으로 전환하고 싶다.
	// 2. 공격범위안에 들어갔으니까
	// -> 타겟과의 거리가 공격범위보다 작다면
	if (distance < attackRange)
	{
		// 3. 상태를 공격으로 전환하고 싶다.
		mState = EEnemyState::Attack;
	}
}

// 일정시간에 한번씩 공격하고 싶다.
// 필요속성 : 공격대기시간
// 공격범위를 벗어나면 상태를 이동으로 전환하고 싶다.
void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		PRINTLOGTOSCREEN(TEXT("Attack!!!"));
	}
	// 공격범위를 벗어나면 상태를 이동으로 전환하고 싶다.
	// 1. 타겟과의 거리
	float distance = FVector::Dist(me->GetActorLocation(), target->GetActorLocation());
	// 2. 타겟과의 거리가 공격범위를 벗어났으니까
	if (distance > attackRange)
	{
		// 3. 상태를 이동으로 전환하고 싶다.
		mState = EEnemyState::Move;
	}
}

void UEnemyFSM::DamageState()
{
}

void UEnemyFSM::DieState()
{
}

// 피격당할 때 호출됨
void UEnemyFSM::OnDamageProcess()
{
	me->Destroy();
}
