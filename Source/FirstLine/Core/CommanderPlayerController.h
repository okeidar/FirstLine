// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CommanderPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTLINE_API ACommanderPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ACommanderPlayerController();
    
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void MoveCameraByInput(const FVector2D& Value);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ScrollSpeed = 500.0f;
    
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float EdgeScrollThreshold = 20.0f;

private:

	FVector2D MovementVector{0.f,0.f};

	void EdgeScroll();
	
};
