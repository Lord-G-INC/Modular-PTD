#include "syati.h"
#include "WaterRiseSwitch.h"
/*
* NOTE: Due to a bug, StageEventDataTable is incompatible with WaterRiseSwitch
*
* A bipyramid-shaped switch that can raise any RailMoveWaterObj's to its vertical position.
* 
* Obj_arg0 (f32): Speed
*   -> Specifies how fast the Water object should rise/fall.
*      It's calculated in Units/Frame, the default is 1.0f.
* Obj_arg1 (f32): Y Offset
*   -> Specifies a Y offset to place the Water objects at.
*      It's calculated in Units, the default is -100.0f.
* 
* Todo:
* - Allow movement on multiple axes, not just the Y axis
* - Perhaps add a Group ID constraint so not all RailMoveWaterObj's are affected
* 
* Credit: Bavario, Alex SMG, Super Hackio
*/

LiveActorGroup *pWaterRiseGroup;

extern "C" {
    void init__11RailMoveObjFRC12JMapInfoIter(RailMoveObj *, const JMapInfoIter &);
}
void joinRailMoveWaterObjs (RailMoveObj *obj, JMapInfoIter &rIter) {
    init__11RailMoveObjFRC12JMapInfoIter(obj, rIter);
    #ifdef SMG63
    if (MR::isEqualString(obj->mName, "CGWater")) {
    #else
    if (MR::isEqualString(obj->mName, "レール移動水オブジェ")) { // RailMoveWaterObj
    #endif
        if (!pWaterRiseGroup) 
            pWaterRiseGroup = new LiveActorGroup("WaterRiseGroup", 16);
        pWaterRiseGroup->registerActor(obj);
    }
}
kmCall(0x802EA394, joinRailMoveWaterObjs);
void resetWaterRiseGroup () {
    delete pWaterRiseGroup;
    pWaterRiseGroup = NULL;
}
kmBranch(0x80451478, resetWaterRiseGroup);

namespace pt {
    WaterRiseSwitch::WaterRiseSwitch(const char *pName) : LiveActor(pName) {
        mIsRiseActive = false;
        mSpeed = 1.0f;
        mOffsetY = -350.0f;
        mCurrentBckFrame = 0.0f;
    }
    void WaterRiseSwitch::init(const JMapInfoIter &rIter) {
        MR::initDefaultPos(this, rIter);
        MR::processInitFunction(this, rIter, false);
        MR::connectToSceneMapObj(this);
        MR::invalidateClipping(this);
        MR::calcGravity(this);
        MR::getJMapInfoArg0NoInit(rIter, &mSpeed);
        MR::getJMapInfoArg1NoInit(rIter, &mOffsetY);

        initHitSensor(1);
        MR::addHitSensorMapObj(this, "WaterRise", 8, 10.0f, TVec3f(0));
        MR::validateHitSensor(getSensor("WaterRise"));
        makeActorAppeared();
    }
    void WaterRiseSwitch::attackSensor(HitSensor *pReceiver, HitSensor *pSender) {
        if (MR::isEqualString(pSender->mActor->mName, "マリオアクター")) {
            if (!mIsRiseActive) {
                MR::startSystemSE("SE_SY_READ_RIDDLE_SS", -1, -1);
                MR::startBck(this, "WaterRiseSpin", NULL);
                MR::setBckFrame(this, mCurrentBckFrame);
                exeRiseWater(pWaterRiseGroup);
            }
        }
    }
    void WaterRiseSwitch::control() {
        if (mIsRiseActive) 
            exeRiseWater(pWaterRiseGroup);
    }
    void WaterRiseSwitch::exeRiseWater(LiveActorGroup *waters) {
        s32 numWaters = waters->getLivingActorNum();
        for (int i = 0; i < numWaters; i++) {
            mIsRiseActive = true;
            f32 dist = this->mTranslation.y - waters->getActor(i)->mTranslation.y + mOffsetY;
            if (dist < mSpeed && dist > -mSpeed) 
                mIsRiseActive = false;
            else if (dist < 0.0f) 
                waters->getActor(i)->mTranslation.y -= mSpeed;
            else 
                waters->getActor(i)->mTranslation.y += mSpeed;
        }
        if (!mIsRiseActive) {
            mCurrentBckFrame = MR::getBckFrame(this);
            if (mCurrentBckFrame == 7.5f || mCurrentBckFrame == 15.0f || mCurrentBckFrame == 22.5f || mCurrentBckFrame == 0.0f) 
                ;
            else if (mCurrentBckFrame < 7.5f) 
                mCurrentBckFrame = 7.5f;
            else if (mCurrentBckFrame < 15.0f) 
                mCurrentBckFrame = 15.0f;
            else if (mCurrentBckFrame < 22.5f) 
                mCurrentBckFrame = 22.5f;
            else 
                mCurrentBckFrame = 0.0f;
            MR::setBckFrameAndStop(this, mCurrentBckFrame);
        }
    }
}