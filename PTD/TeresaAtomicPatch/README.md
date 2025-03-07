# TeresaAtomic Patch

Unhardcodes the amount of child Boos a TeresaAtomic can have.

## Added obj_args

Other obj_args are the same as in the vanilla game.

|obj_arg|Name|Type|Default|Description|
|-------|----|----|-------|-----------|
|obj_arg2|Number of child Boos|int|4|The amount of child Boos that TeresaAtomic should have. Without making any other changes, specifying any number above 4 will crash the game.|

## Having more than 4 child Boos
*A video guide is available [here](https://youtu.be/xs4kj9pUgXs?t=02m05s).*

To have more than 4 child Boos, you need to add a position for each new child Boo. Here is how:
1. Open the `TeresaAtomic.arc` file and open the ActorInfo folder in it.
2. You will see 4 `FollowerX.bcsv` files. Add a new bcsv by simply duplicating the original file.
3. Open the new bcsv in a BCSV editor and specify your desired joint name, offset and rotation. If you're confused what joints it is referring to, you can find them in `TeresaAtomicPosition.arc` -> `TeresaAtomicPosition.bdl`.
4. Afterwards, open `InitSensor.bcsv` in the same folder and add a new entry for `FollowerX` (with `X` being the number of your new child Boo). It should be layed out just like the other Follower entries:
|SensorName|SensorType|Radius|HitSensorNumMax|JointName|Callback|OffsetX|OffsetY|OffsetZ|[528AC841]|[4DB4F9C4]|
|----------|----------|------|---------------|---------|--------|-------|-------|-------|----------|----------|
|FollowerX|Enemy|200.0|8|||0.0|0.0|0.0|o||

5. Save the files and you are good to go!