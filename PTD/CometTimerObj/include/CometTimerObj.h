#pragma once 

#include "syati.h"
#include "Game/Screen/TimeLimitLayout.h"

class CometTimerObj : public NameObj {
public:
    CometTimerObj(const char* pName);
    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    void onTimeUp();

    TimeLimitLayout* mLayout;
    StageSwitchCtrl* mStageSwitchCtrl;
    s32 mTime;
    bool mKillPlayer;
    bool mIsAppeared;
};
