# Construction Monitor Service Blueprint

## Parent Class
BTService_BlueprintBase

## Blackboard Keys Used
- TargetBuilding (Object)
- DefaultBehaviorTree (Object)

## Event Graph
### On Service Start
1. Get target building from blackboard
2. Bind to OnConstructionCompleted event

### On Construction Completed
1. Get owning controller
2. Clear blackboard value 'TargetBuilding'
3. Run behavior tree asset stored in 'DefaultBehaviorTree'

### On Service End
1. Unbind from building events 