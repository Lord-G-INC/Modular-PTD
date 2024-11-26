#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Util/PlayerUtil.h"
#include "Game/LiveActor.h"
#include "syati.h"

class LanternClipArea : public LiveActor {
public:
    LanternClipArea(const char *pName);
    virtual void init(const JMapInfoIter &rIter);
    virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);

    void exeWait(); 
    void exeStartClipArea();

    f32 mRadius;
    s32 mOpening;
    s32 mStay;
    s32 mClosure;
    s32 mTotal;
};

namespace NrvLantern { 
    NERVE(LanternNrvWait);
    NERVE(LanternNrvStart);
}; 
