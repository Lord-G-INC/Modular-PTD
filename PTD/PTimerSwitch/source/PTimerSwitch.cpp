#include "PTimerSwitch.h"

PTimerSwitch::PTimerSwitch(const char* pName) : LiveActor(pName) {
    mParts = 0;
    mConnector = 0;
    mActionTimer = 0;
    mStartHit = false;
}

void PTimerSwitch::reset() {
    LiveActor::appear();
    setNerve(&NrvPTimerSwitch::PTimerSwitchNrvOff::sInstance);
    MR::validateCollisionParts(mParts);
    MR::startBck(this, "Wait", 0);
    mActionTimer = 0;
    mStartHit = false;
}

void PTimerSwitch::init(const JMapInfoIter& rIter) {
    MR::processInitFunction(this, rIter, false);
    //MR::addHitSensorMapObj(this, "body", 0x10, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    //MR::addHitSensor(this, "hit", 0x49, 0x10, 120.0f, TVec3f(0.0f, 0.0f, 50.0f));
    //MR::initCollisionParts(this, "PTimerSwitch", getSensor("body"), 0);
    mParts = MR::getCollisionParts(this);
    //mParts = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("hit"), MR::CollisionScaleType_Default);
    //initSound(1, "PTimerSwitch", &mTranslation, TVec3f(0.0f, 0.0f, 0.0f));
    //MR::needStageSwitchWriteA(this, rIter);
    initNerve(&NrvPTimerSwitch::PTimerSwitchNrvOff::sInstance, 0);
    //initEffectKeeper(0, "PTimerSwitch", false);
    MR::startBck(this, "Move", 0);
    mConnector = new MapObjConnector(this);
    appear();
}

void PTimerSwitch::initAfterPlacement() {
    mConnector->attachToUnder();
}

void PTimerSwitch::appear() {
    reset();
}

void PTimerSwitch::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "Delete");
    MR::invalidateCollisionParts(mParts);
}

void PTimerSwitch::calcAnim() {
	LiveActor::calcAnim();
	TMtx34f mtx;
	PSMTXCopy(MR::getJointMtx(this, "Move"), (MtxPtr)&mtx);
	mParts->setMtx(*(TPos3f*)&mtx);
}

void PTimerSwitch::control() {
    if (mStartHit)
        ++mActionTimer;
    else if (mActionTimer > 0)
        --mActionTimer;

    mStartHit = false;
}

void PTimerSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mConnector->connect();
}

bool PTimerSwitch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool PTimerSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isMsgPlayerTrample(msg)) 
        return false;

    if (pReceiver->mSensorType != 0x49)
        return false;

    if (!isNerve(&NrvPTimerSwitch::PTimerSwitchNrvOff::sInstance))
        return false;

    mStartHit = true;
    return true;
}

bool PTimerSwitch::trySwitchDown() {
    if (mActionTimer > 0) {
        setNerve(&NrvPTimerSwitch::PTimerSwitchNrvSwitchDown::sInstance);
        return true;
    }
    return false;
}

bool PTimerSwitch::tryOn() {
    if (MR::isBckStopped(this)) {
        setNerve(&NrvPTimerSwitch::PTimerSwitchNrvOn::sInstance);
        return true;
    }
    return false;
}

void PTimerSwitch::exeOff() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::offSwitchA(this);
    }
    
    if (!trySwitchDown());
        return;
}

void PTimerSwitch::exeSwitchDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "On", 0);
        MR::invalidateClipping(this);
    }
    
    if (!tryOn())
        return;
}

void PTimerSwitch::exeOn() {
    if (MR::isFirstStep(this)) {
        MR::onSwitchA(this);
        MR::shakeCameraNormal();
        //MR::startSound(this, "SE_OJ_SWITCH_1", -1, -1);
        MR::startActionSound(this, "OjSwitch1", -1, -1, -1);
    }

    if (MR::isGreaterStep(this, 25)) {
        kill();
    }
}

namespace NrvPTimerSwitch {
    void PTimerSwitchNrvOn::execute(Spine* pSpine) const {
        ((PTimerSwitch*)pSpine->mExecutor)->exeOn();
    }

    void PTimerSwitchNrvSwitchDown::execute(Spine* pSpine) const {
        ((PTimerSwitch*)pSpine->mExecutor)->exeSwitchDown();
    }

    void PTimerSwitchNrvOff::execute(Spine* pSpine) const {
        ((PTimerSwitch*)pSpine->mExecutor)->exeOff();
    }

    PTimerSwitchNrvOn(PTimerSwitchNrvOn::sInstance);
    PTimerSwitchNrvSwitchDown(PTimerSwitchNrvSwitchDown::sInstance);
    PTimerSwitchNrvOff(PTimerSwitchNrvOff::sInstance);
}