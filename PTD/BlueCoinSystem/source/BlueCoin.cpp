#include "BlueCoin.h"
#include "Game/MapObj/CoinHolder.h"
#include "BlueCoinLayouts.h"
#include "Game/Screen/GameSceneLayoutHolder.h"
#include "Game/Screen/CounterLayoutControllerExt.h"

BlueCoin::BlueCoin(const char* pName) : Coin(pName) {
    mID = 0;
    mLaunchVelocity = 25.0f;
    mIsCollected = 0;
    
    MR::createCoinRotater();
    MR::createCoinHolder();
    MR::addToCoinHolder(this, this);
}

void BlueCoin::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg0NoInit(rIter, &mID);
        MR::getJMapInfoArg1NoInit(rIter, &mLaunchVelocity);

        Coin::setShadowAndPoseModeFromJMapIter(rIter);
        Coin::initShadow(rIter);

        MR::processInitFunction(this, rIter, BlueCoinUtil::isBlueCoinGotCurrentFile(mID) ? "BlueCoinClear" : "BlueCoin", false);
    }
    else {
        MR::processInitFunction(this, BlueCoinUtil::isBlueCoinGotCurrentFile(mID) ? "BlueCoinClear" : "BlueCoin", false);
    }

    MR::initSensors(this, "BlueCoin", 0);

    mFlashingCtrl = new FlashingCtrl(this, 1);
    
    initEffectKeeper(2, "Coin", 0);
    initNerve(&NrvCoin::CoinNrvFix::sInstance, 0);

    if (mIsInBubble)
        initAirBubble();

    MR::tryCreateMirrorActor(this, "BlueCoin");
    MR::initShadowVolumeSphere(this, 50.0f);
    MR::setShadowDropPositionPtr(this, 0, &mShadowDropPos);
    MR::setShadowDropLength(this, 0, 1000.0f);

    makeActorAppeared();

    if (MR::isValidInfo(rIter) && MR::isValidSwitchB(this) || MR::useStageSwitchSyncAppear(this, rIter))
        requestHide();
    else
        appearFixInit();
}

void BlueCoin::control() {
    if (MR::isOnSwitchB(this) && MR::isHiddenModel(this) && !mIsCollected)
        appearAndMove();

    if (MR::isOnSwitchB(this))
        mLifeTime = 0x7FFFFFFF;
}

bool BlueCoin::receiveMessage(u32 msg, HitSensor* pSender, HitSensor* pReciver) {
    if (MR::isMsgItemGet(msg)) {
        collect();
        return true;
    }

    return false;
}

void BlueCoin::initAirBubble() {
    mAirBubble = MR::createPartsModelNoSilhouettedMapObj(this, "空気アワ", "AirBubble", 0);
    mAirBubble->initFixedPosition(TVec3f(0.0f, 70.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), 0);
    MR::startAction(mAirBubble, "Move");
    MR::setSensorRadius(this, "BlueCoin", 100.0f);
}

void BlueCoin::appearAndMove() {
    TVec3f gravityVec;
    MR::calcGravityVector(this, mTranslation, &gravityVec, 0, 0);
    gravityVec.scale(-mLaunchVelocity);
    appearMove(mTranslation, gravityVec, 0x7FFFFFFF, -1);

    MR::startActionSound(this, "SyBlueCoinAppear", -1, -1, -1);
}

void BlueCoin::collect() {
    mIsCollected = true;
    setNerve(&NrvCoin::CoinNrvGot::sInstance);
    
    if (MR::isValidSwitchA(this))
        MR::onSwitchA(this);

    if (mIsInBubble) {
        MR::emitEffect(mAirBubble, "RecoveryBubbleBreak");
        mAirBubble->kill();
    }
    
    #if defined SMG63 
        MR::emitEffect(this, "BlueCoinGet"); 
    #else
        MR::emitEffect(this, BlueCoinUtil::isBlueCoinGotCurrentFile(mID) ? "BlueCoinClearGet" : "BlueCoinGet"); 
    #endif

    MR::startActionSound(this, "SyBlueCoin", -1, -1, -1);
    
    if (!BlueCoinUtil::isBlueCoinGotCurrentFile(mID)) {
        BlueCoinUtil::setBlueCoinGotCurrentFile(mID);

        if (!BlueCoinUtil::hasSeenBlueCoinTextBoxCurrentFile())
            ((CounterLayoutControllerExt*)MR::getGameSceneLayoutHolder()->mCounterLayoutController)->mBlueCoinCounter->setNerve(&NrvBlueCoinCounter::NrvShowTextBox::sInstance);
    }
    
    if (!MR::isGalaxyDarkCometAppearInCurrentStage())
        MR::incCoin(1, this);

    MR::incPlayerOxygen(mIsInBubble ? 2 : 1);
    makeActorDead();
}