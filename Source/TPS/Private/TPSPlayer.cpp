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
#include "TPS.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


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

	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGun(TEXT("'/Game/SniperGun/sniper11.sniper11'"));
	if (TempSniperGun.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperGun.Object);
		sniperGunComp->SetRelativeLocation(FVector(-20,40,130));
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIASniperGun(TEXT("'/Game/Input/IA_SniperGun.IA_SniperGun'"));
	if (TempIASniperGun.Succeeded())
	{
		ia_sniperGun = TempIASniperGun.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIAGrenadeGun(TEXT("'/Game/Input/IA_GrenadeGun.IA_GrenadeGun'"));
	if (TempIAGrenadeGun.Succeeded())
	{
		ia_grenadeGun = TempIAGrenadeGun.Object;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> TempSniperUI(TEXT("'/Game/Blueprints/WBP_SniperUI.WBP_SniperUI_C'"));
	if (TempSniperUI.Succeeded())
	{
		sniperUIFactory = TempSniperUI.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> TempCrosshairUI(TEXT("'/Game/Blueprints/WBP_CrosshairUI.WBP_CrosshairUI_C'"));
	if (TempCrosshairUI.Succeeded())
	{
		crosshairUIFactory = TempCrosshairUI.Class;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIASniperScope(TEXT("'/Game/Input/IA_SniperScope.IA_SniperScope'"));
	if (TempIASniperScope.Succeeded())
	{
		ia_sniperScope = TempIASniperScope.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> TempBulletEffect(TEXT("'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (TempBulletEffect.Succeeded())
	{
		bulletEffectFactory = TempBulletEffect.Object;
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

	// 스나이퍼 UI 위젯 인스턴스 생성
	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// crosshair ui
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	crosshairUI->AddToViewport();
	
	// 스나이퍼건으로 사용되도록 설정
	ChangeToSniperGun(FInputActionValue());
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
		pi->BindAction(ia_grenadeGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToGrenadeGun);
		pi->BindAction(ia_sniperGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToSniperGun);
		// sniper aim 입력 바인딩
		pi->BindAction(ia_sniperScope, ETriggerEvent::Started, this, &ATPSPlayer::SniperAim);
		pi->BindAction(ia_sniperScope, ETriggerEvent::Completed, this, &ATPSPlayer::SniperAim);
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
	// 현재 유탄총을 상용중인지 스나이퍼 총을 사용중인지
	if (bUsingGrenadeGun)
	{
		// FirePosition 총구위치에서 발사하고 싶다.
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	// 스나이퍼건 사용시
	else
	{
		FVector startPos = cameraComp->GetComponentLocation();
		FVector endPos = startPos + cameraComp->GetForwardVector() * 10000;
		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(this);
		// line 을 쏜다.
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, param);
		// 선이 특정 물체와 부딪히면
		// 부딪힌 물체 날려보내자.
		if (bHit)
		{
			// 총알 파편 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, hitInfo.Location);
			
			// 컴포넌트 simulate physics 가 활성화 되어 있어야 한다.
			auto hitComp = hitInfo.GetComponent();
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				PRINT_CALLINFO();
				// F = ma
				FVector force = cameraComp->GetForwardVector() * 500000;
				//float mass = hitComp->GetMass();
				hitComp->AddImpulseAtLocation(force, hitInfo.Location);
			}
		}
	}
}

void ATPSPlayer::ChangeToGrenadeGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = true;
	gunMeshComp->SetVisibility(true);
	sniperGunComp->SetVisibility(false);
}

void ATPSPlayer::ChangeToSniperGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = false;

	gunMeshComp->SetVisibility(false);
	sniperGunComp->SetVisibility(true);
}

// 스나이퍼 줌모드 입력처리
void ATPSPlayer::SniperAim(const struct FInputActionValue& inputValue)
{
	// 현재 aim 모드 일때
	if (bSniperAim)
	{
		bSniperAim = false;
		// -> add to viewport
		sniperUI->RemoveFromParent();
		// -> camera fov 45 로 설정
		cameraComp->SetFieldOfView(90.0f);
		// crosshair 보이게처리
		crosshairUI->AddToViewport();
	}
	// 그렇지 않을 때
	else
	{
		bSniperAim = true;
		// -> remove
		sniperUI->AddToViewport();
		// -> fov 90
		cameraComp->SetFieldOfView(45.0f);
		crosshairUI->RemoveFromParent();
	}
}
