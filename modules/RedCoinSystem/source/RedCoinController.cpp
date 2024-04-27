#include "RedCoinController.h"
#include "RedCoinLayouts.h"
//#include "pt/MapObj/BlueCoinSystem/BlueCoinUtil.h"
#include "RedCoinUtil.h"

/* --- RED COIN CONTROLLER --- */

/*
    Manages the collected Red Coins, calculated through an Actor Group.
    Activates an event once all linked Red Coins, usually 8, are collected.

    Obj_args:
    0: [Bool] Do not increment the coin counter by 2
    1: [Int] Power Star Indicator: -1 to disable
    2: [Int] Picture Font Icon ID Entry

    Created by Evanbowl
*/

RedCoinController::RedCoinController(const char* pName) : LiveActor(pName) {
    mNumCoins = 0;
    mElapsed = 0;
    mHasAllRedCoins = false;
    mRewardCoins = false;
    mIsValidCounterAppear = false;
    mRedCoinCounter = 0;
    mRedCoinPlayerCounter = 0;
    mRedCoinSwitch = 0;
    mLinkedCoins = 0;
}

void RedCoinController::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    MR::registerDemoSimpleCastAll(this);
    MR::useStageSwitchWriteA(this, rIter);
    MR::joinToGroupArray(this, rIter, "RedCoinGroup", 24);

    // Get Obj_args
    s32 powerStarCheck = 0;
    s32 iconID = 0x37;
    s32 layoutMode = -1;
    MR::getJMapInfoArg0NoInit(rIter, &mRewardCoins); // Should the Red Coin increment the coin counter by 2?
    MR::getJMapInfoArg1NoInit(rIter, &powerStarCheck); // Power Star to check for to set the collected star indicator
    MR::getJMapInfoArg2NoInit(rIter, &iconID); // PictureFont.brfnt entry to display
    MR::getJMapInfoArg3NoInit(rIter, &layoutMode);

    // Initialize the RedCoinCounter
    mRedCoinCounter = new RedCoinCounter("RedCoinCounter");
    mRedCoinCounter->initWithoutIter();
    mRedCoinCounter->setStarIcon(powerStarCheck, iconID);

    // Initialize RedCoinCounterPlayer
    mRedCoinPlayerCounter = new RedCoinCounterPlayer("RedCoinCounterPlayer");
    mRedCoinPlayerCounter->initWithoutIter();
    
    if (layoutMode == 0) {
        mRedCoinCounter->mLayoutMode = 0;
        mRedCoinCounter->appear();
    }
}

void RedCoinController::initAfterPlacement() {
    mRedCoinSwitch = (RedCoinSwitch*)RedCoinUtil::getSpecificActorFromGroup(this, "RedCoinSwitch");
    LiveActorGroup* group = MR::getGroupFromArray(this);

    for (s32 i = 0; i < group->mNumObjs; i++) {
        if (MR::isEqualString(group->getActor(i)->mName, "RedCoin")) {
            mLinkedCoins++;

            if (mRedCoinSwitch) {
                MR::hideModel(group->getActor(i));
                MR::invalidateShadowAll(group->getActor(i));
                MR::invalidateHitSensors(group->getActor(i));
            }
        }
    }
}

void RedCoinController::movement() {    
    calcCounterVisibility();

    if (mHasAllRedCoins)
        mElapsed++; // There may be a better way to do this, maybe nerves?

    if (mElapsed == 1 && mRedCoinSwitch) {
        mRedCoinSwitch->mTimeLimitLayout->kill();
    }
    if (mElapsed == 140) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }
        
        LiveActorGroup* group = MR::getGroupFromArray(this);
        for (s32 i = 0; i < group->mNumObjs; i++) {
            if (!MR::isEqualString(group->getActor(i)->mName, "RedCoinSwitch"))
                group->getActor(i)->kill();
        }
    }
}

// Increases both layouts by 1
void RedCoinController::startCountUp(LiveActor* pRedCoin) {
    mNumCoins++;
    
    mHasAllRedCoins = mNumCoins == mLinkedCoins;

    mRedCoinCounter->startCountUp(mNumCoins, mHasAllRedCoins, mRedCoinSwitch);

    mRedCoinPlayerCounter->mLastRedCoin = pRedCoin;
    mRedCoinPlayerCounter->mNumCoins = mNumCoins;
    mRedCoinPlayerCounter->appear();
}

namespace BlueCoinUtil {
    extern bool isBlueCoinTextBoxAppeared();
}
void RedCoinController::calcCounterVisibility() {
    bool blueCoin = false;

        blueCoin = BlueCoinUtil::isBlueCoinTextBoxAppeared();

        if (blueCoin) {
            requestResume();
            mRedCoinCounter->requestResume();
        }

    if (MR::isPowerStarGetDemoActive() || MR::isDemoActive() || MR::isPlayerDead() || MR::isTimeKeepDemoActive() || MR::isNormalTalking() || MR::isSystemTalking() || blueCoin)
        MR::hideLayout(mRedCoinCounter);
    else
        MR::showLayout(mRedCoinCounter);
}

void RedCoinController::appearFromSwitch() {
    mRedCoinCounter->appear();
    LiveActorGroup* group = MR::getGroupFromArray(this);

    for (s32 i = 0; i < group->mNumObjs; i++) {
        if (MR::isEqualString(group->getActor(i)->mName, "RedCoin")) {
            MR::showModel(group->getActor(i));
            MR::validateShadowAll(group->getActor(i));
            MR::validateHitSensors(group->getActor(i));
        }
    }
}

void RedCoinController::resetAllRedCoins() {
    LiveActorGroup* group = MR::getGroupFromArray(this);

    for (s32 i = 0; i < group->mNumObjs; i++) {
        if (MR::isEqualString(group->getActor(i)->mName, "RedCoin")) {
            RedCoin* coin = ((RedCoin*)group->getActor(i));
            MR::hideModel(coin);
            MR::invalidateShadowAll(coin);
            MR::invalidateHitSensors(coin);

            coin->mIsCollected = false;

            if (coin->mIsInBubble)
                coin->mAirBubble->makeActorAppeared();
        }
    }

    mNumCoins = 0;
    mIsValidCounterAppear = false;
    MR::stopAnim(mRedCoinPlayerCounter, 0);
    MR::startSystemSE("SE_SY_TIMER_A_0", -1, -1);
    mRedCoinCounter->kill();
    MR::setTextBoxNumberRecursive(mRedCoinCounter, "Counter", 0);
}