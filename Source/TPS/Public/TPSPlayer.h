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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* cameraComp;


	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* imc_tps;



	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* sniperGunComp;

public:
	UPROPERTY(VisibleAnywhere, Category=PlayerComponent)
	class UPlayerBaseComponent* playerMove;
	UPROPERTY(VisibleAnywhere, Category=PlayerComponent)
	class UPlayerBaseComponent* playerFire;
	UPROPERTY(VisibleAnywhere, Category=PlayerComponent)
	class UPlayerBaseComponent* playerDamage;

	// DamageUI 실행하는 함수
	UFUNCTION(BlueprintImplementableEvent, Category=DamageEvent)
	void ShowDamageUI();
};
