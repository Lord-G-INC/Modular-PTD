#include "syati.h"

void setBehaviorSimpleBreakableObj (SimpleBreakableObj *pActor, const JMapInfoIter &rIter) {
    pActor->MapObjActor::init(rIter);
    s32 behavior;
    MR::getJMapInfoArg4NoInit(rIter, &behavior);
    pActor->_E0 |= (u16)behavior;
}

kmCall(0x802FD744, setBehaviorSimpleBreakableObj);
kmWrite32(0x802FD7A4, PPC_B(0x1D4)); // b loc_802FD978