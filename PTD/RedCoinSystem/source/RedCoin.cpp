#include "RedCoin.h"
#include "RedCoinUtil.h"
#include "Game/MapObj/CoinHolder.h"

RedCoin::RedCoin(const char* pName) : Coin(pName) {
    mIsCollected = false;
    mLaunchVelocity = 25.0f;
    mHasRewardedCoins = false;
    mRedCoinCounterPlayerPos = false;

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

    mFlashingCtrl = new FlashingCtrl(this, 1);

    initNerve(&NrvCoin::CoinNrvFix::sInstance, 0);

    if (mIsInBubble)
        initAirBubble();

    MR::tryCreateMirrorActor(this, "RedCoin");
    makeActorAppeared();

    if (MR::isValidSwitchB(this) || MR::useStageSwitchSyncAppear(this, rIter))
        requestHide();
    else
        appearFixInit();
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

bool RedCoin::receiveMessage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
	if (MR::isMsgItemGet(msg) && !mIsCollected) {
		collect();
        return true;
    }
    
	return false;
}

void RedCoin::initAirBubble() {
    mAirBubble = MR::createPartsModelNoSilhouettedMapObj(this, "空気アワ", "AirBubble", 0);
    mAirBubble->initFixedPosition(TVec3f(0.0f, 70.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), 0);
    MR::startAction(mAirBubble, "Move");
    MR::setSensorRadius(this, "RedCoin", 100.0f);
}

void RedCoin::appearAndMove() {
    TVec3f gravityVec;
    MR::calcGravityVector(this, mTranslation, &gravityVec, 0, 0);
    gravityVec.scale(mLaunchVelocity);
    gravityVec.negate();
    appearMove(mTranslation, gravityVec, 0x7FFFFFFF, -1);

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
    
    if (!mHasRewardedCoins && !MR::isGalaxyDarkCometAppearInCurrentStage()) {
        MR::incPlayerLife(1);
        GameSequenceFunction::getPlayResultInStageHolder()->addCoinNum(!pController->mRewardCoins ? 2 : 0);
        mHasRewardedCoins = true;
    }

    pController->startCountUp(this);

    MR::startActionSound(this, pController->mHasAllRedCoins ? "SyRedCoinComplete" : "SyRedCoin", -1, -1, -1);

    MR::incPlayerOxygen(mIsInBubble ? 2 : 1);
    MR::invalidateHitSensors(this);
    MR::invalidateShadowAll(this);
    MR::emitEffect(this, "RedCoinGet");
    MR::hideModel(this);
}