#include "BuildingSelectionWidget.h"

void UBuildingSelectionWidget::SelectBuilding(FName BuildingRowName)
{
    OnBuildingSelected.Broadcast(BuildingRowName);
} 