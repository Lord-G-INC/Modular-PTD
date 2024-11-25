#include "ClipAreaColorController.h"

/*
 * Author: Gabbo
 * Object: ClipAreaColorController
 * Description: A Controller that lets you change the color of some Matter Splatter objects when its SW_A is activated.\nWhen the switch is turned off, the color returns to normal.
 *
 * - SW_A (required): Connects the switch to the controller.
 * 
 *  Obj_arg0: Red (0 to 255), Default 0.
 *  Obj_arg1: Green (0 to 255), Default 0.
 *  Obj_arg2: Blue (0 to 255), Default 0.
 *  Obj_arg3: Alpha (0 to 255), Default 255.
*/

ClipAreaColorController::ClipAreaColorController(const char *pName) : LiveActor(pName) {
    mRed = 0;
    mGreen = 0;
    mBlue = 0;
    mAlpha = 255;
}

void ClipAreaColorController::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::initUseStageSwitchWriteA(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mRed);
    MR::getJMapInfoArg1NoInit(rIter, &mGreen);
    MR::getJMapInfoArg2NoInit(rIter, &mBlue);
    MR::getJMapInfoArg3NoInit(rIter, &mAlpha);
    MR::invalidateClipping(this);
    initNerve(&NrvClipColor::ClipColorNrvWait::sInstance, 0);
    makeActorAppeared();
}

void ClipAreaColorController::exeWait() {
    if (MR::isOnSwitchA(this)) {
        setNerve(&NrvClipColor::ClipColorNrvActive::sInstance);
    }
}

void ClipAreaColorController::exeActive() {
    if (MR::isFirstStep(this)) {
        const GXColor color = {mRed, mGreen, mBlue, mAlpha};
        MR::setFallOutFieldDrawEdgeColor(color);
    }

    if (!MR::isOnSwitchA(this)) {
        const GXColor color = {0, 60, 255, 124};
        MR::setFallOutFieldDrawEdgeColor(color);
        setNerve(&NrvClipColor::ClipColorNrvWait::sInstance);
    }
}

namespace NrvClipColor {    
    void ClipColorNrvWait::execute(Spine* pSpine) const {
        ((ClipAreaColorController*)pSpine->mExecutor)->exeWait();
    }
    void ClipColorNrvActive::execute(Spine* pSpine) const {
        ((ClipAreaColorController*)pSpine->mExecutor)->exeActive();
    }
    ClipColorNrvWait(ClipColorNrvWait::sInstance);
    ClipColorNrvActive(ClipColorNrvActive::sInstance);
}; 