# Building Menu Widget Blueprint

## Parent Class
UserWidget

## Components Hierarchy
- Canvas Panel (Root)
  - Horizontal Box (anchored to bottom, 150 height)
    - Scroll Box
      - Horizontal Box (for building entries)

## Variables
- BuildingDataTable (DataTable reference)
- SelectedPeasant (Actor reference)

## Functions
### PopulateMenu
Inputs:
- InPeasant: Actor
- InBuildingTable: DataTable

Logic:
1. Store peasant reference
2. Clear existing entries
3. Get all rows from building table
4. For each building data:
   - Create building entry button (WBP_BuildingEntry)
   - Set button data (icon, name, costs)
   - Add to horizontal box
   - Bind button click to OnBuildingSelected

### OnBuildingSelected
Inputs:
- RowName: Name

Logic:
1. Get building data from table
2. Call GA_BuildCommand on peasant with building row name
3. Hide menu

## Notes
- Menu should not block mouse input on game viewport
- Should close when right-clicking anywhere
- Should close when peasant is deselected 