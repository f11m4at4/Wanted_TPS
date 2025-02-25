// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseComponent.h"

#include "TPS.h"


// Sets default values for this component's properties
UPlayerBaseComponent::UPlayerBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;	
}

void UPlayerBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// owner 값 할당
	me = Cast<ATPSPlayer>(GetOwner());
	if (me)
	{
		moveComp = me->GetCharacterMovement();
		PRINT_CALLINFO();
		// 입력 바인딩
		me->onInputBindingDelegate.AddUObject(this, &UPlayerBaseComponent::SetupInputBinding);
	}
}


// Called when the game starts
void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}











