#include "syati.h"
#include "BlueCoinSystem/BlueCoin.h"
#include "Game/Enemy/Kuribo.h"
#include "Game/Enemy/SamboHead.h"
#include "Game/Enemy/Teresa.h"
#include "Game/LiveActor/ExtActorActionKeeper.h"
#include "CustomCoinSpawns.h"

// ACTORACTIONKEEPER EXTENSION

// KURIBO

void KuriboSetUpBlueCoin(Kuribo* pKuribo, const JMapInfoIter& rIter, const char* pStr) {
    MR::processInitFunction(pKuribo, rIter, pStr, 0);
    
    #ifdef BLUECOINSYSTEM
        s32 blueCoinArg = -1;
        if (MR::isValidInfo(rIter))
            MR::getJMapInfoArg2NoInit(rIter, &blueCoinArg);

        if (blueCoinArg > -1)
            BlueCoinUtil::tryCreateBlueCoinForSpawningActorActionKeeper(pKuribo, blueCoinArg);
    #endif
    
    #ifdef REDCOINSYSTEM
        s32 redCoinArg = -1;
        if (MR::isValidInfo(rIter))
            MR::getJMapInfoArg3NoInit(rIter, &redCoinArg);

        if (redCoinArg > -1)
            RedCoinUtil::linkToChildRedCoin(pKuribo, rIter);
    #endif
}

kmCall(0x801B8A68, KuriboSetUpBlueCoin);

void KuriboAppearBlueCoin(Kuribo* pKuribo, const char* pStr) {
    MR::emitEffect(pKuribo, pStr);

    #ifdef BLUECOINSYSTEM
        BlueCoinUtil::appearBlueCoinActionKeeper(pKuribo);
    #endif

    #ifdef REDCOINSYSTEM
        RedCoinUtil::appearLinkedRedCoin(pKuribo, pKuribo->mTranslation);
    #endif
}

kmCall(0x801B8EC8, KuriboAppearBlueCoin);

// SAMBOHEAD


void SamboHeadSetUpBlueCoin(SamboHead* pSamboHead, const JMapInfoIter& rIter, const char* pStr) {
    MR::processInitFunction(pSamboHead, rIter, pStr, 0);

    #ifdef BLUECOINSYSTEM
        s32 blueCoinArg = -1;
        MR::getJMapInfoArg1NoInit(rIter, &blueCoinArg);

        if (blueCoinArg > -1)
            BlueCoinUtil::tryCreateBlueCoinForSpawningActorActionKeeper(pSamboHead, blueCoinArg);
    #endif

    #ifdef REDCOINSYSTEM
        s32 redCoinArg = -1;
        MR::getJMapInfoArg2NoInit(rIter, &redCoinArg);

        if (redCoinArg > -1)
            RedCoinUtil::linkToChildRedCoin(pSamboHead, rIter);
    #endif
}

kmCall(0x801F8024, SamboHeadSetUpBlueCoin);

void SamboHeadAppearBlueCoin(SamboHead* pSamboHead, const char* pStr) {
    MR::emitEffect(pSamboHead, pStr);

    #ifdef BLUECOINSYSTEM    
        BlueCoinUtil::appearBlueCoinActionKeeper(pSamboHead);
    #endif

    #ifdef REDCOINSYSTEM
        RedCoinUtil::appearLinkedRedCoin(pSamboHead, pSamboHead->mTranslation);
    #endif  
}

kmCall(0x801F81D4, SamboHeadAppearBlueCoin);

// TERESA


s32 TeresaSetUpBlueCoin(const JMapInfoIter& rIter, Teresa* pTeresa) {
    s32 modelId = MR::getDummyDisplayModelId(rIter, -1);

    s32 arg = -1;
    bool coinCreated = false;
    const char* pDisplayModelName = pTeresa->mDummyDisplayModel->mName;

    #ifdef BLUECOINSYSTEM
        if (MR::isEqualString(pDisplayModelName, "BlueCoin") || MR::isEqualString(pDisplayModelName, "BlueCoinClear")) {
            MR::getJMapInfoArg6NoInit(rIter, &arg);
            bool coinCreated = BlueCoinUtil::tryCreateBlueCoinForSpawningActorActionKeeper(pTeresa, arg);
        }
    #endif

    #ifdef REDCOINSYSTEM
        if (MR::isEqualString(pDisplayModelName, "RedCoin"))
            coinCreated = RedCoinUtil::linkToChildRedCoin(pTeresa, rIter);
    #endif

    if (coinCreated)
        pTeresa->_10C = 0;
    
    return modelId;
}

kmWrite32(0x8020C15C, 0x7F84E378); // mr r4, r28
kmCall(0x8020C160, TeresaSetUpBlueCoin);

void TeresaAppearBlueCoin(Teresa* pTeresa) {
    if (pTeresa->_10C)
        MR::appearCoinPop(pTeresa, pTeresa->mTranslation, 1);

    #ifdef BLUECOINSYSTEM    
        BlueCoinUtil::appearBlueCoinActionKeeper(pTeresa);
    #endif

    #ifdef REDCOINSYSTEM
        RedCoinUtil::appearLinkedRedCoin(pTeresa, pTeresa->mTranslation);
    #endif  
}

kmWrite32(0x8020C394, 0x7FE3FB78); // mr r3, r31
kmCall(0x8020C398, TeresaAppearBlueCoin);
kmWrite32(0x8020C39C, 0x48000014); // b 0x14