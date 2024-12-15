# Build Command Gameplay Ability Blueprint

## Parent Class
GameplayAbility_BuildCommand (or existing parent)

## Functions
### ActivateAbility
1. Get building data from row name
2. Check resource costs against player's resources
3. If can afford:
   - Deduct resources using gameplay effects
   - Continue with building placement
4. If cannot afford:
   - End ability
   - Show feedback (optional)

### OnBuildingPlaced
1. Spawn building
2. Setup construction
3. Assign worker 