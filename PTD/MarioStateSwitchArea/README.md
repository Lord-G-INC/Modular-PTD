# MarioStateSwitchArea

A custom area that enables its SW_A when Mario enters it and is in a given state. Either the Status Check Code or the Bitfield Check Code have to match to turn the switch on.

## obj_args

|obj_arg|Name|Type|Default|Description|
|-------|----|----|-------|-----------|
|obj_arg0|Status Check Code|int|(none)|The state to check for. A definition of the Mario states can be found [here]().|
|obj_arg1|Bitfield Check Code|int|(none)|The Mario->_C bitfield value to check for. It's recommended you use debug mode to figure out what value to use.|
|obj_arg2|Keep switch on?|bool|false|Whether SW_A should be kept on after being enabled once.|
|obj_arg3|Use Debug Mode?|bool|false|If true, it will OSReport the current bitfield value every frame that Mario is inside the area. This allows you to pick out any value to your liking.|

## Switches

|Switch|Type|Behavior|
|------|----|--------|
|SW_A|Write|Turns on if the player is inside and if either check matches.|