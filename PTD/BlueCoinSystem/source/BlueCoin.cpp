#include "BlueCoin.h"
#include "Game/MapObj/CoinHolder.h"

BlueCoin::BlueCoin(const char* pName) : Coin(pName) {
    mID = 0;
    mLaunchVelocity = 250.0f;
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
        MR::setGroupClipping(this, rIter, 64);
    }
    else
        MR::processInitFunction(this, BlueCoinUtil::isBlueCoinGotCurrentFile(mID) ? "BlueCoinClear" : "BlueCoin", false);

    initEffectKeeper(2, "Coin", 0);
    MR::calcGravity(this);

    initNerve(&NrvCoin::CoinNrvFix::sInstance, 0);
    initHitSensor(1);

    #ifdef SMG63
        MR::addHitSensor(this, "BlueCoin", 0x4A, 4, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));
    #else
        MR::addHitSensor(this, "BlueCoin", 0x4A, 4, 55.0f, TVec3f(0.0f, 70.0f, 0.0f));
    #endif

    MR::initShadowVolumeSphere(this, 50.0f);
    MR::setShadowDropPositionPtr(this, 0, &mShadowDropPos);
    MR::setShadowDropLength(this, 0, 1000.0f);

    mFlashingCtrl = new FlashingCtrl(this, 1);

    makeActorAppeared();

    if (MR::isValidInfo(rIter))
        MR::useStageSwitchSyncAppear(this, rIter);
}

void BlueCoin::initAfterPlacement() {
    if (MR::isValidSwitchB(this)) {
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
    }
    else
        MR::offBind(this);

    Coin::initAfterPlacement();
}

void BlueCoin::kill() {
    if (!mIsCollected)
        OSReport("Blue Coin %d died without being collected.\n", mID);

    LiveActor::kill();
}

void BlueCoin::control() {
    if (MR::isOnSwitchB(this) && MR::isHiddenModel(this))
        appearAndMove();
}

void BlueCoin::calcAndSetBaseMtx() {
    Coin::calcAndSetBaseMtx();
}

bool BlueCoin::receiveMessage(u32 msg, HitSensor* pSender, HitSensor* pReciver) {
    if (MR::isMsgItemGet(msg)) {
        collect();
        return true;
    }

    return false;
}

void BlueCoin::appearAndMove() {
    TVec3f coinVelocity = TVec3f(0.0f, mLaunchVelocity / 10.0f, 0.0f);
    coinVelocity.scale(coinVelocity.y, -mGravity);

    MR::startSystemSE("SE_SY_PURPLE_COIN_APPEAR", -1, -1);
    
    appearMove(mTranslation, coinVelocity, 0x7FFFFFFF, 60);
}

void BlueCoin::collect() {
    mIsCollected = true;
    
    if (MR::isValidSwitchA(this))
        MR::onSwitchA(this);

    MR::emitEffect(this, "BlueCoinGet"); 

    #if defined SMG63 
        MR::emitEffect(this, "BlueCoinGet"); 
        MR::startSystemSE("SE_SY_TICO_COIN", -1, -1);
    #else
        MR::emitEffect(this, BlueCoinUtil::isBlueCoinGotCurrentFile(mID) ? "BlueCoinClearGet" : "BlueCoinGet"); 
        MR::startSystemSE("SE_SY_PURPLE_COIN", -1, -1);     
    #endif
    

    if (!BlueCoinUtil::isBlueCoinGotCurrentFile(mID)) {
        BlueCoinUtil::setBlueCoinGotCurrentFile(mID);
        BlueCoinUtil::startCounterCountUp();
    }
    
    if (!MR::isGalaxyDarkCometAppearInCurrentStage()) {
        MR::incCoin(1, this);
    }

    makeActorDead();
}