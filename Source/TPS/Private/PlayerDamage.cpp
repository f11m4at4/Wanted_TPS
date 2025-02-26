// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDamage.h"

#include "TPS.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UPlayerDamage::UPlayerDamage()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerDamage::BeginPlay()
{
	Super::BeginPlay();

	// 체력초기화
	hp = initialHP;

	// 피격 이벤트 받을 이벤트 콜백 함수 등록
	OnUpdateHealth.AddDynamic(this, &UPlayerDamage::UpdateHP);
	// 죽을때 처리할 콜백 함수 등록
	OnPlayerDeath.AddDynamic(this, &UPlayerDamage::GameOverProcess);
}


// Called every frame
void UPlayerDamage::TickComponent(float DeltaTime, ELevelTick TickType,
                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerDamage::OnDamageProcess()
{
	hp--;
	OnUpdateHealth.Broadcast();
	if (hp <=0)
	{
		OnPlayerDeath.Broadcast();
	}
	me->ShowDamageUI();
}

void UPlayerDamage::UpdateHP()
{
	PRINTLOGTOSCREEN(TEXT("Damaged!!! in c++"));
}

void UPlayerDamage::GameOverProcess()
{
	// 게임오버일 때 일시정시 시키자
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

