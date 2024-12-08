#pragma once

#include "syati.h"

namespace RedCoinUtil {
    extern RedCoin* tryLinkToChildRedCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter);
    extern bool tryAppearLinkedRedCoin(LiveActor* pSourceActor, const TVec3f& pPosition);
};

namespace BlueCoinUtil {
    extern bool tryCreateBlueCoinForSpawningActorActionKeeper(LiveActor* pSourceActor, s32 id);
    extern void appearBlueCoinActionKeeper(LiveActor* pSourceActor, TVec3f& rPos);
};