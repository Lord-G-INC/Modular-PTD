#include "CycleTransitionObj.h"
#include "Game/Util.h"

CycleTransitionObj::CycleTransitionObj(const char *pName) : LiveActor(pName) {}

void CycleTransitionObj::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    MR::joinToGroupArray(this, rIter, "CycleController", 16);

    // Use the SW_A
    MR::useStageSwitchReadA(this, rIter);
}

void CycleTransitionObj::activate() {
    if (MR::isValidSwitchA(this)) {
        OSReport("--- CycleTransitionObj --- Activating SW_A\n");
        MR::onSwitchA(this);
    }
}

void CycleTransitionObj::deactivate() {
    if (MR::isValidSwitchA(this)) {
        OSReport("--- CycleTransitionObj --- Deactivating SW_A\n");
        MR::offSwitchA(this);
    }
}