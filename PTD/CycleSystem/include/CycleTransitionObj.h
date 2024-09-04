#pragma once

#include "syati.h"

class CycleTransitionObj : public LiveActor {
public:
    CycleTransitionObj(const char *pName);

    virtual void init(const JMapInfoIter &rIter);

    void activate();
    void deactivate();

};