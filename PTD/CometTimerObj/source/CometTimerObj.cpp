#include "CometTimerObj.h"

/*
* Comet Timer Creator
*
* Creates a Comet Timer on switch activation.
*
* Work in progress. Exclusive to PTD.
*/

CometTimerObj::CometTimerObj(const char* pName) : NameObj(pName) {
    mTime = 60;
    mKillPlayer = false;
    mIsAppeared = false;
    mStageSwitchCtrl = 0;
}

void CometTimerObj::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &mTime);
    MR::getJMapInfoArg1NoInit(rIter, &mKillPlayer);

    mStageSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);

    mLayout = new TimeLimitLayout();
    mLayout->setDisplayModeOnNormal(true);
    mLayout->initWithoutIter();
    MR::connectToSceneLayout(mLayout);
}

void CometTimerObj::movement() {
    if (mStageSwitchCtrl->isValidSwitchA() && mStageSwitchCtrl->isOnSwitchA() && !mIsAppeared) {
        if (MR::isHiddenLayout(mLayout))
            MR::showLayout(mLayout);
            
        mLayout->setTimeLimit(mTime * 60);
        mLayout->appear();
        mIsAppeared = true;
    }
    
    if (mLayout->isReadyToTimeUp() && mIsAppeared)
        onTimeUp();

    if (mStageSwitchCtrl->isOnSwitchB()) {
        mIsAppeared = false;
        MR::hideLayout(mLayout);
    }
}

void CometTimerObj::onTimeUp() {
    if (mKillPlayer)
        MR::forceKillPlayerByGroundRace();
    else {
        if (mStageSwitchCtrl->isValidSwitchDead())
            mStageSwitchCtrl->onSwitchDead();

        MR::startSystemSE("SE_SY_TIMER_A_0", -1, -1);
        mIsAppeared = false;
        MR::hideLayout(mLayout);
    }
    
}
