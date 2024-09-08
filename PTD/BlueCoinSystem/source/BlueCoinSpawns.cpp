#include "syati.h"
#include "BlueCoin.h"
#include "Game/Enemy/Kuribo.h"
#include "Game/Enemy/SamboHead.h"
#include "Game/Enemy/Teresa.h"
#include "Game/LiveActor/ExtActorActionKeeper.h"

// ACTORACTIONKEEPER EXTENSION

ExtActorActionKeeper::ExtActorActionKeeper(LiveActor *pActor, ActorAnimKeeper *pAnimKeeper, ActionFlagCtrl *pFlagCtrl, ActorPadAndCameraCtrl *pPadAndCameraCtrl,
    ActorHitReactionCtrl *pHitReactionCtrl, ActorItemGenerator *pItemGenerator, ActorParam *pActorParam, ActionScreenBlurCtrl *pScreenBlurCtrl,
    ActionFootPrintCtrl *pFootPrintCtrl, ActionBgmCtrl *pBgmCtrl) : 
    ActorActionKeeper(pActor, pAnimKeeper, pFlagCtrl, pPadAndCameraCtrl, pHitReactionCtrl, pItemGenerator, pActorParam, pScreenBlurCtrl, pFootPrintCtrl, pBgmCtrl) {
    
    mBlueCoin = 0;
}

ExtActorActionKeeper* createExtActorActionKeeper(s32 l, LiveActor *pActor, ActorAnimKeeper *pAnimKeeper, ActionFlagCtrl *pFlagCtrl, ActorPadAndCameraCtrl *pPadAndCameraCtrl,
    ActorHitReactionCtrl *pHitReactionCtrl, ActorItemGenerator *pItemGenerator, ActorParam *pActorParam, ActionScreenBlurCtrl *pScreenBlurCtrl,
    ActionFootPrintCtrl *pFootPrintCtrl, ActionBgmCtrl *pBgmCtrl) {

    return new ExtActorActionKeeper(pActor, pAnimKeeper, pFlagCtrl, pPadAndCameraCtrl, pHitReactionCtrl, pItemGenerator, pActorParam, pScreenBlurCtrl, pFootPrintCtrl, pBgmCtrl);
}

kmWrite32(0x8022A9AC, 0x48000010); // b 0x10
kmCall(0x8022A9E4, createExtActorActionKeeper);

// KURIBO

void KuriboSetUpBlueCoin(Kuribo* pKuribo, const JMapInfoIter& rIter, const char* pStr) {
    s32 arg = -1;
    if (MR::isValidInfo(rIter))
        MR::getJMapInfoArg2NoInit(rIter, &arg);
        
    MR::processInitFunction(pKuribo, rIter, pStr, 0);

    BlueCoinUtil::tryCreateBlueCoinForSpawningActorActionKeeper(pKuribo, arg);

}

kmCall(0x801B8A68, KuriboSetUpBlueCoin);

void KuriboAppearBlueCoin(Kuribo* pKuribo, const char* pStr) {
    MR::emitEffect(pKuribo, pStr);

    BlueCoinUtil::appearBlueCoinActionKeeper(pKuribo);
}

kmCall(0x801B8EC8, KuriboAppearBlueCoin);

// SAMBOHEAD


void SamboHeadSetUpBlueCoin(SamboHead* pSamboHead, const JMapInfoIter& rIter, const char* pStr) {
    s32 arg = -1;

    if (MR::isValidInfo(rIter))
        MR::getJMapInfoArg1NoInit(rIter, &arg);

    MR::processInitFunction(pSamboHead, rIter, pStr, 0);

    BlueCoinUtil::tryCreateBlueCoinForSpawningActorActionKeeper(pSamboHead, arg);
}

kmCall(0x801F8024, SamboHeadSetUpBlueCoin);

void SamboHeadAppearBlueCoin(SamboHead* pSamboHead, const char* pStr) {
    MR::emitEffect(pSamboHead, pStr);

    BlueCoinUtil::appearBlueCoinActionKeeper(pSamboHead);
}

kmCall(0x801F81D4, SamboHeadAppearBlueCoin);

// TERESA


s32 TeresaSetUpBlueCoin(const JMapInfoIter& rIter, Teresa* pTeresa) {
    s32 modelId = MR::getDummyDisplayModelId(rIter, -1);

    s32 arg = -1;
    const char* pDisplayModelName = pTeresa->mDummyDisplayModel->mName;
    if (MR::isEqualString(pDisplayModelName, "BlueCoin") || MR::isEqualString(pDisplayModelName, "BlueCoinClear"))
        MR::getJMapInfoArg6NoInit(rIter, &arg);

    bool coinCreated = BlueCoinUtil::tryCreateBlueCoinForSpawningActorActionKeeper(pTeresa, arg);
    
    if (arg > -1 && coinCreated)
        pTeresa->_10C = 0;
    
    return modelId;
}

kmWrite32(0x8020C15C, 0x7F84E378); // mr r4, r28
kmCall(0x8020C160, TeresaSetUpBlueCoin);

void TeresaAppearBlueCoin(Teresa* pTeresa) {
    if (pTeresa->_10C)
        MR::appearCoinPop(pTeresa, pTeresa->mTranslation, 1);
    else
        BlueCoinUtil::appearBlueCoinActionKeeper(pTeresa);
}

kmWrite32(0x8020C394, 0x7FE3FB78); // mr r3, r31
kmCall(0x8020C398, TeresaAppearBlueCoin);
kmWrite32(0x8020C39C, 0x48000014); // b 0x14