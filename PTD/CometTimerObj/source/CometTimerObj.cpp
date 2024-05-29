#include "CometTimerObj.h"

/*
* Comet Timer Creator
*
* Creates a Comet Timer on switch activation.
*/

CometTimerObj::CometTimerObj(const char* pName) : LiveActor(pName) {
    mTime = 60;
    mKillPlayer = true;
}

void CometTimerObj::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mTime);
    MR::getJMapInfoArg1NoInit(rIter, &mKillPlayer);
    MR::needStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);

    mLayout = new TimeLimitLayout();
    mLayout->setDisplayModeOnNormal(true);
    mLayout->initWithoutIter();
    MR::connectToSceneLayout(mLayout);
    makeActorAppeared();
}

void CometTimerObj::movement() {
    if (!MR::isDead(mLayout)) {
        if (MR::isOnSwitchA(this)) {
            mLayout->setTimeLimit(mTime * 60);
            mLayout->appear();
        }

        if (mLayout->isReadyToTimeUp())
            onTimeUp();
    }

    if (MR::isValidSwitchB(this) && MR::isOnSwitchB(this)) {
        mLayout->kill();
    }
}

void CometTimerObj::onTimeUp() {
    if (mKillPlayer)
        MR::forceKillPlayerByGroundRace();
    else {
        if (MR::isValidSwitchDead(this))
            MR::onSwitchDead(this);

        MR::startSystemSE("SE_SY_TIMER_A_0", -1, -1);
    }

    mLayout->kill();
}