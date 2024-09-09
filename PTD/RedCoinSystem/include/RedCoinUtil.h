#pragma once

#include "syati.h"

namespace RedCoinUtil {
    LiveActor* getSpecificActorFromGroup(LiveActor* pActor, const char* pName);
    bool linkToChildRedCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter);
    void appearLinkedRedCoin(LiveActor* pActor, const TVec3f& pPosition);
}