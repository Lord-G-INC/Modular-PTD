# SimpleBreakableObjExt

Allows for the creation of custom SimpleBreakableObjs.

To register them, simply add a new entry into the ProductMapObjDataTable with the Class name being `SimpleBreakableObj`.

After that you can decide what break conditions your new object should have.

## Break Conditions
The game checks for multiple different conditions in a bitfield. If one of them is met, the object breaks and releases the item(s).

There are 16 different bits, of which bits 2 - 11 are used for conditions:

| Bit | Condition              |
|-----|------------------------|
| #2  | Spin Attack            |
| #3  | Ground Pound           |
| #4  | Explosion Attack       |
| #5  | JetTurtle Attack       |
| #6  | Fire Ball Attack       |
| #7  | Enemy Attack           |
| #8  | Tamakoro Dash (Wall)   |
| #9  | Tamakoro Dash (Ground) |
| #10 | Drill Attack           |
| #11 | Rock Attack            |

To make your own custom break conditions, make a binary string with 16 zeros:

`0000000000000000`

Then, replace the 0 representing your desired condition with a 1:

`0000001000001010`

In this case, the BreakableObj can be broken if you Spin Attack it (bit #2), explode it (bit #4) or drill attack it (bit #10).

Now convert the binary into a decimal number and use it as the value for **obj_arg4** on your object:

`522`