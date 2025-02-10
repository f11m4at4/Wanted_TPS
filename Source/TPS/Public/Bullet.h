// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TPS_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* sphereComp;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* bodyMesh;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* moveComp;

	UPROPERTY(EditAnywhere, Category=Settings)
	float speed = 1000;
	
	UPROPERTY(EditAnywhere, Category=Settings)
	float lifeTime = 2.0f;
	float currentTime = 0;
	// 죽음처리 함수
	void Die();

	// 액터의 특정 속성을 수정하면 자동으로 호출되는 이벤트 콜백
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};
