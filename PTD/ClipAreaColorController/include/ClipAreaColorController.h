#pragma once

#include "syati.h"
#include "Game/Util.h"
#include "Game/LiveActor/LiveActor.h"

class ClipAreaColorController : public LiveActor {
public:
    ClipAreaColorController(const char *pName);
    virtual void init(const JMapInfoIter &rIter);

    void exeWait();
    void exeActive();

    s32 mRed;
    s32 mGreen;
    s32 mBlue;
    s32 mAlpha;
};

namespace NrvClipColor { 
    NERVE(ClipColorNrvWait);
    NERVE(ClipColorNrvActive);
}; 