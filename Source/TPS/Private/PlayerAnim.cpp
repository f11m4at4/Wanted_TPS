// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"

#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 소유하고 있는 폰 얻어오기
	auto ownerPawn = TryGetPawnOwner();
	// TPSPlayer 로 cast 해야 한다.
	auto player = Cast<ATPSPlayer>(ownerPawn);
	if (player)
	{
		// speed 값을 설정
		FVector velocity = player->GetVelocity();
		// 전방벡터
		FVector forward = player->GetActorForwardVector();
		speed = FVector::DotProduct(velocity, forward);

		// 좌우방향
		FVector right = player->GetActorRightVector();
		direction = FVector::DotProduct(velocity, right);
		
		// 공중에 있는지 여부
		bIsInAir = player->GetCharacterMovement()->IsFalling();
	}
}











