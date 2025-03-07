#pragma once

#include "syati.h"

class MarioStateSwitchArea : public AreaObj {
public:
    MarioStateSwitchArea(const char *pName);
    ~MarioStateSwitchArea();
    virtual void init(const JMapInfoIter &rIter);
    virtual void movement();

    s32 mStatusCheckCode;
    s32 mBitfieldCheck;
    bool mKeepOn;
    bool mIsDebugMode;
};