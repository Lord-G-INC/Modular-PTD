#pragma once

#include "syati.h"

#include "CoinBase.h"
namespace RedCoinUtil {
    extern CoinBase* tryLinkToChildRedCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter, s32 arg);
    extern bool tryAppearLinkedRedCoin(LiveActor* pSourceActor, const TVec3f& pPosition);
};

namespace BlueCoinUtil {
    extern CoinBase* tryCreateBlueCoinForSpawningActorActionKeeper(LiveActor* pSourceActor, const JMapInfoIter& rIter, s32 id);
    extern bool tryAppearBlueCoinActionKeeper(LiveActor* pSourceActor, const TVec3f& rPosition);
};

// LiveActor*, const JMapInfoIter&, int arg

// LiveActor*, TVec3f& rPos

struct CoinCreateInfo {
    CoinBase* (*mCreateFunc)(LiveActor* pSourceActor, const JMapInfoIter& rIter, s32 arg);
    s32 type;
};

struct CoinAppearInfo {
    bool (*mAppearFunc)(LiveActor* pSourceActor, const TVec3f &rPos);
    s32 type;
};

CoinCreateInfo cCoinCreateInfo[2] = {
    {RedCoinUtil::tryLinkToChildRedCoin, 0 },
    {BlueCoinUtil::tryCreateBlueCoinForSpawningActorActionKeeper, 1 }
};

const size_t cNumNewCoinCreateInfo = sizeof(cCoinCreateInfo)/sizeof(cCoinCreateInfo[0]);

CoinAppearInfo cCoinAppearInfo[2] = {
    {RedCoinUtil::tryAppearLinkedRedCoin, 0},
    {BlueCoinUtil::tryAppearBlueCoinActionKeeper, 1 }
};

namespace CoinBaseSpawnUtil {
    CoinBase* createCustomCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter, s32 arg, s32 type);
    bool appearCustomCoin(LiveActor* pSourceActor, const TVec3f& rPos);
};