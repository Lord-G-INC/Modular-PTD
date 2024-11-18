#include "syati.h"

struct StageInstrumentEntry {
    const char* pName;
    const char* pStage;
    u8 _8[4];
};

namespace pt {
    extern void* loadArcAndFile(const char *pArc, const char *pFile, JKRHeap *pHeap);
};
