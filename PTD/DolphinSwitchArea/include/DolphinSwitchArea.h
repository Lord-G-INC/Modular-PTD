#pragma once

#include "syati.h"

class DolphinSwitchArea : public AreaObj {
public:
    DolphinSwitchArea(const char *pName);
    ~DolphinSwitchArea();
    virtual void init(const JMapInfoIter &rIter);
    virtual void movement();
};

namespace pt {
    extern s32 getIOSVersion();
}