#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CommanderHUD.generated.h"

UCLASS(Abstract, Blueprintable)
class FIRSTLINE_API ACommanderHUD : public AHUD
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintImplementableEvent, Category = "Selection")
    void StartSelection(const FVector2D& ScreenPosition);

    UFUNCTION(BlueprintImplementableEvent, Category = "Selection")
    void UpdateSelection(const FVector2D& CurrentScreenPos);

    UFUNCTION(BlueprintImplementableEvent, Category = "Selection")
    void EndSelection(TArray<AActor*>& OutActors);

}; 