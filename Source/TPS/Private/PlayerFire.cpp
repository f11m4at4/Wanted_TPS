// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"

#include "Bullet.h"
#include "EnemyFSM.h"
#include "EnhancedInputComponent.h"
#include "PlayerAnim.h"
#include "TPS.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


// Sets default values for this component's properties
UPlayerFire::UPlayerFire()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UInputAction> TempIASniperScope(TEXT("'/Game/Input/IA_SniperScope.IA_SniperScope'"));
	if (TempIASniperScope.Succeeded())
	{
		ia_sniperScope = TempIASniperScope.Object;
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

	ConstructorHelpers::FObjectFinder<UParticleSystem> TempBulletEffect(TEXT("'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (TempBulletEffect.Succeeded())
	{
		bulletEffectFactory = TempBulletEffect.Object;
	}

}


// Called when the game starts
void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	cameraComp = me->cameraComp;
	gunMeshComp = me->gunMeshComp;
	sniperGunComp = me->sniperGunComp;

	// 스나이퍼 UI 위젯 인스턴스 생성
	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// crosshair ui
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	crosshairUI->AddToViewport();

	// 스나이퍼건으로 사용되도록 설정
	ChangeToSniperGun(FInputActionValue());
}

void UPlayerFire::SetupInputBinding(class UEnhancedInputComponent* pi)
{
	Super::SetupInputBinding(pi);
	// sniper aim 입력 바인딩
	pi->BindAction(ia_sniperScope, ETriggerEvent::Started, this, &UPlayerFire::SniperAim);
	pi->BindAction(ia_sniperScope, ETriggerEvent::Completed, this, &UPlayerFire::SniperAim);
	pi->BindAction(ia_grenadeGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToGrenadeGun);
	pi->BindAction(ia_sniperGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToSniperGun);

	pi->BindAction(ia_fire, ETriggerEvent::Started, this, &UPlayerFire::InputFire);
}

// 스나이퍼 줌모드 입력처리
void UPlayerFire::SniperAim(const struct FInputActionValue& inputValue)
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

void UPlayerFire::ChangeToGrenadeGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = true;
	gunMeshComp->SetVisibility(true);
	sniperGunComp->SetVisibility(false);
}

void UPlayerFire::ChangeToSniperGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = false;

	gunMeshComp->SetVisibility(false);
	sniperGunComp->SetVisibility(true);
}

void UPlayerFire::InputFire(const struct FInputActionValue& inputValue)
{
	// 카메라 진동 재생
	auto pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		pc->PlayerCameraManager->StartCameraShake(cameraShake);
	}

	
	// 총쏘기 애니메이션 재생
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	if (anim)
	{
		anim->OnPlayFireAnimation();
	}
	
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
		param.AddIgnoredActor(me);
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

			// 맞은녀석이 적이면 피격 이벤트 호출해주자.
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto fsm = Cast<UEnemyFSM>(enemy);
				FVector force = cameraComp->GetForwardVector() * 500;
				fsm->OnDamageProcess(force);
			}
		}
	}
}