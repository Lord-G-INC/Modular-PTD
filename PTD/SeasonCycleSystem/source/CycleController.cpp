#include "CycleController.h"
#include "Game/Util.h"

/* --- CYCLE CONTROLLER --- */
/*
    A controller for a repeatable cycle.
    Manages the appearing of and timing between cycle parts.

    Obj_args:
    0: [Int] Cycle part change delay: in 60 frames per seconds (1800 = 30 seconds)
    1: [Int] Cycle part transition time: in 60 frames per seconds (90 = 1.5 seconds)
*/

CycleController::CycleController(const char *pName) : LiveActor(pName) {
    mActive = false;
    mTimer = 0;
    mTransitionTimer = 0;
    mCurrentCycleObj = 0;
    mCycleObjCount = 0;
}

void CycleController::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    MR::joinToGroupArray(this, rIter, "CycleController", 16);

    // Get cycle obj change delay from Obj_arg0
    cycleChangeDelay = 1800;
    MR::getJMapInfoArg0NoInit(rIter, &cycleChangeDelay);
    // Prevent negative delay
    if (cycleChangeDelay < 0) {
        cycleChangeDelay = 1800;
    }
    mTimer = cycleChangeDelay;

    // Get cycle obj transition time from Obj_arg1
    cycleTransitionTime = 75;
    MR::getJMapInfoArg1NoInit(rIter, &cycleTransitionTime);
    // Prevent time below 10 frames
    if (cycleTransitionTime < 10) {
        cycleTransitionTime = 75;
    }

    // Use the SW_A and SW_B
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);

    // Listen to SW_A
    if (MR::isValidSwitchA(this)) {
        MR::listenStageSwitchOnA(this, MR::Functor(this, &syncSwitchOnA));
    }

    // Listen to SW_B
    if (MR::isValidSwitchB(this)) {
        MR::listenStageSwitchOnB(this, MR::Functor(this, &syncSwitchOnB));
    }
}

void CycleController::initAfterPlacement() {
    LiveActorGroup *group = MR::getGroupFromArray(this);
    for (s32 i = 0; i < group->mNumObjs; i++) {
        LiveActor *actor = group->getActor(i);
        if (MR::isEqualString(actor->mName, "CycleObj")) {
            CycleObj *cycleObj = (CycleObj *) actor;
            mCycleObjCount++;

            if (cycleObj->cycleObjIndex == 0) {
                cycleObj->activate();
            }
        }
    }
}

void CycleController::movement() {
    if (mTransitionTimer >= 0) {
        mTransitionTimer--;
    }

    if (mTransitionTimer == cycleTransitionTime / 2) {
        toNextCycleObj();
        return;
    }

    if (mTransitionTimer == 0) {
        endTransition();
    }

    if (!mActive || mTransitionTimer > 0) {
        return;
    }

    mTimer--;
    if (mTimer == 0) {
        startTransition();
    }
}

void CycleController::syncSwitchOnA() {
    mActive = true;
}

void CycleController::syncSwitchOnB() {
    mActive = false;
}

void CycleController::startTransition() {
    mTransitionTimer = cycleTransitionTime;

    // Play a fancy sound effect
    MR::startSystemSE("SE_OJ_SW_EFFECT", -1, -1);

    LiveActorGroup *group = MR::getGroupFromArray(this);
    for (s32 i = 0; i < group->mNumObjs; i++) {
        LiveActor *actor = group->getActor(i);
        if (MR::isEqualString(actor->mName, "CycleTransitionObj")) {
            CycleTransitionObj *obj = (CycleTransitionObj *) actor;
            obj->activate();
        }
    }
}

void CycleController::endTransition() {
    LiveActorGroup *group = MR::getGroupFromArray(this);
    for (s32 i = 0; i < group->mNumObjs; i++) {
        LiveActor *actor = group->getActor(i);
        if (MR::isEqualString(actor->mName, "CycleTransitionObj")) {
            CycleTransitionObj *obj = (CycleTransitionObj *) actor;
            obj->deactivate();
        }
    }
}

void CycleController::toNextCycleObj() {
    // Disable the current active cycle obj
    CycleObj *oldCycleObj = findCycleObj(mCurrentCycleObj);
    if (oldCycleObj != NULL) {
        oldCycleObj->deactivate();
    }

    // Change the current cycle obj index
    mTimer = cycleChangeDelay;
    mCurrentCycleObj = (mCurrentCycleObj + 1) % mCycleObjCount;

    // Enable the new cycle obj
    CycleObj *newCycleObj = findCycleObj(mCurrentCycleObj);
    if (oldCycleObj != NULL && newCycleObj != NULL) {
        newCycleObj->activate();

        // Change the music
        if (oldCycleObj->stageBgmInfoIndex == newCycleObj->stageBgmInfoIndex) {
            MR::setStageBGMState(newCycleObj->bgmChannel, newCycleObj->musicFadeTime);
            return;
        }

        const char *bgmName = newCycleObj->getBgmName();
        MR::startStageBGM(bgmName, false);
    }
}

CycleObj *CycleController::findCycleObj(s32 index) {
    LiveActorGroup *group = MR::getGroupFromArray(this);
    for (s32 i = 0; i < group->mNumObjs; i++) {
        LiveActor *actor = group->getActor(i);
        if (MR::isEqualString(actor->mName, "CycleObj")) {
            CycleObj *cycleObj = (CycleObj *) actor;
            if (cycleObj->cycleObjIndex == index) {
                return cycleObj;
            }
        }
    }

    return NULL;
}