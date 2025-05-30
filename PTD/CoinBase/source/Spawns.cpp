#include "syati.h"
#include "Game/Enemy/Kuribo.h"
#include "Game/Enemy/SamboHead.h"
#include "Game/Enemy/Teresa.h"
#include "Game/LiveActor/ExtActorActionKeeper.h"
#include "Spawns.h"

// KURIBO

void KuriboSetUpCustomCoin(Kuribo* pKuribo, const JMapInfoIter& rIter, const char* pStr) {
    MR::processInitFunction(pKuribo, rIter, pStr, 0);

   s32 coinArg = 0;
   s32 coinType = 0;
   MR::getJMapInfoArg2NoInit(rIter, &coinArg);
   MR::getJMapInfoArg3NoInit(rIter, &coinType);
   CoinBaseSpawnUtil::createCustomCoin(pKuribo, rIter, coinArg, coinType);
}

kmCall(0x801B8A68, KuriboSetUpCustomCoin);

void KuriboAppearCustomCoin(Kuribo* pKuribo) {
    pKuribo->LiveActor::makeActorDead();
    
    CoinBaseSpawnUtil::appearCustomCoin(pKuribo, pKuribo->mTranslation);
}

kmBranch(0x801B8E70, KuriboAppearCustomCoin);

// SAMBOHEAD


void SamboHeadSetUpCustomCoin(SamboHead* pSamboHead, const JMapInfoIter& rIter, const char* pStr) {
    MR::processInitFunction(pSamboHead, rIter, pStr, 0);

    #ifdef BLUECOINSYSTEM
        s32 blueCoinArg = -1;
        MR::getJMapInfoArg1NoInit(rIter, &blueCoinArg);

        if (blueCoinArg > -1)
            BlueCoinUtil::tryCreateBlueCoinForSpawningActorActionKeeper(pSamboHead, rIter, blueCoinArg);
    #endif

    #ifdef REDCOINSYSTEM
        s32 redCoinArg = -1;
        MR::getJMapInfoArg2NoInit(rIter, &redCoinArg);

        if (redCoinArg > -1)
            RedCoinUtil::tryLinkToChildRedCoin(pSamboHead, rIter, 0);
    #endif
}

kmCall(0x801F8024, SamboHeadSetUpCustomCoin);

void SamboHeadAppearCustomCoin(SamboHead* pSamboHead, const char* pStr) {
    MR::emitEffect(pSamboHead, pStr);

    #ifdef BLUECOINSYSTEM    
        BlueCoinUtil::tryAppearBlueCoinActionKeeper(pSamboHead, pSamboHead->mTranslation);
    #endif

    #ifdef REDCOINSYSTEM
        RedCoinUtil::tryAppearLinkedRedCoin(pSamboHead, pSamboHead->mTranslation);
    #endif  
}

kmCall(0x801F81D4, SamboHeadAppearCustomCoin);

// TERESA


s32 TeresaSetUpCustomCoin(const JMapInfoIter& rIter, Teresa* pTeresa) {
    s32 modelId = MR::getDummyDisplayModelId(rIter, -1);

    s32 arg = -1;
    bool coinCreated = false;
    const char* pDisplayModelName = pTeresa->mDummyDisplayModel->mName;

    #ifdef BLUECOINSYSTEM
        if (MR::isEqualString(pDisplayModelName, "BlueCoin") || MR::isEqualString(pDisplayModelName, "BlueCoinClear")) {
            MR::getJMapInfoArg6NoInit(rIter, &arg);
            coinCreated = BlueCoinUtil::tryCreateBlueCoinForSpawningActorActionKeeper(pTeresa, rIter, arg);
        }
    #endif

    #ifdef REDCOINSYSTEM
        if (MR::isEqualString(pDisplayModelName, "RedCoin"))
            coinCreated = RedCoinUtil::tryLinkToChildRedCoin(pTeresa, rIter, 0);
    #endif

    if (coinCreated)
        pTeresa->_10C = 0;
    
    return modelId;
}

kmWrite32(0x8020C15C, 0x7F84E378); // mr r4, r28
kmCall(0x8020C160, TeresaSetUpCustomCoin);

void TeresaAppearCustomCoin(Teresa* pTeresa) {
    if (pTeresa->_10C)
        MR::appearCoinPop(pTeresa, pTeresa->mTranslation, 1);

    #ifdef BLUECOINSYSTEM    
        BlueCoinUtil::tryAppearBlueCoinActionKeeper(pTeresa, pTeresa->mTranslation);
    #endif

    #ifdef REDCOINSYSTEM
        RedCoinUtil::tryAppearLinkedRedCoin(pTeresa, pTeresa->mTranslation);
    #endif  
}

kmWrite32(0x8020C394, 0x7FE3FB78); // mr r3, r31
kmCall(0x8020C398, TeresaAppearCustomCoin);
kmWrite32(0x8020C39C, 0x48000014); // b 0x14

namespace CoinBaseSpawnUtil {
    CoinBase* createCustomCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter, s32 arg, s32 type) {
        CoinCreateInfo* pInfo = &cCoinCreateInfo[type];

        for (s32 i = 0; i < cNumNewCoinCreateInfo; i++) {
            if (type == cCoinCreateInfo[i].type) {
                ((ExtActorActionKeeper*)pSourceActor->mActionKeeper)->mType = pInfo->type; 
                return (pInfo->mCreateFunc)(pSourceActor, rIter, arg);
                break;
            }
        }
        return 0;
    }
    
    bool appearCustomCoin(LiveActor* pSourceActor, const TVec3f& rPos) {
        ExtActorActionKeeper* pKeeper = (ExtActorActionKeeper*)pSourceActor->mActionKeeper;
        s32 type = pKeeper->mType;
        
        for (s32 i = 0; i < cNumNewCoinCreateInfo; i++) {
            if (type == cCoinAppearInfo[i].type) {
                CoinAppearInfo* pInfo = &cCoinAppearInfo[i];
                ((ExtActorActionKeeper*)pSourceActor->mActionKeeper)->mType = pInfo->type; 
                return (pInfo->mAppearFunc)(pSourceActor, rPos);
                break;
            }
        }
        return false;
    }
}