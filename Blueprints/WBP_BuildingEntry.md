# Building Entry Widget Blueprint

## Parent Class
UserWidget

## Description
Single entry in the building menu representing one building type with its icon, name and costs.

## Designer View
### Root
- Button "BuildButton" (Root)
  * Size: 120x140
  * Padding: 5 all sides
  * HorizontalAlignment: Fill
  * VerticalAlignment: Fill

### Layout
- Vertical Box "MainLayout"
  * Padding: 2 all sides
  * Child 1: Image "BuildingIcon"
    - Size: 100x100
    - Alignment: Center
  * Child 2: TextBlock "BuildingName"
    - Font: Roboto 12
    - Alignment: Center
    - Wrapping: No wrap
  * Child 3: Horizontal Box "CostsContainer"
    - Alignment: Center
    - Padding: 2 all sides
    - Will be populated in Event Graph

## Variables
### Input
- BuildingData (FBuildingData)
  * Category: "Input"
  * BlueprintReadWrite
  * ExposeOnSpawn

### Internal
- CostEntryClass (TSubclassOf<UUserWidget>)
  * Category: "UI"
  * EditDefaultsOnly
  * Description: "Widget class for cost entries"

## Event Graph

### Event Construct
1. No nodes needed, widget starts empty

### Set Building Data
Inputs:
- BuildingData (FBuildingData)

Flow:
1. Set Building Icon
   - Get BuildingData.MenuIcon
   - Set Brush of BuildingIcon Image

2. Set Name
   - Get BuildingData.DisplayName
   - Set Text of BuildingName TextBlock

3. Clear Costs
   - Clear Children of CostsContainer

4. Create Cost Entries   ```
   ForEach loop on BuildingData.ResourceCosts Map:
   - Create Widget (CostEntryClass)
   - Cast to WBP_ResourceCostEntry
   - Set Resource Tag (Key)
   - Set Amount (Value)
   - Add to CostsContainer   ```

### Button Clicked
1. Get BuildingData
2. Get Parent Widget (WBP_BuildingMenu)
3. Call Parent's OnBuildingSelected with BuildingData row name