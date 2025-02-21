// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPS_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerFire();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* pi) override;

public:
	UPROPERTY()
	class UCameraComponent* cameraComp;
	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY()
	class UStaticMeshComponent* sniperGunComp;
public:
	// input
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_sniperScope;
	void SniperAim(const struct FInputActionValue& inputValue);

	// 현재 aim 모드 저장
	bool bSniperAim = false;
	//WBP_sniperuI
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUserWidget> sniperUIFactory;
	// INstance
	UPROPERTY()
	class UUserWidget* sniperUI;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	// INstance
	UPROPERTY()
	class UUserWidget* crosshairUI;


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

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_fire;
	void InputFire(const struct FInputActionValue& inputValue);

	// 카메라 셰이크 블푸저장할 변수
	UPROPERTY(EditDefaultsOnly, Category=CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

	// 총알공장
	UPROPERTY(EditDefaultsOnly, Category=Bullet)
	TSubclassOf<class ABullet> bulletFactory;
	// 총알파편 효과
	UPROPERTY(EditDefaultsOnly, Category=Bullet)
	class UParticleSystem* bulletEffectFactory;


};
