# FirstLine RTS Game Implementation Documentation

## Core Systems Overview

### Command System
**Status: Partially Implemented**
- Uses GAS (Gameplay Ability System) for command execution
- Commands defined in Data Table (DT_Commands)
- Dynamic command resolution based on unit selection and context
- Command data includes:
  - Command tags
  - Required unit tags
  - Associated gameplay ability

**Implementation Details:**
- `UCommandSystemComponent`: Handles command resolution and execution
- Commands are cached for performance
- Uses gameplay tags for flexible command identification
- Integrated with GAS for ability activation

### Resource System
**Status: Partially Implemented**
- Resources managed through GAS attributes
- Resource types defined by gameplay tags
- Resource gathering uses command system
- Resources stored in CommanderPlayerState

**Implementation Details:**
- Uses `UCommanderAttributeSet` for resource attributes
- Resource transfer system in development
- Resource costs defined in data tables

### Unit Management
**Status: In Progress**
- Units implement `ICommandable` interface
- Population system using GAS attributes
- Unit production through TownHall
- Units controlled via command system

**Implementation Details:**
- Peasant spawning implemented through `GA_PeriodicSpawn`
- AI controlled using behavior trees
- Units share PlayerState with owning player

### Building System
**Status: In Development**
- Building data defined in Data Table (DT_Buildings)
- Construction requires worker assignment
- Preview system for placement
- Resource cost verification

**Components:**
1. `ABuildingBase`: Base building class
2. `BuildingTargetActor`: Handles placement preview
3. Building data table for configuration
4. Construction tasks in behavior tree (planned)

## Game Areas

### Resource and Building Area
**Status: Planned**
- Dedicated to resource gathering
- Building placement restrictions
- Resource node placement

### Military Section
**Status: Planned**
- Unit training facilities
- Separate from resource area
- Offensive unit management

### Defense Line Area
**Status: Planned**
- Tower defense mechanics
- Wall placement system
- Strategic defensive positioning

## Technical Architecture

### Networking
**Status: In Progress**
- Server authoritative
- GAS for ability replication
- Command validation on server
- Building placement verification

### Input System
**Status: Partially Implemented**
- Enhanced Input System
- Context-sensitive input mapping
- Multiple input contexts (RTS, Building, etc.)

### AI System
**Status: In Progress**
- Behavior Tree for unit AI
- Task system for worker assignments
- Pathfinding integration

## Next Steps

### Priority 1: Building System
1. Complete building placement input handling
2. Implement construction progress system
3. Worker assignment mechanics
4. Resource cost verification

### Priority 2: Resource System
1. Complete resource transfer mechanics
2. Resource UI implementation
3. Resource node placement system

### Priority 3: Military System
1. Unit training mechanics
2. Military building functionality
3. Combat system implementation

## Design Decisions

### Why GAS?
- Flexible ability system
- Built-in networking
- Attribute management
- Tag-based system fits RTS design

### Data-Driven Approach
- Data Tables for configuration
- Gameplay Tags for identification
- Blueprint extensibility where needed
- C++ for core systems

### Blueprint vs C++ Split
**C++:**
- Core systems
- Interfaces
- Base classes
- Performance-critical code

**Blueprint:**
- UI implementation
- Visual feedback
- Quick iterations
- Building-specific logic

## Implementation Details

### Building System Current Implementation
The building system uses a combination of C++ base classes and Blueprint implementations:

**C++ Components:**
- `ABuildingBase`: Base building class with construction state management
- `BuildingTargetActor`: Handles building placement preview and targeting
- Building data structures and interfaces

**Blueprint Components:**
- `GA_Build`: Gameplay ability for building placement
- Building-specific blueprints inheriting from `ABuildingBase`
- Input mapping contexts for building placement

**Data Assets:**
- `DT_Buildings`: Maps building tags to their data (class, costs, construction time)
- `DT_Commands`: Defines available commands including building commands

### Current Focus
Currently implementing the building placement system with proper input handling and preview functionality. 