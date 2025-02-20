// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

#include "Enemy.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyManager::AEnemyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	// spawnpoints 찾기
	FindSpawnPoints();
	
	float createTime = FMath::RandRange(minTime, maxTime);
	// 타이머를 이용해서 적을 생성하자.
	GetWorldTimerManager().SetTimer(spawnTimeHandle, this, &AEnemyManager::CreateEnemy, createTime);
}

void AEnemyManager::CreateEnemy()
{
	// 랜덤한 위치에 적을 하나 만들기
	// 1. 랜덤한 위치
	// -> 배열의 크기가 필요
	int spawnSize = spawnPoints.Num();
	// -> 랜덤으로 하나 뽑기
	int index = FMath::RandRange(0, spawnSize-1);
	// 2. 적을 만들기
	auto spawn = spawnPoints[index]->GetTransform();
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawn);
	
	float createTime = FMath::RandRange(minTime, maxTime);
	// 타이머를 이용해서 적을 생성하자.
	GetWorldTimerManager().SetTimer(spawnTimeHandle, this, &AEnemyManager::CreateEnemy, createTime);
}

void AEnemyManager::FindSpawnPoints()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_SpawnPoint")))
		{
			spawnPoints.Add(spawn);
		}
	}
	// for (TActorIterator<AActor> it(GetWorld()); it; ++it)
	// {
	// 	AActor* spawn = *it;
	// 	// 이 액터가 spawnpoint 인지 체크
	// 	if (spawn->GetName().Contains(TEXT("BP_SpawnPoint")))
	// 	{
	// 		spawnPoints.Add(spawn);
	// 	}
	// }
}

