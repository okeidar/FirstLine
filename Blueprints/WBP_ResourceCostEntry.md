# Resource Cost Entry Widget Blueprint

## Parent Class
UserWidget

## Description
Small widget showing a resource icon and amount, used in building entry cost display.

## Designer View
### Root
- Horizontal Box "RootBox"

### Layout
- Image "ResourceIcon"
  * Size: 16x16
  * Horizontal Box Settings:
    - Size: Auto
    - Padding: 2,0,2,0
    - HorizontalAlignment: Left
    - VerticalAlignment: Center

- TextBlock "AmountText"
  * Horizontal Box Settings:
    - Size: Auto
    - Padding: 0,0,2,0
    - HorizontalAlignment: Left
    - VerticalAlignment: Center
  * Text Settings:
    - Font: Roboto 10
    - Color: White
    - Min Desired Width: 20
    - Justification: Left

## Variables
### Input
- ResourceTag (FGameplayTag)
  * Category: "Resource"
  * BlueprintReadWrite
  * ExposeOnSpawn

- Amount (float)
  * Category: "Resource"
  * BlueprintReadWrite
  * ExposeOnSpawn

### Internal
- ResourceDataTable (DataTable)
  * Category: "Resource"
  * EditDefaultsOnly
  * Description: "Data table containing resource icons"

## Event Graph

### Event Construct
Flow:
1. Get Resource Icon
   - Get Row from ResourceDataTable using ResourceTag as name
   - Get Icon from resource data row
   - Set Image Brush of ResourceIcon

2. Set Amount Text
   - Convert Amount to int (floor)
   - Convert to string
   - Set Text of AmountText

### PreConstruct
Flow:
1. Setup default values for designer preview
   - Set dummy icon
   - Set "100" as preview text