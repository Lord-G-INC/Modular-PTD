#include "ButtonSwitchArea.h"

/*
* An area that can activate its SW_A based on the controller inputs.
*
* Obj_arg0: The number of the controller to check (0 or 1: Controller 1 or 2). Default value is 0.
* Obj_arg1: The button mode to use. Default value is 0.
*           0: Button mode: SW_A gets activated when the button is pressed/deactivated when it is released.
*           1: Trigger mode: SW_A gets activated on the first frame the button is pressed, then deactivated.
*           2: Release mode: SW_A gets activated on the first frame the button is released, then deactivated.
* Obj_arg2: The number of the button(s) to check. Default value is -1.
*           -1: Any button is pressed.
*           0: No button is pressed.
*           1: Left
*           2: Right
*           4: Down
*           8: Up
*           16: Plus
*           256: 2
*           512: 1
*           1024: B
*           2048: A
*           4096: Minus
*           8192: Z
*           16384: C
*           32768: Home
*           To check for multiple buttons at once, add their values together (A + B = 2048 + 1024 = 3072).
* Obj_arg3: The stick/pointer mode to use. Default value is -1.
*           -1: Any stick/pointer values work.
*           0: Check stick X and Y
*           1: Check stick X
*           2: Check stick Y
*           3: Check pointer X and Y
*           4: Check pointer X
*           5: Check pointer Y
*           6: Check stick and pointer X and Y
*           7: Check stick and pointer X
*           8: Check stick and pointer Y
* Obj_arg4: The X value of the stick/pointer to check for, will be divided by 10. Default value is 0 (0 / 10 = 0.0).
* Obj_arg5: The Y value of the stick/pointer to check for, will be divided by 10. Default value is 0 (0 / 10 = 0.0).
* Obj_arg6: The rounding precision for the stick/pointer values, will be divided by 10. Default value is 1 (1 / 10 = 0.1)
* SW_B: If on, the area will not function until the switch is turned off again.
*/

ButtonSwitchArea::ButtonSwitchArea (const char *pName) : AreaObj(pName) {
    mControllerNum = 0;
    mButtonMode = 0;
    mButtonCheckNum = -1;
    mExtraMode = -1;
    mCheckXCoord = 0;
    mCheckYCoord = 0;
    mRoundPrecision = 1;
    mWPad = 0;
    mIsBufferFrame = false;
}

void ButtonSwitchArea::init (const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);

    MR::getJMapInfoArg0NoInit(rIter, &mControllerNum);
    MR::getJMapInfoArg1NoInit(rIter, &mButtonMode);
    MR::getJMapInfoArg2NoInit(rIter, &mButtonCheckNum);
    MR::getJMapInfoArg3NoInit(rIter, &mExtraMode);
    MR::getJMapInfoArg4NoInit(rIter, &mCheckXCoord);
    MR::getJMapInfoArg5NoInit(rIter, &mCheckYCoord);
    MR::getJMapInfoArg6NoInit(rIter, &mRoundPrecision);
    
    mWPad = MR::getWPad(mControllerNum);
    if (!mWPad) 
        OSReport("ButtonSwitchArea: could not get WPad #%d!\n", mControllerNum);
}

void ButtonSwitchArea::movement () {
    if (isInVolume(*MR::getPlayerPos())) {
        f32 XCoord = mCheckXCoord / 10.0f;
        f32 YCoord = mCheckYCoord / 10.0f;
        f32 Precision = mRoundPrecision / 10.0f;

        WPadButton *pWPadButtons = mWPad->mButtons;
        bool buttonCondition;
        if (mButtonCheckNum == -1) 
            buttonCondition = true;
        else switch (mButtonMode) {
            case 0:
                buttonCondition = pWPadButtons->mButton == mButtonCheckNum;
                break;
            case 1:
                buttonCondition = pWPadButtons->mTrigger == mButtonCheckNum;
                break;
            case 2:
                buttonCondition = pWPadButtons->mRelease == mButtonCheckNum;
                break;
        }

        bool extraCondition;
        WPadStick *pWPadStick = mWPad->mStick;
        WPadPointer *pWPadPointer = mWPad->mPointer;
        TVec2f pointingPos;
        pWPadPointer->getPointingPos(&pointingPos);
        switch (mExtraMode) {
            case 0:
                extraCondition = MR::isNearZero(pWPadStick->mStickX - XCoord, Precision) && MR::isNearZero(pWPadStick->mStickY - YCoord, Precision);
                break;
            case 1:
                extraCondition = MR::isNearZero(pWPadStick->mStickX - XCoord, Precision);
                break;
            case 2:
                extraCondition = MR::isNearZero(pWPadStick->mStickY - YCoord, Precision);
                break;
            case 3:
                extraCondition = MR::isNearZero(pointingPos.x - XCoord, Precision) && MR::isNearZero(pointingPos.y - YCoord, Precision);
                break;
            case 4:
                extraCondition = MR::isNearZero(pointingPos.x - XCoord, Precision);
                break;
            case 5:
                extraCondition = MR::isNearZero(pointingPos.y - YCoord, Precision);
                break;
            case 6:
                extraCondition = MR::isNearZero(pWPadStick->mStickX - XCoord, Precision) && MR::isNearZero(pWPadStick->mStickY - YCoord, Precision) && MR::isNearZero(pointingPos.x - XCoord, Precision) && MR::isNearZero(pointingPos.y - YCoord, Precision);;
                break;
            case 7:
                extraCondition = MR::isNearZero(pWPadStick->mStickX - XCoord, Precision) && MR::isNearZero(pointingPos.x - XCoord, Precision);
                break;
            case 8:
                extraCondition = MR::isNearZero(pWPadStick->mStickY - YCoord, Precision) && MR::isNearZero(pointingPos.y - YCoord, Precision);
                break;
            default:
                extraCondition = true;
        }

        OSReport("%d;%d;%d\n", !isDisabled(), buttonCondition, extraCondition);

        if (!isDisabled() && buttonCondition && extraCondition) {
            mIsBufferFrame = true;
            onSwitchA();
        }
        else if (mIsBufferFrame) 
            mIsBufferFrame = false;
        else 
            offSwitchA();
    } else 
        offSwitchA();
}

bool ButtonSwitchArea::isDisabled () {
    return isValidSwitchB() && mStageSwitchCtrl->isOnSwitchB();
}