// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPS_API AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// 랜덤시간 변수
	UPROPERTY(EditAnywhere, Category=SpawnSettings)
	float minTime = 1;
	UPROPERTY(EditAnywhere, Category=SpawnSettings)
	float maxTime = 5;
	// spawn할 위치들 저장하기
	UPROPERTY(EditAnywhere, Category=SpawnSettings)
	TArray<AActor*> spawnPoints;
	UPROPERTY(EditAnywhere, Category=SpawnSettings)
	TSubclassOf<class AEnemy> enemyFactory;

	// 타이머 변수
	FTimerHandle spawnTimeHandle;
	
	// 적을 만들 함수
	void CreateEnemy();

	// 스폰할 위치 동적으로 찾아 할당하는 함수
	void FindSpawnPoints();
};
