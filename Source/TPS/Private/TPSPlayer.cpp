// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Bullet.h"
#include "EnemyFSM.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PlayerAnim.h"
#include "PlayerDamage.h"
#include "PlayerFire.h"
#include "PlayerMove.h"
#include "TPS.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Mesh 데이터 할당하기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (TempMesh.Object != nullptr)
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0,-90,0));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	// 위치값 지정하기
	springArmComp->SetRelativeLocation(FVector(0, 70, 50));

	// Camera 컴포넌트 생성
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);

	// imc_tps
	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("'/Game/Input/IMC_TPS.IMC_TPS'"));
	if (TempIMC.Succeeded())
	{
		imc_tps = TempIMC.Object;
	}
	
	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 유탄발사기 총
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGun(TEXT("'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGun.Object);
		gunMeshComp->SetRelativeLocation(FVector(-8.722623,5.497995,-0.691068));
		gunMeshComp->SetRelativeRotation(FRotator(21.232685,86.422334,-10.949290));
	}

	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGun(TEXT("'/Game/SniperGun/sniper11.sniper11'"));
	if (TempSniperGun.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperGun.Object);
		sniperGunComp->SetRelativeLocation(FVector(-38.476999,0.890077,8.579185));
		sniperGunComp->SetRelativeRotation(FRotator(21.232685,86.422334,-10.949290));
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}
	
	// ABP 등록 해주기
	ConstructorHelpers::FClassFinder<UAnimInstance> TempABP(TEXT("'/Game/Animations/ABP_Player.ABP_Player_C'"));

	if (TempABP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempABP.Class);
	}
	
	// Player Move 컴포넌트 추가
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	// PlayerFire 컴포넌트 추가
	// playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
	// PlayerDamage 컴포넌트 추가
	playerDamage = CreateDefaultSubobject<UPlayerDamage>(TEXT("PlayerDamage"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// EnhancedInput 설정
	// imc_tps
	// ia_playerMove
	//GetWorld()->GetFirstPlayerController()
	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSystem)
		{
			subSystem->AddMappingContext(imc_tps, 0);
		}
	}
}


// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto pi = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (pi)
	{
		PRINT_CALLINFO();
		// 내 자식 컴포넌트한테 입력 바인딩 하도록 호출
		onInputBindingDelegate.Broadcast(pi);
		// playerMove->SetupInputBinding(pi);
		// playerFire->SetupInputBinding(pi);
	}
}
