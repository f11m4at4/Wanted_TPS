// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = sphereComp;

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	bodyMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (TempMesh.Succeeded())
	{
		bodyMesh->SetStaticMesh(TempMesh.Object);
		bodyMesh->SetRelativeLocation(FVector(0, 0, -50));
	}
	
	moveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	moveComp->SetUpdatedComponent(sphereComp);
	moveComp->InitialSpeed = 5000;
	moveComp->MaxSpeed = 5000;
	moveComp->bShouldBounce = true;
	moveComp->Bounciness = 0.3f;

	// InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	// Die 가 2초후에 호출되도록 타이머 맞추자
	// 알람시계
	FTimerHandle deathTimer;
	// 알람맞추기

	auto dieCallback = [this]()->void
	{
		Destroy();	
	};
	GetWorldTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda(dieCallback), lifeTime, false);
}
// lambda, 람다

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// currentTime += DeltaTime;
	// if (currentTime > lifeTime)
	// {
	// 	Die();
	// }
}

void ABullet::Die()
{
	Destroy();
}

void ABullet::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// speed 값이 수정됐는지 체크
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		moveComp->InitialSpeed = speed;
		moveComp->MaxSpeed = speed;
	}
}

