# Peasant Controller Blueprint

## Variables
- BuildingMenuClass (TSubclassOf<WBP_BuildingMenu>)
- BuildingMenuInstance (WBP_BuildingMenu reference)

## Event Graph
### On Peasant Selected
1. If BuildingMenuInstance exists, remove it
2. Create BuildingMenuInstance from BuildingMenuClass
3. Add to viewport
4. Pass peasant and building data table to PopulateMenu

### On Peasant Deselected
1. If BuildingMenuInstance exists:
   - Remove from viewport
   - Clear reference

### Input Events
1. On Right Mouse Button:
   - If BuildingMenuInstance exists:
     * Remove from viewport
     * Clear reference

2. On Unit Selection Changed:
   - If no units selected and BuildingMenuInstance exists:
     * Remove from viewport
     * Clear reference 