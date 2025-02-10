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
};
