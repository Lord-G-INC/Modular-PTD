#pragma once

#include "syati.h"
#include "RedCoin.h"

namespace RedCoinUtil {
    LiveActor* getSpecificActorFromGroup(LiveActor* pActor, const char* pName);
    RedCoin* tryLinkToChildRedCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter);
    bool tryAppearLinkedRedCoin(LiveActor* pSourceActor, const TVec3f& pPosition);
}