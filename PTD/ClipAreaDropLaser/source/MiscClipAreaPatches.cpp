#include "syati.h"

void ClipAreaMovableReadSwitchAppear(LiveActor* pActor, const JMapInfoIter& rIter) {
    MR::addBaseMatrixFollowTarget(pActor, rIter, 0, 0);

    if (!MR::useStageSwitchSyncAppear(pActor, rIter))
        pActor->makeActorAppeared();
}

kmCall(0x8028672C, ClipAreaMovableReadSwitchAppear);
kmWrite32(0x80286730, 0x48000014); // b 0x14