#include "PurpleCoinHolderExt.h"

PurpleCoinHolderExt::PurpleCoinHolderExt(const char* pName) : CoinHolder(pName) {
    OSReport("Create\n");
    MR::createPurpleCoinCounter();
    mGroup = new LiveActorGroup("パープルコイン管理", 256);

    if (!MR::isStagePurpleCoinsMoreThan100(MR::getCurrentStageName()))
        MR::declarePowerStarCoin100();
}

void PurpleCoinHolderExt::init(const JMapInfoIter& rIter) {
    for (int i = 0; i < 32; i++) {
        Coin* pCoin = new Coin("PurpleCoinS");
        pCoin->mIsPurple = true;
        pCoin->initWithoutIter();
        registerActor(pCoin);
        mGroup->registerActor(pCoin);
        OSReport("Coin created\n");
    }
}

void PurpleCoinHolderExt::initAfterPlacement() {
    MR::validatePurpleCoinCounter();
}

PurpleCoinHolderExt* createPurpleCoinCounterExt() {
    return new PurpleCoinHolderExt("PurpleCoinHolderExt");
}

PurpleCoinHolderExt::~PurpleCoinHolderExt() {}


kmCall(0x8045A124, createPurpleCoinCounterExt);
kmWrite32(0x8045A128, 0x480007F4);

namespace PurpleCoinUtil {
    void declareCoin(const NameObj* pObj, s32 amount) {
        if (!MR::isExistSceneObj(SCENE_OBJ_PURPLE_COIN_HOLDER))
            MR::createSceneObj(SCENE_OBJ_PURPLE_COIN_HOLDER);

        PurpleCoinHolderExt* pPurpleCoinHolder = (PurpleCoinHolderExt*)MR::getSceneObjHolder()->getObj(SCENE_OBJ_PURPLE_COIN_HOLDER);
        pPurpleCoinHolder->declare(pObj, amount);
    }

    void appearCoinPop(const NameObj* pObj, const TVec3f& rVec, s32 l) {
        if (!MR::isExistSceneObj(SCENE_OBJ_PURPLE_COIN_HOLDER))
            return; 

        OSReport("Attempting coin pop\n");
        PurpleCoinHolderExt* pPurpleCoinHolder = (PurpleCoinHolderExt*)MR::getSceneObjHolder()->getObj(SCENE_OBJ_PURPLE_COIN_HOLDER);
        pPurpleCoinHolder->appearCoinPop(pObj, rVec, l);
    }

    void appearCoinPopToDirection(const NameObj* pObj, const TVec3f& rPos, const TVec3f rVel, s32 l) {
        if (!MR::isExistSceneObj(SCENE_OBJ_PURPLE_COIN_HOLDER))
            return; 

        OSReport("Attempting coin pop (direction)\n");
        PurpleCoinHolderExt* pPurpleCoinHolder = (PurpleCoinHolderExt*)MR::getSceneObjHolder()->getObj(SCENE_OBJ_PURPLE_COIN_HOLDER);
        pPurpleCoinHolder->appearCoinPopToDirection(pObj, rPos, rVel, l);
    }
}

void addToPurpleCoinHolder_redir(PurpleCoinHolderExt* pPurpleCoinHolderExt, LiveActor* pActor) {
    pPurpleCoinHolderExt->mGroup->registerActor(pActor);
}

kmCall(0x802E79C4, addToPurpleCoinHolder_redir);