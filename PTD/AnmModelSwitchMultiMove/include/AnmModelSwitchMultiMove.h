#pragma once

#include "syati.h"

class AnmModelSwitchMultiMove : public LiveActor {
public:
    AnmModelSwitchMultiMove(const char *pName);
    ~AnmModelSwitchMultiMove();
    virtual void init(const JMapInfoIter &rIter);
    virtual void control();

    const char *mObjectName;
};