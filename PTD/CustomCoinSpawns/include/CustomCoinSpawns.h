#pragma once

#include "syati.h"

namespace RedCoinUtil {
    extern bool linkToChildRedCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter);
    extern void appearLinkedRedCoin(LiveActor* pActor, const TVec3f& pPosition);
};

namespace BlueCoinUtil {
    extern bool tryCreateBlueCoinForSpawningActorActionKeeper(LiveActor* pSourceActor, s32 id);
    extern void appearBlueCoinActionKeeper(LiveActor* pSourceActor);
};