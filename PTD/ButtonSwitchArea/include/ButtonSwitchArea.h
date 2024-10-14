#pragma once

#include "syati.h"
#include "Game/System/WPadHolder.h"
#include "Game/System/WPadStick.h"
#include "Game/System/WPadPointer.h"

class ButtonSwitchArea : public AreaObj {
public:
    ButtonSwitchArea(const char *pName);
    virtual void init(const JMapInfoIter &);
    virtual void movement();
    bool isDisabled();

    s32 mControllerNum;
    s32 mButtonMode;
    s32 mButtonCheckNum;
    s32 mExtraMode;
    s32 mCheckXCoord;
    s32 mCheckYCoord;
    s32 mRoundPrecision;
    WPad *mWPad;
};