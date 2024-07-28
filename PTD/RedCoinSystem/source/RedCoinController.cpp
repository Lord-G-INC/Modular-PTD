#include "RedCoinController.h"
#include "RedCoinLayouts.h"
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
    mRedCoinCounter = NULL;
    mRedCoinPlayerCounter = NULL;
    mRedCoinSwitch = NULL;
    mLinkedCoins = 0;
}

void RedCoinController::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::registerDemoSimpleCastAll(this);
    MR::invalidateClipping(this);
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

    initNerve(&NrvRedCoinController::NrvWait::sInstance, 1);

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

    makeActorAppeared();
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

void RedCoinController::control() {    
    setCounterVisibility();
}

void RedCoinController::allCoinsCollectedDemo() {
    if (MR::isFirstStep(this)) {
        mRedCoinCounter->setNerve(&NrvRedCoinCounter::NrvComplete::sInstance);
        MR::tryStartDemo(this, "RedCoinDemo");
    }

    if (MR::isStep(this, 150)) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }
    
        LiveActorGroup* group = MR::getGroupFromArray(this);
        for (s32 i = 0; i < group->mNumObjs; i++) {
            if (MR::isEqualString(group->getActor(i)->mName, "RedCoin"))
                group->getActor(i)->kill();
        }
        MR::endDemo(this, "RedCoinDemo");
    }
}
// Increases both layouts by 1
void RedCoinController::startCountUp(LiveActor* pRedCoin) {
    mNumCoins++;
    
    mHasAllRedCoins = mNumCoins == mLinkedCoins;

    mRedCoinCounter->startCountUp(mNumCoins, mRedCoinSwitch);

    mRedCoinPlayerCounter->mLastRedCoin = pRedCoin;
    mRedCoinPlayerCounter->mNumCoins = mNumCoins;
    mRedCoinPlayerCounter->appear();

    if (mHasAllRedCoins)
        setNerve(&NrvRedCoinController::NrvAllRedCoinsDemo::sInstance);
}


void RedCoinController::setCounterVisibility() {
    bool layoutVisibility = true;

    if (MR::isStageStateScenarioOpeningCamera() || MR::isBeginScenarioStarter())
        layoutVisibility = false;

    if (MR::isPowerStarGetDemoActive())
        layoutVisibility = false;

    if (MR::isDemoActive() && !MR::isDemoActive("RedCoinDemo"))
        layoutVisibility = false;

    if (MR::isPlayerDead())
        layoutVisibility = false;

    if (MR::isTimeKeepDemoActive() && !MR::isDemoActive("RedCoinDemo"))
        layoutVisibility = false;

    if (MR::isSystemTalking() || MR::isNormalTalking())
        layoutVisibility = false;

    if (layoutVisibility)
        MR::showLayout(mRedCoinCounter);
    else
        MR::hideLayout(mRedCoinCounter);
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

namespace NrvRedCoinController {
    void NrvWait::execute(Spine* pSpine) const {}

    void NrvAllRedCoinsDemo::execute(Spine* pSpine) const {
        ((RedCoinController*)pSpine->mExecutor)->allCoinsCollectedDemo();
    }

    NrvWait(NrvWait::sInstance);
    NrvAllRedCoinsDemo(NrvAllRedCoinsDemo::sInstance);
};