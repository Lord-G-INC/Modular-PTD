#include "PurpleCoinAppearSpot.h"
#include "PurpleCoinHolderExt.h"
PurpleCoinAppearSpot::PurpleCoinAppearSpot(const char* pName) : LiveActor(pName) {
    //mCoins = 0;
    mMtx.identity();
    mNumCoins = 0;
    mCoinLifeTime = 0x7FFFFFFF;
    mSpeed = 4.0f;
}

void PurpleCoinAppearSpot::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::makeMtxTR((MtxPtr)&mMtx, this);
    MR::connectToSceneMapObjMovement(this);
    MR::setGroupClipping(this, rIter, 8);
    MR::setClippingTypeSphere(this, 100.0f);
    MR::getJMapInfoArg0NoInit(rIter, &mNumCoins);
    MR::getJMapInfoArg1NoInit(rIter, &mCoinLifeTime);
    MR::getJMapInfoArg2NoInit(rIter, &mSpeed);

    PurpleCoinUtil::declareCoin(this, 5);

    //mCoins = new Coin*[mNumCoins];
//
    //for (int i = 0; i < mNumCoins; i++) {
    //    mCoins[i] = MR::createPurpleCoin(this, "PurpleCoinS");
    //    mCoins[i]->initWithoutIter();
    //    mCoins[i]->kill();
    //}

    MR::useStageSwitchSyncAppear(this, rIter);
    makeActorDead();
}

void PurpleCoinAppearSpot::appear() {
    LiveActor::appear();
    PurpleCoinUtil::appearCoinPop(this, mTranslation, 5);
    //for (int i = 0; i < mNumCoins; i++) {
    //    TVec3f grav;
    //    MR::calcGravityVector(this, mTranslation, &grav, 0, 0);
    //    grav.negate();
    //    grav.scale(25.0f);
//
    //    if (mNumCoins > 1)
    //        MR::addRandomVector(&grav, grav, mSpeed);
//
    //    mCoins[i]->appearMove(mTranslation, grav, mCoinLifeTime, 60);
    //}

    MR::startSystemSE("SE_SY_PURPLE_COIN_APPEAR", -1, -1);
}

MtxPtr PurpleCoinAppearSpot::getBaseMtx() {
    return (MtxPtr)&mMtx;
}

PurpleCoinAppearSpot::~PurpleCoinAppearSpot() {
    //delete mCoins;
}