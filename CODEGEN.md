# Code Generation Guide
This is a code generation guide for a code project. The code is generated in phases. each phase is listed below. The code that already exists in the context was generated in previous phases. your job is to adapt the existing code to the requirements of the phase that was instructed to you in the current prompt. with each phase, we focus on another aspect of the codebase. this document starts with general background information about the project specs, and continue to list the instructions for currently thought through phases of code generation. I will add instructions for phases as the code generation progresses.
We are developing a game in Unreal Engine 5. and as such, a design section is included to describe the game design, and every design choice should be made with the game design in mind, and should be documented in the design section.

## Overview
* The project is a game in Unreal Engine 5.
* The game is a multiplayer game.
* The game is a mix of city building and tower defense.
* The players are in control of a city and must defend it from waves of enemies.
* The waves are generated by the players and sent to their opponents.
* the city is divided by a wall.
* The players can build 3 types of buildings:
    * Towers - towers are defensive buildings that shoot at enemies and can be built in the area beyond the wall.
    * general buildings - used for resource gathering and researching upgrades. can be build inside the wall.
    * military buildings - used for training soldiers. can be build inside the wall, in a military district.
* The town hall produces peasants.
* Peasants are used for collecting resources.
* Peasants can be trained to be soldiers (preparing a wave  for the enemy), trained to be siege engineers (sent beyond the wall to build towers and defenses), or used to build buildings.

## General Coding Guidelines
* Add code only to the "Source/FirstLine" folder.
* Add code to the appropriate subfolder.
* The project is written in c++ and blueprint. maintain a proper and clever balance between the two.
* if a bluprint is to be created, write the instructions for the blueprint in a file under the "Blueprints" folder.
* when writing c++ code, consider if it should be used by a blueprint, and if so, add propper macros.
* Think about the codebase as a whole and how the current phase fits into the overall architecture.
* Keep the code clean and readable.
* Use meaningful variable and function names.
* Add comments to the code to explain why behind the code.
* use unreal loggins system to log messages to the console.

## Specific coding guidelines:
* create a propper environment using cursor ignore files to clean the codebase from noise.
* the design section should be always located last in the file.

## Code Analysis Guidelines
* Only document features that are clearly implemented in the code
* When describing mechanics, stick to what's visible in the codebase
* Avoid making assumptions about gameplay elements not yet implemented
* If a system is partially implemented, document only the implemented parts
* When in doubt, be conservative in feature description


### Phase 1:
* prepare environment and update the design section to specify what is already in the codebase. the design section should be in high level and describe the intent. such as "The player can select units by clicking on them or by marquee selection" and "The player can issue commands to selected units by right clicking on a context sensitive locations such as the ground to move them or a resource to collect from".
* create whatis defined in the specific coding guidelines 

## Design

The codebase implements a city building game with RTS-style controls:

### Unit Control
- The player can select units by clicking on them or using marquee selection
- Selected units can be commanded by right-clicking on context-sensitive locations:
  * Ground - to move units
  * Resources - to gather resources from them

### Building System
- Players can place buildings using a preview system that shows valid/invalid placement
- Buildings are constructed in stages, showing visual progress
- Peasants are assigned to construct buildings
- Construction progresses through stages: Foundation -> Framework -> Structure -> Walls -> Finishing -> Complete

### Resource Management
- Peasants can gather resources (currently implemented: wood)
- Resource gathering involves:
  * Right-clicking on resource nodes
  * Peasant collects the resource
  * Resources are tracked using the attribute system

### Town Management
- The Town Hall periodically produces new peasants
- Population is managed through a maximum cap system

















