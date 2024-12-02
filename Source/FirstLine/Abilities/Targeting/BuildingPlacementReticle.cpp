// BuildingVisualizationReticle.cpp
#include "BuildingPlacementReticle.h"
#include "Components/CapsuleComponent.h"
#include "Components/MeshComponent.h"

ABuildingVisualizationReticle::ABuildingVisualizationReticle(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("CollisionCapsule0");
    CollisionComponent->InitCapsuleSize(0.f, 0.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = CollisionComponent;

    // Ensure we never replicate movement
    bReplicates = false;
}

void ABuildingVisualizationReticle::InitializeReticleVisualizationInformation(AActor* InTargetingActor, AActor* VisualizationActor, UMaterialInterface* VisualizationMaterial)
{
    if (!VisualizationActor) return;

    // Get all mesh components from the visualization actor
    TInlineComponentArray<UMeshComponent*> MeshComps;
    USceneComponent* MyRoot = GetRootComponent();
    VisualizationActor->GetComponents(MeshComps);

    TargetingActor = InTargetingActor;
    AddTickPrerequisiteActor(TargetingActor);

    for (UMeshComponent* MeshComp : MeshComps)
    {
        // Clear root if needed
        if (MeshComp == VisualizationActor->GetRootComponent())
        {
            VisualizationActor->SetRootComponent(nullptr);
        }

        // Disable collision and physics
        MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MeshComp->SetSimulatePhysics(false);
        MeshComp->SetEnableGravity(false);

        // Disable component interpolation
        MeshComp->SetComponentTickEnabled(false);
        MeshComp->SetVisibleInSceneCaptureOnly(false);
        MeshComp->SetUsingAbsoluteRotation(true);

        // Get the relative transform before detaching
        FTransform RelativeTransform = MeshComp->GetRelativeTransform();
        RelativeTransform.SetRotation(FQuat::Identity); // Force zero rotation

        // Move component to reticle directly without keeping world transform
        MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        MeshComp->AttachToComponent(MyRoot, FAttachmentTransformRules::KeepRelativeTransform);
        MeshComp->SetRelativeTransform(RelativeTransform, false, nullptr, ETeleportType::TeleportPhysics);
        MeshComp->Rename(nullptr, this);

        if (VisualizationMaterial)
        {
            MeshComp->SetMaterial(0, VisualizationMaterial);
        }

        VisualizationComponents.Add(MeshComp);
    }
}

void ABuildingVisualizationReticle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}
