# JumpStandAutoNeo

Overhauls the vanilla JumpStandAuto object, adding support for many things such as custom launch heights, colors, switch and rail support.

## obj_args

|obj_arg|Name|Type|Default|Description|
|-------|----|----|-------|-----------|
|obj_arg0|Launch Force|float|30.0f|The force to launch the player with. By default, this matches the original JumpStandAuto.|
|obj_arg1|Ground Pound Launch Force|float|obj_arg0 + 5.0f|The force to launch the player with when ground pounding onto the trampoline.|
|obj_arg2|Color Frame|float|7|The Color you want to use. You can add additional colors by simply editing `ColorChange.btp`. (0 = Red, 1 = Green, 2 = Blue, 3 = Yellow, 4 = Orange, 5 = Pink, 6 = Purple, 7 = Vanilla, 8 = Gray)|
|obj_arg3|Path Move Speed|float|10.0f|The Speed that the trampoline will move at if it is placed on a path.|
|obj_arg4|Stop moving when launching Player?|bool|false|Whether the trampoline should stop moving (when placed on a path) when the player is bouncing on it.|

## Switches

|Switch|Type|Behavior|
|------|----|--------|
|SW_A|Read|If turned off, the trampoline will be deactivated until the switch is turned on.|
|SW_B|Write|Turns on after the player gets launched by the trampoline.|