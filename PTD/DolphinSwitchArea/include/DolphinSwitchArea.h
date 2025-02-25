#pragma once

#include "syati.h"

class DolphinSwitchArea : public AreaObj {
public:
    DolphinSwitchArea(const char *pName);
    ~DolphinSwitchArea();
    virtual void init(const JMapInfoIter &rIter);
    virtual void movement();
    static bool isDolphinEmulator();
};

namespace pt {
    extern s32 getIOSVersion();
}
extern "C" {
    s32 IOS_Open(const char *path, u32 flags);
    s32 IOS_Close(s32 fd);
}