#pragma once 

#include "syati.h"
#include "Game/Screen/TimeLimitLayout.h"

class CometTimerObj : public LiveActor {
public:
    CometTimerObj(const char* pName);
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    void exeStartCountDown();
    void exeCountDown();
    void exeTimeUp();

    TimeLimitLayout* mLayout;
    s32 mTime;
    bool mNoKill;
};

namespace NrvCometTimerObj {
    NERVE(NrvWait);
    NERVE(NrvStartCountDown);
    NERVE(NrvCountDown);
    NERVE(NrvTimeUp);
};