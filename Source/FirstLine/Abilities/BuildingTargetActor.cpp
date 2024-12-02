// BuildingTargetActor.cpp
#include "BuildingTargetActor.h"
#include "Buildings/BuildingBase.h"
#include "Components/BoxComponent.h"
#include "Targeting/BuildingPlacementReticle.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystemComponent.h"

ABuildingTargetActor::ABuildingTargetActor()
{
    PrimaryActorTick.bCanEverTick = true;
    bDestroyOnConfirmation = true;
    ShouldProduceTargetDataOnServer = true;

    // Create placement collision component
    PlacementCollision = CreateDefaultSubobject<UBoxComponent>("PlacementCollision");
    RootComponent = PlacementCollision;
    PlacementCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PlacementCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

    // Set replication
    bReplicates = true;
    SetReplicatingMovement(false);  // Disable movement replication since we handle it manually
}

FGameplayAbilityTargetDataHandle ABuildingTargetActor::MakeTargetData(const FHitResult& HitResult) const
{
    FGameplayAbilityTargetDataHandle Handle;

    FGameplayAbilityTargetData_LocationInfo* LocationData = new FGameplayAbilityTargetData_LocationInfo();
    LocationData->SourceLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
    LocationData->SourceLocation.LiteralTransform = GetActorTransform();
    LocationData->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
    LocationData->TargetLocation.LiteralTransform = FTransform(GetActorRotation(), HitResult.Location);

    Handle.Add(LocationData);
    return Handle;
}

void ABuildingTargetActor::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);

    // Set the owner to the ability's avatar actor
    if (Ability && Ability->GetAvatarActorFromActorInfo())
    {
        SetOwner(Ability->GetAvatarActorFromActorInfo());
    }

    if (!BuildingClass)
    {
        CancelTargeting();
        return;
    }

    // Only setup visualization on owning client
    if (!IsLocallyControlled())
    {
        return;
    }

    // Set a fixed rotation (facing forward)
    SetActorRotation(FRotator::ZeroRotator);

    // Create deferred actor for collision setup without spawning
    FTransform SpawnTransform(FRotator::ZeroRotator, FVector::ZeroVector);
    ABuildingBase* TempBuilding = GetWorld()->SpawnActorDeferred<ABuildingBase>(
        BuildingClass,
        SpawnTransform,
        nullptr,
        nullptr,
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn
    );

    if (TempBuilding)
    {
        // Setup placement collision based on building's collision
        UBoxComponent* BuildingCollision = TempBuilding->FindComponentByClass<UBoxComponent>();
        if (BuildingCollision)
        {
            PlacementCollision->SetBoxExtent(BuildingCollision->GetScaledBoxExtent());
            PlacementCollision->SetRelativeTransform(BuildingCollision->GetRelativeTransform());
        }

        // Create and setup reticle
        SetupBuildingReticle();
        if (BuildingReticle)
        {
            BuildingReticle->InitializeReticleVisualizationInformation(this, TempBuilding, ValidPlacementMaterial);
            BuildingReticle->SetActorRotation(FRotator::ZeroRotator);
        }
        TempBuilding->Destroy();
    }

    // Start with an initial trace
    UpdatePlacement();
}

void ABuildingTargetActor::SetupBuildingReticle()
{
    // Reticle should only exist on owning client
    if (!IsLocallyControlled() || BuildingReticle || !ensure(BuildingClass))
    {
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    BuildingReticle = GetWorld()->SpawnActor<ABuildingVisualizationReticle>(
        ABuildingVisualizationReticle::StaticClass(),
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        SpawnParams
    );
}

void ABuildingTargetActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Only update placement on owning client
    if (IsLocallyControlled())
    {
        UpdatePlacement();
    }
}

void ABuildingTargetActor::UpdatePlacement()
{
    if (!IsLocallyControlled())
    {
        return;
    }

    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC)
    {
        PC = UGameplayStatics::GetPlayerController(this, 0);
        if (!PC) return;
    }

    // Get mouse position
    FVector2D MousePosition;
    if (!PC->GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        return;
    }

    // Perform deproject
    FVector WorldOrigin, WorldDirection;
    if (PC->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldOrigin, WorldDirection))
    {
        // Line trace from cursor
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        if (BuildingReticle)
        {
            QueryParams.AddIgnoredActor(BuildingReticle);
        }

        FHitResult HitResult;
        const float TraceDistance = 100000.0f;
        const FVector TraceEnd = WorldOrigin + (WorldDirection * TraceDistance);
        
        if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, TraceEnd, ECC_Visibility, QueryParams))
        {
            LastHitResult = HitResult;
            const FVector CurrentLocation = GetActorLocation();
            const FVector NewLocation = HitResult.Location;

            // Only update location if it has changed significantly
            const float LocationTolerance = 1.0f;
            if (!CurrentLocation.Equals(NewLocation, LocationTolerance))
            {
                // Create the target transform
                FTransform NewTransform = FTransform::Identity;
                NewTransform.SetLocation(NewLocation);
                
                // Set the transform in one operation
                SetActorTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);

                // Update client-only visualization
                if (BuildingReticle)
                {
                    BuildingReticle->SetActorTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);
                }
            }

            // Check placement validity
            bool bNewCanPlaceHere = IsValidPlacementLocation(HitResult);
            if (bNewCanPlaceHere != bCanPlaceHere)
            {
                bCanPlaceHere = bNewCanPlaceHere;
                if (BuildingReticle)
                {
                    UMaterialInterface* MaterialToUse = bCanPlaceHere ? ValidPlacementMaterial : InvalidPlacementMaterial;
                    TArray<UMeshComponent*> MeshComponents;
                    BuildingReticle->GetComponents<UMeshComponent>(MeshComponents);
                    for (UMeshComponent* Mesh : MeshComponents)
                    {
                        if (Mesh && Mesh->GetMaterial(0) != MaterialToUse)
                        {
                            Mesh->SetMaterial(0, MaterialToUse);
                        }
                    }
                }
            }
        }
    }
}

bool ABuildingTargetActor::IsValidPlacementLocation(const FHitResult& HitResult) const
{
    if (!HitResult.bBlockingHit) return false;

    // Check for overlapping actors using box collision
    TArray<AActor*> OverlappingActors;
    PlacementCollision->GetOverlappingActors(OverlappingActors);
    return OverlappingActors.Num() == 0;
}

void ABuildingTargetActor::ConfirmTargetingAndContinue()
{
    // Server validates placement
    if (GetLocalRole() == ROLE_Authority)
    {
        bCanPlaceHere = IsValidPlacementLocation(LastHitResult);
    }

    if (!bCanPlaceHere)
    {
        CancelTargeting();
        return;
    }

    FGameplayAbilityTargetDataHandle Handle = MakeTargetData(LastHitResult);
    TargetDataReadyDelegate.Broadcast(Handle);
}

void ABuildingTargetActor::CancelTargeting()
{
    // Only destroy reticle on owning client
    if (IsLocallyControlled() && BuildingReticle)
    {
        BuildingReticle->Destroy();
        BuildingReticle = nullptr;
    }
    Super::CancelTargeting();
}

void ABuildingTargetActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Only destroy reticle on owning client
    if (IsLocallyControlled() && BuildingReticle)
    {
        BuildingReticle->Destroy();
        BuildingReticle = nullptr;
    }
    Super::EndPlay(EndPlayReason);
}

bool ABuildingTargetActor::IsLocallyControlled() const
{
    // Check if we're on a client and if our owner is locally controlled
    if (GetNetMode() != NM_DedicatedServer)
    {
        APlayerController* PC = Cast<APlayerController>(GetOwner());
        if (PC)
        {
            return PC->IsLocalController();
        }

        // If owner is not a player controller, check if it's a locally controlled pawn
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (OwnerPawn)
        {
            return OwnerPawn->IsLocallyControlled();
        }
    }
    
    return false;
}

void ABuildingTargetActor::SetBuildingClass(TSubclassOf<ABuildingBase> InBuildingClass)
{
    BuildingClass = InBuildingClass;
}
