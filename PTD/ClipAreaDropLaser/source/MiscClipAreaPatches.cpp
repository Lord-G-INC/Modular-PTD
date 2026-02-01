#include "syati.h"

void ClipAreaMovableReadSwitchAppear(LiveActor* pActor, const JMapInfoIter& rIter) {
    MR::addBaseMatrixFollowTarget(pActor, rIter, 0, 0);

    if (!MR::useStageSwitchSyncAppear(pActor, rIter))
        pActor->makeActorAppeared();
}

kmCall(0x8028672C, ClipAreaMovableReadSwitchAppear);
kmWrite32(0x80286730, PPC_B(0x14)); // b 0x14