#include "pt/MapObj/BenefitItemMoon.h"

BenefitItemMoon::BenefitItemMoon(const char* pName): LiveActor(pName) {
    mIsCollected = false;
}

void BenefitItemMoon::init(const JMapInfoIter& rIter) {
    MR::processInitFunction(this, rIter, false);
    MR::calcGravity(this);

    initHitSensor(1);
    MR::addHitSensor(this, "BenefitItemMoon", 0x4A, 4, 55.0f, TVec3f(0.0f, 70.0f, 0.0f));

    makeActorAppeared();

    // Can't use ActorInfo for this one...
    MR::useStageSwitchSyncAppear(this, rIter);
}

void BenefitItemMoon::control() {
    if (mIsCollected) {
        mVelocity = TVec3f(0, 0, 0);
    }
}

void BenefitItemMoon::appearAndMove() {
    MR::validateHitSensors(this);
}

bool BenefitItemMoon::receiveMessage(u32 msg, HitSensor* pSender, HitSensor* pReciver) {
    if (MR::isMsgItemGet(msg) && !mIsCollected)
        collect();
        return false;
}

void BenefitItemMoon::collect() {
    mIsCollected = true;
    MR::startSystemSE("SE_SY_5UP", -1, -1);
    MR::getGameSceneLayoutHolder()->requestOneUp(3);
    GameDataFunction::addPlayerLeft(3);
    MR::invalidateHitSensors(this);
    MR::invalidateShadowAll(this);
    MR::emitEffect(this, "MoonGet");
    MR::hideModel(this);
}