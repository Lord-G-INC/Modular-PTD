#pragma once

#include "syati.h"

namespace RedCoinUtil {
    LiveActor* getSpecificActorFromGroup(LiveActor* pActor, const char* pName);
    bool tryLinkToChildRedCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter);
    bool tryAppearLinkedRedCoin(LiveActor* pSourceActor, const TVec3f& pPosition);
}