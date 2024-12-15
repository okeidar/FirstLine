# Phase 2 Implementation Plan

## Systems Affected
1. UI System (new building menu)
2. Command System (building selection)
3. AI System (construction behavior)
4. Resource System (cost deduction)

## Dependencies
1. Data Assets:
   - DT_ResourceUI: Data table using FResourceUIData
     * Row names should match resource gameplay tags
     * Contains icons and display names for resources
2. Building menu needs peasant selection state
3. Building placement needs resource verification
4. Construction behavior needs default behavior tree reference

## Implementation Order
1. Create Resource UI Data Table
2. Building Menu UI
3. Building Selection Flow
4. Resource Cost Handling
5. Construction Completion Behavior

## Blueprint Updates

### Required Blueprint Files:
1. WBP_BuildingMenu - Building selection interface
2. WBP_BuildingEntry - Individual building option
3. BTS_ConstructionMonitor - Handle construction completion
4. BP_PeasantController - Menu management

### Existing Blueprints to Modify:
1. GA_BuildCommand - Add resource cost check/deduction

### 1. WBP_BuildingMenu
[Previous implementation remains valid, adding:]

#### Testing
Prerequisites:
- Test map with peasant and building data table
- Resource system setup with wood

Steps:
1. Select peasant
   - Expected: Menu appears at bottom
   - Check: Doesn't block game view
2. Right click anywhere
   - Expected: Menu disappears
3. Select different unit
   - Expected: Menu disappears

### 2. WBP_BuildingEntry
[Previous implementation remains valid, adding:]

#### Testing
Prerequisites:
- Building data table with test entries
- Building icons set up

Steps:
1. Menu populated
   - Expected: Entries show correct icons/names/costs
2. Hover entry
   - Expected: Button feedback visible
3. Click entry
   - Expected: Menu closes, building placement starts

### 3. GA_BuildCommand
[Previous implementation remains valid, adding:]

#### Testing
Prerequisites:
- Commander with resources
- Building data with costs

Steps:
1. Try build with sufficient resources
   - Expected: Resources deducted, building placed
2. Try build with insufficient resources
   - Expected: Build cancelled, feedback shown

### 4. BTS_ConstructionMonitor
[Previous implementation remains valid, adding:]

#### Testing
Prerequisites:
- Test building
- Default behavior tree asset

Steps:
1. Start construction
   - Expected: Peasant builds
2. Complete construction
   - Expected: Peasant returns to default behavior
3. Cancel construction
   - Expected: Peasant returns to default behavior

## Edge Cases
1. Menu open when peasant dies
2. Building placement active when peasant dies
3. Resource deduction with exact/insufficient amounts
4. Multiple peasants selected
5. Construction interrupted by enemy (if implemented)

## Multiplayer Considerations
1. Resource deduction must be server authoritative
2. Building placement validation on server
3. Construction state replication
4. Menu is client-side only 