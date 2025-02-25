#include "JumpStandAutoNeo.h"

JumpStandAutoNeo::JumpStandAutoNeo (const char *pName) : JumpStandAuto(pName) {
    mLaunchForce = 30.0f;
    mBtpColorFrame = 7; // Vanilla
    mRailSpeed = 10.0f;
    mStopDuringExtend = false;
    mTevColor = J3DGXColor();
    mTevColor.a = 255;
}

JumpStandAutoNeo::~JumpStandAutoNeo () {

}

void JumpStandAutoNeo::init (const JMapInfoIter &rIter) {
    MR::processInitFunction(this, rIter, true);
    MR::initDLMakerMatColor0(this, "JumpStandAuto00_v", &mTevColor);
    MR::newDifferedDLBuffer(this);
    initNerve(&NrvJumpStandAutoNeo::NrvWait::sInstance, 0);
    MR::calcGravity(this);
    makeActorAppeared();
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);

    MR::getJMapInfoArg0NoInit(rIter, &mLaunchForce);
    mHipDropLaunchForce = mLaunchForce + 5.0f;
    MR::getJMapInfoArg1NoInit(rIter, &mHipDropLaunchForce);
    MR::getJMapInfoArg2NoInit(rIter, &mBtpColorFrame);
    MR::getJMapInfoArg3NoInit(rIter, &mRailSpeed);
    MR::getJMapInfoArg4NoInit(rIter, &mStopDuringExtend);
    MR::startBtpAndSetFrameAndStop(this, "ColorChange", mBtpColorFrame);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        MR::moveCoordAndTransToNearestRailPos(this);
        mRailRider->setSpeed(mRailSpeed);
    }
}

void JumpStandAutoNeo::attackSensor (HitSensor *pReceiver, HitSensor *pSender) {
    if (MR::isSensorPlayer(pSender) && MR::isEqualSensor(pReceiver, this, "Body") && isPlayerJumpable()) {
        if (MR::isPlayerHipDropLand()) 
            jumpPlayer(mHipDropLaunchForce);
        else 
            jumpPlayer(mLaunchForce);
        setNerve(&NrvJumpStandAutoNeo::NrvExtend::sInstance);
    }

    if (MR::isEqualSensor(pReceiver, this, "YoshiPush") && MR::isSensorYoshi(pSender) && isNerve(&NrvJumpStandAutoNeo::NrvExtend::sInstance)) 
        MR::sendMsgPush(pSender, pReceiver);
}

bool JumpStandAutoNeo::isPlayerJumpable () {
    MarioActor *pMarioActor = MarioAccess::getPlayerActor();
    TVec3f playerUp, myUp;
    MR::getPlayerUpVec(&playerUp);
    MR::calcUpVec(&myUp, this);
    return (MR::isOnGroundPlayer() && MR::isNearAngleDegree(playerUp, myUp, 45))
        || (isPlayerWallTouch() && MR::isNearAngleDegree(playerUp, myUp, 90 + 45));
}

bool JumpStandAutoNeo::isPlayerWallTouch () {
    return (MarioAccess::getPlayerActor()->mMario->_C >> (31 - 8)) & 1;
}

void JumpStandAutoNeo::jumpPlayer (f32 force) {
    TVec3f myUp;
    MR::calcUpVec(&myUp, this);
    myUp.scale(force);
    MR::startBckPlayerJ("ƒWƒƒƒ“ƒvB");
    MR::forceJumpPlayer(myUp);
}

void JumpStandAutoNeo::moveRail () {
    mRailRider->move();
    MR::moveTransToCurrentRailPos(this);
    if (mRailRider->isReachedGoal()) 
        mRailRider->reverse();
}

void JumpStandAutoNeo::exeWait () {
    if (MR::isExistRail(this)) 
        moveRail();
    if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) 
        setNerve(&NrvJumpStandAutoNeo::NrvDisabled::sInstance);
}

void JumpStandAutoNeo::exeExtend () {
    if (MR::isExistRail(this) && !mStopDuringExtend) 
        moveRail();
    if (MR::isFirstStep(this)) {
        MR::startLevelSoundPlayerOrYoshi("JumpStandAuto", "JumpStandAuto", -1);
        MR::startAction(this, "Extend");
        if (MR::isValidSwitchB(this)) 
            MR::onSwitchB(this);
    }
    MR::setNerveAtBckStopped(this, &NrvJumpStandAutoNeo::NrvWait::sInstance);
}

void JumpStandAutoNeo::exeDisabled () {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Disable");
        MR::startBtpAndSetFrameAndStop(this, "ColorChange", 8); // Disabled (Gray)
        MR::invalidateHitSensors(this);
    }
    
    if (MR::isOnSwitchA(this)) {
        MR::startBck(this, "Enable", 0);
        MR::startBtpAndSetFrameAndStop(this, "ColorChange", mBtpColorFrame);
        MR::validateHitSensors(this);
        setNerve(&NrvJumpStandAutoNeo::NrvWait::sInstance);
    }
}

namespace NrvJumpStandAutoNeo {
    void NrvExtend::execute (Spine *pSpine) const {
        JumpStandAutoNeo *pActor = (JumpStandAutoNeo *)pSpine->mExecutor;
        pActor->exeExtend();
    }
    void NrvWait::execute (Spine *pSpine) const {
        JumpStandAutoNeo *pActor = (JumpStandAutoNeo *)pSpine->mExecutor;
        pActor->exeWait();
    }
    void NrvDisabled::execute (Spine *pSpine) const {
        JumpStandAutoNeo *pActor = (JumpStandAutoNeo *)pSpine->mExecutor;
        pActor->exeDisabled();
    }

    NrvExtend(NrvExtend::sInstance);
    NrvWait(NrvWait::sInstance);
    NrvDisabled(NrvDisabled::sInstance);
}