#include "LanternClipArea.h"
#include "Game/MapObj/ClipAreaDropHolder.h"

/*
 * Author: Gabbo
 * Object: LanternClipArea
 * Description:
 * A custom lantern that evokes a spherical matter splatter when it is selected with the player's cursor,
 * and disappears after a chosen amount of time. A matter splatter setup is needed to make it work.
 * 
 * - Obj_arg0: The size of the sphere, Default 750.
 * - Obj_arg1: The time in frames it takes for the sphere to grow until it reaches its specified size in Obj_arg0, Default 30.
 * - Obj_arg2: The time in frames it stays at full size before it shrinks down, Default 180.
 * - Obj_arg3: the time in frames it takes for the sphere to shrink down to 0, Default 120.
 * - Obj_arg4: the color to use (0 = Gray, 1 = Red, 2 = Green, 3 = Blue, 4 = Purple, 5 = Cyan, 6 = Gold), Default 0.
 *
 * It can be paired with the Matter Splatter Color Controller Module to change the color of the sphere.
 * It enables SW_A when the sphere starts growing and disables it when it's done shrinking down to 0.
 * It enables SW_A when the sphere is at full size and disables it when the sphere starts to shrink down again.
*/

LanternClipArea::LanternClipArea(const char *pName) : LiveActor(pName) {
    mRadius = -1;
    mOpening = -1;
    mStay = -1; 
    mClosure = -1; 
    mTotal = 0;
    mColorFrame = 0.0f;
    MR::createClipAreaDropHolder();
}

void exeWaitClipDropArea(ClipAreaDrop *pDrop){  
    if (MR::isFirstStep(pDrop)) {
        pDrop->mTime = 0;
    }
    f32 radius;  

    /* a Nintendo programmer was high while making this, so I had to decompile the entire function and write 
    a new calculation sytem in the else statement in order to be able to individually access the time of both
    the opening and closure of the bubble */
    if (pDrop->_D0 == 5 && pDrop->_D4 == 250 && pDrop->_D8 == 300) {
        if (pDrop->mTime >= pDrop->_D0) {
            f32 v6 = pDrop->mTime;
            f32 normClamp = MR::normalizeClamp(v6, pDrop->_D4, pDrop->_D8);
            radius = MR::getEaseOutValue(normClamp, pDrop->mRadius, 0.0f, 1.0f);
        }
        else {
            f32 normClamp = MR::normalizeClamp(pDrop->mTime, 0.0f, pDrop->_D0);
            radius = MR::getEaseOutValue(normClamp, 0.0f, pDrop->mRadius, 1.0f);
        }

        pDrop->mSphere->mRadius = radius;

        if (pDrop->_D8 <= pDrop->mTime) {
            pDrop->kill();
        }

        if (pDrop->_E0) {
            pDrop->mTime++;
        }        
    }
    else {
        if (pDrop->mTime >= pDrop->_D0 + pDrop->_D4) {
            f32 time = pDrop->mTime - pDrop->_D0 - pDrop->_D4;
            f32 normClamp = MR::normalizeClamp(time, 0.0f, pDrop->_D8);
            radius = MR::getEaseInValue(normClamp, pDrop->mRadius, 0.0f, 1.0f);    
        }
        else {
            f32 normClamp = MR::normalizeClamp(pDrop->mTime, 0.0f, pDrop->_D0);
            radius = MR::getEaseOutValue(normClamp, 0.0f, pDrop->mRadius, 1.0f);                    
        }

        pDrop->mSphere->mRadius = radius;

        if (pDrop->mTime >= pDrop->_D0 + pDrop->_D4 + pDrop->_D8) {
            pDrop->kill();
        }
        else {
            pDrop->mTime++;
        }
    }
}

kmBranch(0x80285DE0, exeWaitClipDropArea);

void LanternClipArea::init(const JMapInfoIter &rIter) {
    MR::processInitFunction(this, rIter, false);
    MR::initDefaultPos(this, rIter);
    MR::initStarPointerTarget(this, 200.0, TVec3f(0.0f, 0.0f, 0.0f));
    initHitSensor(1);
    initEffectKeeper(8, NULL, false);
    MR::connectToSceneMapObj(this);
    MR::addHitSensorMapObj(this, "Body", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::getJMapInfoArg0NoInit(rIter, &mRadius);
    MR::getJMapInfoArg1NoInit(rIter, &mOpening);
    MR::getJMapInfoArg2NoInit(rIter, &mStay);
    MR::getJMapInfoArg3NoInit(rIter, &mClosure);
    MR::getJMapInfoArg4NoInit(rIter, &mColorFrame);
    MR::startBrkAndSetFrameAndStop(this, "ColorChange", mColorFrame);
    MR::startBtpAndSetFrameAndStop(this, "LanternLight", 0.0f);
    MR::invalidateClipping(this);
    MR::useStageSwitchWriteA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    initNerve(&NrvLantern::LanternNrvWait::sInstance, 0);
    makeActorAppeared();
    if (mRadius <= 0.0f) {
        mRadius = 750.0f;
    }         

    if (mOpening <= 0) {
        mOpening = 30;
    }   

    if (mStay <= 0) {
        mStay = 180;
    }

    if (mClosure <= 0) {
        mClosure = 120;
    }
    mTotal = (mOpening + mStay + mClosure);
}

void LanternClipArea::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isSensorPlayerOrYoshiOrRide(pReceiver)) {
        MR::sendArbitraryMsg(ACTMES_PUSH_FORCE, pReceiver, pSender);
    }
}

void LanternClipArea::exeWait(){
    if (MR::isNearPlayer(this, 4750.0f)) {
        if (MR::isStarPointerPointing(this, 0, true, 0)){
            if (MR::testCorePadTriggerA(0)){
                setNerve(&NrvLantern::LanternNrvStart::sInstance);
            }
        }        
    }
}

void LanternClipArea::exeStartClipArea(){
    if (MR::isFirstStep(this)){
        MR::emitEffect(this, "Get");
        MR::emitEffect(this, "Glow");
        MR::startSystemSE("SE_SY_LIGHT_FRUIT_GET", -1, -1);
        MR::startBtpAndSetFrameAndStop(this, "LanternLight", 1.0f);
        MR::killClipAreaDrop(mTranslation, mOpening, mStay, mClosure, mRadius);
        if (MR::isValidSwitchA(this)) 
            MR::onSwitchA(this);
    }

    if (MR::isGreaterStep(this, mOpening) && MR::isValidSwitchB(this)) 
        MR::onSwitchB(this);
    else if (MR::isGreaterStep(this, mOpening + mStay) && MR::isValidSwitchB(this)) 
        MR::offSwitchB(this);

    if (MR::isGreaterEqualStep(this, mTotal)) {
        MR::startBtpAndSetFrameAndStop(this, "LanternLight", 0.0f);
        MR::startSystemSE("SE_SY_LIGHT_FRUIT_TIMER_END", -1, -1);
        MR::deleteEffectAll(this);
        setNerve(&NrvLantern::LanternNrvWait::sInstance);
        if (MR::isValidSwitchA(this)) 
            MR::offSwitchA(this);
    }
}

namespace NrvLantern {    
    void LanternNrvWait::execute(Spine* pSpine) const {
        ((LanternClipArea*)pSpine->mExecutor)->exeWait();
    }
    void LanternNrvStart::execute(Spine* pSpine) const {
        ((LanternClipArea*)pSpine->mExecutor)->exeStartClipArea();
    }
    LanternNrvWait(LanternNrvWait::sInstance);
    LanternNrvStart(LanternNrvStart::sInstance);
}; 