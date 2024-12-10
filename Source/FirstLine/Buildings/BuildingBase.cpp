#include "BuildingBase.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

ABuildingBase::ABuildingBase()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>("BuildingMesh");
    RootComponent = BuildingMesh;

    BuildingState = EBuildingState::Placing;
    ConstructionProgress = 0.0f;
    ConstructionTime = 10.0f;

    PlacementCollision = CreateDefaultSubobject<UBoxComponent>("PlacementCollision");
    PlacementCollision->SetupAttachment(BuildingMesh);
    PlacementCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
    PlacementCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
    PlacementCollision->OnComponentBeginOverlap.AddDynamic(this, &ABuildingBase::OnPlacementOverlapBegin);
    PlacementCollision->OnComponentEndOverlap.AddDynamic(this, &ABuildingBase::OnPlacementOverlapEnd);
    bCanBePlaced = true;
}

void ABuildingBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABuildingBase, BuildingState);
    DOREPLIFETIME(ABuildingBase, ConstructionProgress);
    DOREPLIFETIME(ABuildingBase, AssignedWorker);
}

bool ABuildingBase::CanPlaceAt_Implementation(const FVector& Location) const
{
    // Basic implementation - can be overridden in BP for specific building requirements
    return true;
}

void ABuildingBase::StartConstruction_Implementation(APawn* Worker)
{
    if (GetLocalRole() != ROLE_Authority) return;

    AssignedWorker = Worker;
    BuildingState = EBuildingState::Foundation;
    OnRep_BuildingState();
}

void ABuildingBase::UpdateConstruction_Implementation(float DeltaTime)
{
    IConstructable::UpdateConstruction_Implementation(DeltaTime);
}

void ABuildingBase::UpdateConstructionProgress(float DeltaProgress)
{
    if (GetLocalRole() != ROLE_Authority || BuildingState == EBuildingState::Complete) 
        return;

    float OldProgress = ConstructionProgress;
    ConstructionProgress = FMath::Clamp(ConstructionProgress + DeltaProgress, 0.0f, 1.0f);
    
    // Calculate and update building stage
    EBuildingState NewState = BuildingState;
    if (ConstructionProgress >= 1.0f)
    {
        NewState = EBuildingState::Complete;
    }
    else
    {
        // Determine stage based on progress
        if (ConstructionProgress < 0.2f) NewState = EBuildingState::Foundation;
        else if (ConstructionProgress < 0.4f) NewState = EBuildingState::Framework;
        else if (ConstructionProgress < 0.6f) NewState = EBuildingState::Structure;
        else if (ConstructionProgress < 0.8f) NewState = EBuildingState::Walls;
        else NewState = EBuildingState::Finishing;
    }

    // If stage changed, update state and notify blueprint
    if (NewState != BuildingState)
    {
        BuildingState = NewState;
        OnConstructionStateChanged(BuildingState);
        OnRep_BuildingState();
    }
}

void ABuildingBase::WorkerRemoved_Implementation(APawn* Worker)
{
    if (GetLocalRole() != ROLE_Authority || Worker != AssignedWorker) return;
    
    AssignedWorker = nullptr;
    // Building remains in Building state but won't progress without worker
}

void ABuildingBase::SetBuildingState(EBuildingState NewBuildingState)
{
    BuildingState=NewBuildingState;
}

void ABuildingBase::OnRep_BuildingState()
{
    OnConstructionStateChanged(BuildingState);
}

void ABuildingBase::OnPlacementOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (BuildingState == EBuildingState::Placing && OtherActor != this)
    {
        bCanBePlaced = false;
    }
}

void ABuildingBase::OnPlacementOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (BuildingState == EBuildingState::Placing)
    {
        // Check if there are any remaining overlaps
        TArray<AActor*> OverlappingActors;
        PlacementCollision->GetOverlappingActors(OverlappingActors);
        bCanBePlaced = OverlappingActors.Num() == 0;
    }
} 