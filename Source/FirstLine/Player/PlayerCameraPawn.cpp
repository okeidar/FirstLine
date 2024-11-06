// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraPawn.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APlayerCameraPawn::APlayerCameraPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 0;
	SpringArm->bEnableCameraLag = true;
    RootComponent = SpringArm;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeRotation({-70,0,0});

}


