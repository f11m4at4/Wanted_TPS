// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "AIController.h"
#include "Enemy.h"
#include "EnemyAnim.h"
#include "NavigationSystem.h"
#include "PlayerDamage.h"
#include "TPS.h"
#include "TPSPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Navigation/PathFollowingComponent.h"


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

	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	// ai controller 가져오기
	ai = Cast<AAIController>(me->GetController());
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
		anim->animState = mState;

		// 패트롤할 랜덤한 위치를 설정
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
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
	// 목적지
	FVector destination = target->GetActorLocation();
	FVector direction = destination - me->GetActorLocation();

	// 1. 타겟과의 거리가 있어야한다.
	float distance = direction.Size();
	
	direction.Normalize();

	// me->AddMovementInput(direction);
	// ai controller 를 이용해 이동시켜주자.
	// 1. 목적지로 이동가능한지 체크하자.
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;
	// 도착 영역 설정
	req.SetAcceptanceRadius(3);
	// 도착지 설정
	req.SetGoalLocation(destination);
	// 도착지에 도착할 수 있는지 질문 생성
	ai->BuildPathfindingQuery(req, query);
	// 질문하기
	FPathFindingResult r = ns->FindPathSync(query);
	// -> 가능하면 목적지로 이동해라
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	// 2. 그렇지 않으면
	else
	{
		// -> 랜덤위치 설정하고 그리로 이동해라
		// 1. 랜덤위치로 이동해라
		auto result = ai->MoveToLocation(randomPos);
		// 2. 랜덤위치에 도착하면?
		if (result == EPathFollowingRequestResult::Type::AlreadyAtGoal ||
			result == EPathFollowingRequestResult::Type::Failed)
		{
			//	-> 랜덤위치 다시 설정하기
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}
	
	// 타겟이 공격범위 안에 들어오면 상태를 공격으로 전환하고 싶다.
	// 2. 공격범위안에 들어갔으니까
	// -> 타겟과의 거리가 공격범위보다 작다면
	if (distance < attackRange)
	{
		// 3. 상태를 공격으로 전환하고 싶다.
		mState = EEnemyState::Attack;
		anim->animState = mState;
		// 바로 공격하도록 대기시간으로 설정
		currentTime = attackDelayTime;

		ai->StopMovement();
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
		
		anim->bAttackPlay = true;
	}
	// 공격범위를 벗어나면 상태를 이동으로 전환하고 싶다.
	// 1. 타겟과의 거리
	float distance = FVector::Dist(me->GetActorLocation(), target->GetActorLocation());
	// 2. 타겟과의 거리가 공격범위를 벗어났으니까
	if (distance > attackRange)
	{
		// 3. 상태를 이동으로 전환하고 싶다.
		mState = EEnemyState::Move;
		anim->animState = mState;

		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

// 일정시간이 지나면 상태를 Idle 로 전환하고 싶다.
void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 최종목적지까지 도착하면 상태를 Idle 로 전환
	float dist = FVector::Dist(me->GetActorLocation(), KnockBackDestPos);
	if (currentTime > damageDelayTime || dist < 10)
	{
		currentTime = 0;
		mState = EEnemyState::Idle;
	}

	// P = p0 + vt
	// FVector P = me->GetActorLocation() + force;
	// me->SetActorLocation(P);
	// -> 부드럽게 주어진 힘만큼 이동시키기
	// -> 러프를 이용해서 움직이게 해보자
	// Lerp (from, to, percent(speed))
	FVector from = me->GetActorLocation();
	float p = 10 * GetWorld()->DeltaTimeSeconds;

	FVector updatedPos = FMath::Lerp(from, KnockBackDestPos, p);
	me->SetActorLocation(updatedPos, true);

	// 회전 lerp(from, to, p)
	FRotator rotFrom = me->GetActorRotation();
	FVector toward = -knockBackForce;
	FRotator rotTo = UKismetMathLibrary::MakeRotFromZX(FVector::UpVector, toward);
	FRotator destRot = FMath::Lerp(rotFrom, rotTo, p);
	me->SetActorRotation(destRot);
}

// 아래로 계속 이동하다가 안보이면 제거하고싶다.
void UEnemyFSM::DieState()
{
	// 다이이동 가능할때 이동시키기
	if (anim->bDieMovingStart == false)
	{
		return;
	}
	
	// 1. 아래로 이동해야 한다.
	FVector P = me->GetActorLocation() + FVector::DownVector * dieMoveSpeed * GetWorld()->DeltaTimeSeconds;
	me->SetActorLocation(P);
	// 2. 위치가 -200 보다 작으면 없어지게 하자.
	if (me->GetActorLocation().Z < -200)
	{
		// 3. 제거하고 싶다.
		me->Destroy();
	}
}

// 피격당할 때 호출됨
void UEnemyFSM::OnDamageProcess(const FVector& force)
{
	--hp;
	
	currentTime = 0;
	
	// 만약 체력이 남아있다면
	if (hp > 0)
	{
		// -> 상태를 Damage 로 전환
		mState = EEnemyState::Damage;

		// 피격 애니메이션 재생
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnimation(FName(*sectionName));

		// 뒤로 밀리도록 구현하기
		// 맞은 방향으로 뒤로 밀리도록 처리하자
		// 필요속성 : 맞은 방향, 밀릴힘

		knockBackForce = force;
		knockBackForce.Z = 0;
		KnockBackDestPos = me->GetActorLocation() + knockBackForce;
	}
	// 그렇지 않으면
	else
	{
		// -> 상태를 Die 로 전환
		mState = EEnemyState::Die;
		// 캡슐 충돌체를 꺼져워 한다.
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 애니메이션 재생
		anim->PlayDamageAnimation(TEXT("Die"));
	}

	anim->animState = mState;
	// ai 이동 멈추기
	ai->StopMovement();
}

// 랜덤위치가져오기
bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLoc, float radius,
	FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation loc;
	bool bResult = ns->GetRandomReachablePointInRadius(centerLoc, radius, loc);

	// 찾은 랜덤위치 넘겨주기
	dest = loc.Location;
	return bResult;
}
