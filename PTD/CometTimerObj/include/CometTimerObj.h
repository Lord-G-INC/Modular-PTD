#pragma once 

#include "syati.h"
#include "Game/Screen/TimeLimitLayout.h"
#include "Game/Screen/BombTimerLayout.h"

class TimeAttackClockExt;

class CometTimerObj : public LiveActor {
public:
    CometTimerObj(const char* pName);
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    void exeStartCountDown();
    void exeCountDown();
    void exeTimeUp();
    void addTime(u32 time);

    TimeLimitLayout* mTimeLimitLayout;
    BombTimerLayout* mBombTimerLayout;
    TimeAttackClockExt** mLinkedClocks;
    s32 mLayoutType;
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