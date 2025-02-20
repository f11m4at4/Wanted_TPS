// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Settings)
	float walkSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Settings)
	float runSpeed = 600;
	
	UPROPERTY()
	FVector direction;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* imc_tps;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_move;

	// 이동처리 함수
	void Move(const struct FInputActionValue& inputValue);

	// InputAction 회전
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_turn;
	void Turn(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Lookup;
	void Lookup(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_jump;
	void InputJump(const struct FInputActionValue& inputValue);

	// 총알공장
	UPROPERTY(EditDefaultsOnly, Category=Bullet)
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_fire;
	void InputFire(const struct FInputActionValue& inputValue);

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* sniperGunComp;

	// 유탄총 사용여부
	bool bUsingGrenadeGun = false;
	//유탄총으로 변경
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_grenadeGun;
	void ChangeToGrenadeGun(const struct FInputActionValue& inputValue);
	//스나이퍼로 변경
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_sniperGun;
	void ChangeToSniperGun(const struct FInputActionValue& inputValue);

	// 현재 aim 모드 저장
	bool bSniperAim = false;
	//WBP_sniperuI
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUserWidget> sniperUIFactory;
	// INstance
	UPROPERTY()
	class UUserWidget* sniperUI;
	// input
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_sniperScope;
	void SniperAim(const struct FInputActionValue& inputValue);

	// 총알파편 효과
	UPROPERTY(EditDefaultsOnly, Category=Bullet)
	class UParticleSystem* bulletEffectFactory;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	// INstance
	UPROPERTY()
	class UUserWidget* crosshairUI;

	// 달리기 입력
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_run;
	void InputRun(const struct FInputActionValue& inputValue);

	// 카메라 셰이크 블푸저장할 변수
	UPROPERTY(EditDefaultsOnly, Category=CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;


public:
	UPROPERTY(VisibleAnywhere, Category=PlayerComponent)
	class UPlayerBaseComponent* playerMove;
};
