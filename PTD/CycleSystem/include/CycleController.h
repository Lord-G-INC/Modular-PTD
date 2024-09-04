#pragma once

#include "syati.h"
#include "CycleObj.h"
#include "CycleTransitionObj.h"

class CycleController : public LiveActor {
public:
    CycleController(const char *pName);

    virtual void init(const JMapInfoIter &rIter);
    virtual void initAfterPlacement();
    virtual void movement();

    void syncSwitchOnA();
    void syncSwitchOnB();

    void startTransition();
    void endTransition();
    void toNextCycleObj();

    CycleObj *findCycleObj(s32 index);

    s32 cycleChangeDelay;
    s32 cycleTransitionTime;

    bool mActive;

    s32 mTimer;
    s32 mTransitionTimer;
    s32 mCurrentCycleObj;
    s32 mCycleObjCount;
};