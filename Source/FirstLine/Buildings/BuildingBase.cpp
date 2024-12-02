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
    BuildingState = EBuildingState::Building;
    OnRep_BuildingState();
}

void ABuildingBase::UpdateConstruction_Implementation(float DeltaTime)
{
    if (GetLocalRole() != ROLE_Authority || BuildingState != EBuildingState::Building) return;

    ConstructionProgress += DeltaTime / ConstructionTime;
    if (ConstructionProgress >= 1.0f)
    {
        ConstructionProgress = 1.0f;
        BuildingState = EBuildingState::Complete;
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