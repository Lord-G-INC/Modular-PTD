#pragma once 

#include "syati.h"
#include "Game/Screen/TimeLimitLayout.h"

class CometTimerObj : public LiveActor {
public:
    CometTimerObj(const char* pName);
    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    void onTimeUp();

    TimeLimitLayout* mLayout;
    s32 mTime;
    bool mKillPlayer;
};
