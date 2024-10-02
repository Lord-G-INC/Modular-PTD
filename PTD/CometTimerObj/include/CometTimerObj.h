#pragma once 

#include "syati.h"
#include "Game/Screen/TimeLimitLayout.h"

class TimeAttackClockExt;

class CometTimerObj : public LiveActor {
public:
    CometTimerObj(const char* pName);
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    void exeStartCountDown();
    void exeCountDown();
    void exeTimeUp();

    TimeLimitLayout* mLayout;
    TimeAttackClockExt** mLinkedClocks;
    u32 mTime;
    bool mNoKill;
    bool mNoKillAllowRepeat;
    bool mSwitchMode;
};

namespace NrvCometTimerObj {
    NERVE(NrvWait);
    NERVE(NrvStartCountDown);
    NERVE(NrvCountDown);
    NERVE(NrvTimeUp);
};