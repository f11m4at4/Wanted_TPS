// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Bullet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	springArmComp->SetRelativeLocation(FVector(0, 0, 50));

	// Camera 컴포넌트 생성
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);

	// imc_tps
	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("'/Game/Input/IMC_TPS.IMC_TPS'"));
	if (TempIMC.Succeeded())
	{
		imc_tps = TempIMC.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAMove(TEXT("'/Game/Input/IA_PlayerMove.IA_PlayerMove'"));
	if (TempIAMove.Succeeded())
	{
		ia_move = TempIAMove.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIATurn(TEXT("'/Game/Input/IA_PlayerTurn.IA_PlayerTurn'"));
	if (TempIATurn.Succeeded())
	{
		ia_turn = TempIATurn.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIALookup(TEXT("'/Game/Input/IA_PlayerLookup.IA_PlayerLookup'"));
	if (TempIALookup.Succeeded())
	{
		ia_Lookup = TempIALookup.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIAJump(TEXT("'/Game/Input/IA_PlayerJump.IA_PlayerJump'"));
	if (TempIAJump.Succeeded())
	{
		ia_jump = TempIAJump.Object;
	}

	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 총알발사 입력 애셋로드
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAFire(TEXT("'/Game/Input/IA_Fire.IA_Fire'"));
	if (TempIAFire.Succeeded())
	{
		ia_fire = TempIAFire.Object;
	}

	ConstructorHelpers::FClassFinder<ABullet> TempBullet(TEXT("'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));

	if (TempBullet.Succeeded())
	{
		bulletFactory = TempBullet.Class;
	}

	// 유탄발사기 총
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGun(TEXT("'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGun.Object);
		gunMeshComp->SetRelativeLocation(FVector(-20.000000,20.000000,110.000000));
	}
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

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// P = P0 + vt
	// direction 을 컨트롤러가 바라보는 방향으로 변환
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	
	// FVector P0 = GetActorLocation();
	// FVector vt = direction * speed * DeltaTime;
	// FVector P = P0 + vt;
	// SetActorLocation(P);
	AddMovementInput(direction);
	direction = FVector::Zero();
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto pi = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (pi)
	{
		pi->BindAction(ia_move, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
		pi->BindAction(ia_turn, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
		pi->BindAction(ia_Lookup, ETriggerEvent::Triggered, this, &ATPSPlayer::Lookup);
		pi->BindAction(ia_jump, ETriggerEvent::Started, this, &ATPSPlayer::InputJump);
		pi->BindAction(ia_fire, ETriggerEvent::Started, this, &ATPSPlayer::InputFire);
	}
}

void ATPSPlayer::Move(const FInputActionValue& inputValue)
{
	FVector2d dir = inputValue.Get<FVector2d>();
	direction.X = dir.Y;
	direction.Y = dir.X;
}

void ATPSPlayer::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void ATPSPlayer::Lookup(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}

void ATPSPlayer::InputJump(const struct FInputActionValue& inputValue)
{
	Jump();
}

void ATPSPlayer::InputFire(const struct FInputActionValue& inputValue)
{
	// FirePosition 총구위치에서 발사하고 싶다.
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
}