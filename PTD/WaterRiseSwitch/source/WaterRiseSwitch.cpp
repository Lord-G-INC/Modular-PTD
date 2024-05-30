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

void joinRailMoveWaterObjs(RailMoveObj* pActor, const JMapInfoIter& rIter) {
    pActor->RailMoveObj::init(rIter);
    ModelObj* pObj = MR::createIndirectModel(pActor, pActor->getBaseMtx());
    asm("stw %0, 0xE0(%1)" : "=r" (pObj) : "=r" (pActor));

    s32 group = -1;
    MR::getJMapInfoArg3NoInit(rIter, &group);
    if (MR::isExistSceneObj(EXT_SCENE_OBJ_WATER_RISE_HOLDER))
        pt::getWaterRiseHolder()->mGroups[group]->registerActor(pActor);
}

//kmCall(0x802EA380, joinRailMoveWaterObjs);
//kmWrite32(0x802EA384, 0x4E800020);

namespace pt {
    extern void clampS32(s32 min, s32 max, s32* val);

    WaterRiseHolder::WaterRiseHolder(const char* pName) : NameObj(pName) {
        for (s32 i = 0; i < 4; i++) {
            char str[17];
            snprintf(str, 17, "WaterRiseGroup%d", i);
            mGroups[i] = new LiveActorGroup(str, 16);
        }
    }

    void WaterRiseHolder::init(const JMapInfoIter& rIter) {
        for (s32 i = 0; i < 4; i++) {
            OSReport("WaterRiseHolder Init %s\n", mGroups[i]->mName);
        }
    }

    NameObj* createWaterRiseHolder() {
        return new WaterRiseHolder("WaterRiseHolder");
    }

    WaterRiseHolder* getWaterRiseHolder() {
        return (WaterRiseHolder*)MR::getSceneObjHolder()->getObj(EXT_SCENE_OBJ_WATER_RISE_HOLDER);
    }

    WaterRiseSwitch::WaterRiseSwitch(const char *pName) : LiveActor(pName) {
        mIsRiseActive = false;
        mSpeed = 1.0f;
        mOffsetY = -350.0f;
        mWaterRiseGroup = 0;
        mCurrentBckFrame = 0.0f;

        if (!MR::isExistSceneObj(EXT_SCENE_OBJ_WATER_RISE_HOLDER))
            MR::createSceneObj(EXT_SCENE_OBJ_WATER_RISE_HOLDER);
    }
    void WaterRiseSwitch::init(const JMapInfoIter &rIter) {
        MR::initDefaultPos(this, rIter);
        MR::processInitFunction(this, rIter, false);
        MR::connectToSceneMapObj(this);
        MR::invalidateClipping(this);
        MR::calcGravity(this);
        MR::getJMapInfoArg0NoInit(rIter, &mSpeed);
        MR::getJMapInfoArg1NoInit(rIter, &mOffsetY);
        MR::getJMapInfoArg2NoInit(rIter, &mWaterRiseGroup);

        initHitSensor(1);
        MR::addHitSensorMapObj(this, "WaterRise", 8, 10.0f, TVec3f(0));
        MR::validateHitSensor(getSensor("WaterRise"));
        makeActorAppeared();
        OSReport("%d\n", getWaterRiseHolder()->mGroups[mWaterRiseGroup]->mNumObjs);
    }

    void WaterRiseSwitch::attackSensor(HitSensor *pReceiver, HitSensor *pSender) {
        if (MR::isSensorPlayer(pSender)) {
            if (!mIsRiseActive) {
                MR::startSystemSE("SE_SY_READ_RIDDLE_SS", -1, -1);
                MR::startBck(this, "WaterRiseSpin", NULL);
                MR::setBckFrame(this, mCurrentBckFrame);
                exeRiseWater();
            }
        }
    }
    void WaterRiseSwitch::control() {
        if (mIsRiseActive) {
            exeRiseWater();
        }
    }
    void WaterRiseSwitch::exeRiseWater() {
        LiveActorGroup* pGroup = getWaterRiseHolder()->mGroups[mWaterRiseGroup];
        s32 numWaters = pGroup->getLivingActorNum();
        for (int i = 0; i < numWaters; i++) {
            mIsRiseActive = true;
            f32 dist = this->mTranslation.y - pGroup->getActor(i)->mTranslation.y + mOffsetY;
            if (dist < mSpeed && dist > -mSpeed) 
                mIsRiseActive = false;
            else if (dist < 0.0f) 
                pGroup->getActor(i)->mTranslation.y -= mSpeed;
            else 
                pGroup->getActor(i)->mTranslation.y += mSpeed;
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