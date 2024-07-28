#include "RedCoin.h"
#include "RedCoinUtil.h"
#include "Game/MapObj/CoinHolder.h"

/*
* These actors are exclusive to PT Debug.
* RedCoin, RedCoinController
*
* A long awaited project that I finally decided to scrape together.
*
* A coin type from other Mario games that activates an event once all
* red coins (usually 8) are collected. Typically a star would spawn.
*
* Some parts of this actor are initialized through ActorInfo.
*
* Must be linked to a RedCoinController through Group ID.
*
* Credits:
* Evanbowl, Lord-Giganticus, Galaxy Master, and Aurum for helping me with crash fixes.
*/

/* --- RED COIN --- */
RedCoin::RedCoin(const char* pName) : Coin(pName) {
    mIsCollected = false;
    mLaunchVelocity = 250.0f;
    mInvalidateShadows = false;
    mHasRewardedCoins = false;
    mRedCoinCounterPlayerPos = false;

    // Setup Coin
    MR::createCoinRotater();
    MR::createCoinHolder();
    MR::addToCoinHolder(this, this);
}

void RedCoin::init(const JMapInfoIter& rIter) {
    MR::processInitFunction(this, rIter, false);
    MR::joinToGroupArray(this, rIter, "RedCoinGroup", 24);

    MR::getJMapInfoArg0NoInit(rIter, &mLaunchVelocity);
    MR::getJMapInfoArg1NoInit(rIter, &mRedCoinCounterPlayerPos);

    Coin::setShadowAndPoseModeFromJMapIter(rIter);
    Coin::initShadow(rIter);

    MR::calcGravity(this);
    MR::invalidateClipping(this);

    initHitSensor(1);
    MR::addHitSensor(this, "RedCoin", 0x4A, 4, 55.0f, TVec3f(0.0f, 70.0f, 0.0f));
    mFlashingCtrl = new FlashingCtrl(this, 1);

    initNerve(&NrvCoin::CoinNrvFix::sInstance, 0);

    if (!MR::isValidSwitchB(this)) {
        initAirBubble();
    }
    makeActorAppeared();

    // Can't use ActorInfo for this one...
    if (MR::isValidInfo(rIter))
        MR::useStageSwitchSyncAppear(this, rIter);
}

void RedCoin::initAfterPlacement() {
    if (MR::isValidSwitchB(this) && !mIsInBubble) {
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
    }
    else {
        MR::offBind(this);
    }

    if (mInvalidateShadows)
        MR::invalidateShadowAll(this);

    Coin::initAfterPlacement();
}


void RedCoin::control() {
    if (MR::isOnSwitchB(this) && MR::isHiddenModel(this) && !mIsCollected) {
        appearAndMove();
    }

    if (MR::isOnSwitchB(this))
        mLifeTime = 0x7FFFFFFF;

    if (mIsCollected)
        MR::zeroVelocity(this);
}

void RedCoin::calcAndSetBaseMtx() {
    Coin::calcAndSetBaseMtx();
}

bool RedCoin::receiveMessage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
	if (MR::isMsgItemGet(msg) && !mIsCollected) {
		collect();
        return true;
    }
    
	return false;
}

void RedCoin::initAirBubble() {
    if (mIsInBubble) {
        mAirBubble = MR::createPartsModelNoSilhouettedMapObj(this, "空気アワ", "AirBubble", 0);
        mAirBubble->initFixedPosition(TVec3f(0.0f, 70.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), 0);
        MR::startAction(mAirBubble, "Move");
        MR::setSensorRadius(this, "RedCoin", 100.0f);
    }
}

void RedCoin::appearAndMove() {
    // I need a better way to calculate the gravity
    TVec3f coinVelocity = TVec3f(0.0f, mLaunchVelocity / 10.0f, 0.0f);
    coinVelocity.scale(coinVelocity.y, -mGravity);
    
    appearMove(mTranslation, coinVelocity, 300, 30);
    MR::startSystemSE("SE_SY_RED_COIN_APPEAR", -1, -1);
}

void RedCoin::collect() {
    mIsCollected = true;
    setNerve(&NrvCoin::CoinNrvGot::sInstance);
    
    RedCoinController* pController = (RedCoinController*)RedCoinUtil::getSpecificActorFromGroup(this, "RedCoinController");
    
    if (MR::isValidSwitchA(this))
        MR::onSwitchA(this);
    
    if (mIsInBubble) {
        MR::emitEffect(mAirBubble, "RecoveryBubbleBreak");
        mAirBubble->kill();
    }
    
    // Only ever increment coins once.
    if (!mHasRewardedCoins && !MR::isGalaxyDarkCometAppearInCurrentStage()) {
        MR::incPlayerLife(1);
        GameSequenceFunction::getPlayResultInStageHolder()->addCoinNum(!pController->mRewardCoins ? 2 : 0);
        mHasRewardedCoins = true;
    }

    pController->startCountUp(this);

    MR::startSystemSE(pController->mHasAllRedCoins ? "SE_SY_RED_COIN_COMPLETE" : "SE_SY_RED_COIN", -1, -1);

    MR::incPlayerOxygen(mIsInBubble ? 2 : 1);
    MR::invalidateHitSensors(this);
    MR::invalidateShadowAll(this);
    MR::emitEffect(this, "RedCoinGet");
    MR::hideModel(this);
}