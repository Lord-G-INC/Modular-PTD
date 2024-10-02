#include "CometTimerObj.h"
#include "Game/MapObj/TimeAttackClockExt.h"
/*
* Comet Timer Creator
*
* Creates a Comet Timer on switch activation.
*/

CometTimerObj::CometTimerObj(const char* pName) : LiveActor(pName) {
    mTime = 60;
    mNoKill = false;
    mNoKillAllowRepeat = false;
    mSwitchMode = false;
    mLinkedClocks = 0;
}

void CometTimerObj::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, (s32*)&mTime);
    MR::getJMapInfoArg1NoInit(rIter, &mNoKill);
    MR::getJMapInfoArg2NoInit(rIter, &mNoKillAllowRepeat);
    MR::getJMapInfoArg3NoInit(rIter, &mSwitchMode);
    MR::needStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::invalidateClipping(this);

    mLayout = new TimeLimitLayout();
    mLayout->setDisplayModeOnNormal(true);
    mLayout->initWithoutIter();
    MR::connectToSceneLayout(mLayout);

    s32 childnum = MR::getChildObjNum(rIter);

    if (childnum > 0) {
        mLinkedClocks = new TimeAttackClockExt*[childnum];

        for (s32 i = 0; i < childnum; i++) {
            mLinkedClocks[i] = new TimeAttackClockExt("TimeAttackClockC");
            MR::initChildObj(mLinkedClocks[i], rIter, i);
            mLinkedClocks[i]->mCometTimerObjPtr = this;
        }
    }

    initNerve(&NrvCometTimerObj::NrvWait::sInstance, 0);
    makeActorAppeared();
}

void CometTimerObj::control() {
    if (MR::isOnSwitchA(this) && isNerve(&NrvCometTimerObj::NrvWait::sInstance))
        setNerve(&NrvCometTimerObj::NrvStartCountDown::sInstance);

    if (MR::isValidSwitchB(this) && MR::isOnSwitchB(this)) {
        mLayout->kill();
        setNerve(&NrvCometTimerObj::NrvWait::sInstance);
    }
}

void CometTimerObj::exeStartCountDown() {
    if (MR::isFirstStep(this)) {
        mLayout->setTimeLimit(mTime*60);
        mLayout->appear();
        
        if (MR::isValidSwitchDead(this) && mSwitchMode)
            MR::onSwitchDead(this);

        setNerve(&NrvCometTimerObj::NrvCountDown::sInstance);
    }
}

void CometTimerObj::exeCountDown() {
    if (mLayout->isReadyToTimeUp())
        setNerve(&NrvCometTimerObj::NrvTimeUp::sInstance);
}

void CometTimerObj::exeTimeUp() {
    if (MR::isFirstStep(this)) {
        if (!mNoKill) {
            MR::forceKillPlayerByGroundRace();
        }
        else {
            MR::startSystemSE("SE_SY_TIMER_A_0", -1, -1);
            mLayout->kill();

            if (MR::isValidSwitchDead(this)) {
                if (mSwitchMode)
                    MR::offSwitchDead(this);
                else
                    MR::onSwitchDead(this);
            }

            if (mNoKillAllowRepeat)
                setNerve(&NrvCometTimerObj::NrvWait::sInstance);
            else 
                kill();
        }
    }
}

namespace NrvCometTimerObj {
    void NrvWait::execute(Spine* pSpine) const {};

    void NrvStartCountDown::execute(Spine* pSpine) const {
        ((CometTimerObj*)pSpine->mExecutor)->exeStartCountDown();
    };

    void NrvCountDown::execute(Spine* pSpine) const {
        ((CometTimerObj*)pSpine->mExecutor)->exeCountDown();
    };

    void NrvTimeUp::execute(Spine* pSpine) const {
        ((CometTimerObj*)pSpine->mExecutor)->exeTimeUp();
    };

    NrvWait(NrvWait::sInstance);
    NrvStartCountDown(NrvStartCountDown::sInstance);
    NrvCountDown(NrvCountDown::sInstance);
    NrvTimeUp(NrvTimeUp::sInstance);
};