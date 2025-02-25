// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "TPS.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UPlayerMove::UPlayerMove()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

	ConstructorHelpers::FObjectFinder<UInputAction> TempIAMove(TEXT("'/Game/Input/IA_PlayerMove.IA_PlayerMove'"));
	if (TempIAMove.Succeeded())
	{
		ia_move = TempIAMove.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIAJump(TEXT("'/Game/Input/IA_PlayerJump.IA_PlayerJump'"));
	if (TempIAJump.Succeeded())
	{
		ia_jump = TempIAJump.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIARun(TEXT("'/Game/Input/IA_Run.IA_Run'"));
	if (TempIARun.Succeeded())
	{
		ia_run = TempIARun.Object;
	}

}


// Called when the game starts
void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	// 속도초기화
	moveComp->MaxWalkSpeed = walkSpeed;
}


// Called every frame
void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType,
                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// P = P0 + vt
	// direction 을 컨트롤러가 바라보는 방향으로 변환
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	
	// FVector P0 = GetActorLocation();
	// FVector vt = direction * walkSpeed * DeltaTime;
	// FVector P = P0 + vt;
	// SetActorLocation(P);
	me->AddMovementInput(direction);
	direction = FVector::Zero();
}

void UPlayerMove::SetupInputBinding(class UEnhancedInputComponent* pi)
{
	Super::SetupInputBinding(pi);

	PRINT_CALLINFO();
	pi->BindAction(ia_turn, ETriggerEvent::Triggered, this, &UPlayerMove::Turn);
	pi->BindAction(ia_Lookup, ETriggerEvent::Triggered, this, &UPlayerMove::Lookup);

	pi->BindAction(ia_move, ETriggerEvent::Triggered, this, &UPlayerMove::Move);
	pi->BindAction(ia_jump, ETriggerEvent::Started, this, &UPlayerMove::InputJump);
	pi->BindAction(ia_run, ETriggerEvent::Started, this, &UPlayerMove::InputRun);
	pi->BindAction(ia_run, ETriggerEvent::Completed, this, &UPlayerMove::InputRun);
}


void UPlayerMove::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	me->AddControllerYawInput(value);
}

void UPlayerMove::Lookup(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	me->AddControllerPitchInput(value);
}


void UPlayerMove::Move(const FInputActionValue& inputValue)
{
	FVector2d dir = inputValue.Get<FVector2d>();
	direction.X = dir.Y;
	direction.Y = dir.X;
}


void UPlayerMove::InputJump(const struct FInputActionValue& inputValue)
{
	me->Jump();
}

void UPlayerMove::InputRun(const struct FInputActionValue& inputValue)
{
	auto movement = moveComp;
	// 달리기모드일때
	if (movement->MaxWalkSpeed > walkSpeed)
	{
		// -> 걷기로
		movement->MaxWalkSpeed = walkSpeed;
	}
	// 걷기 중일때
	else
	{
		// -> 달리기로
		movement->MaxWalkSpeed = runSpeed;
	}
}