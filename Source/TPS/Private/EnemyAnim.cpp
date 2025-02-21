// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

#include "PlayerDamage.h"
#include "Kismet/GameplayStatics.h"

void UEnemyAnim::AnimNotify_AttackEnd()
{
	bAttackPlay = false;
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	// 아래로 내려가기 시작
	bDieMovingStart = true;
}

void UEnemyAnim::AnimNotify_AttackStart()
{
	// 플래이어한테 이벤트 알려주기
	auto target = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (target)
	{
		auto playerDamage = Cast<UPlayerDamage>(target->GetDefaultSubobjectByName(TEXT("PlayerDamage")));
		playerDamage->OnDamageProcess();
	}
}
