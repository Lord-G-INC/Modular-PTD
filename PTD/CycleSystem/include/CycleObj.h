#pragma once

#include "syati.h"

namespace pt {
    extern void* loadArcAndFile(const char *pArc, const char *pFile, JKRHeap *pHeap);
}

class CycleObj : public LiveActor {
public:
    CycleObj(const char *pName);

    virtual void init(const JMapInfoIter &rIter);

    void activate();
    void deactivate();

    const char *getBgmName();

    s32 cycleObjIndex;
    s32 stageBgmInfoIndex;
    s32 bgmChannel;
    s32 musicFadeTime;

};