#pragma once

#include "syati.h"

namespace BlueCoinUtil {
    extern bool isBlueCoinGotCurrentFile(u8);
};

namespace pt {
    extern void* loadArcAndFile(const char *pArc, const char *pFile, JKRHeap *pHeap);
};

class ExtDummyDisplayModel : public DummyDisplayModel {
public:
    ExtDummyDisplayModel(LiveActor *pHost, const DummyDisplayModelInfo *pInfo, int i, s32 l1, s32 l2);

    s32 mSpinMode;
};