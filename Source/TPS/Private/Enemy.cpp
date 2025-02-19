// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyAnim.h"
#include "EnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// mesh data 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -86));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.83f));
	}

	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// abp 로드
	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnim(TEXT("'/Game/Blueprints/ABP_Enemy.ABP_Enemy_C'"));
	if (TempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// auto anim = StaticLoadClass(UEnemyAnim::StaticClass(), nullptr, TEXT("'/Game/Blueprints/ABP_Enemy.ABP_Enemy_C'"));
	// FSoftClassPath path(TEXT("'/Game/Blueprints/ABP_Enemy.ABP_Enemy_C'"));
	// TSoftClassPtr<UEnemyAnim> softClass(path);
	// if (softClass.IsValid())
	// {
	// 	auto anim = softClass.LoadSynchronous();
	// }
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

