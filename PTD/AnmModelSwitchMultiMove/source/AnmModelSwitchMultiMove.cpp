#include "AnmModelSwitchMultiMove.h"

AnmModelSwitchMultiMove::AnmModelSwitchMultiMove (const char *pName) : LiveActor(pName) {

}

AnmModelSwitchMultiMove::~AnmModelSwitchMultiMove () {

}

void AnmModelSwitchMultiMove::init (const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::processInitFunction(this, rIter, false);
    MR::connectToSceneMapObj(this);
    MR::getObjectName(&mObjectName, rIter);
    
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);

    initHitSensor(1);
    MR::addHitSensorMapObj(this, "Body", 1, 0.0f, TVec3f(0));
    if (MR::isExistCollisionResource(this, mObjectName)) 
        MR::initCollisionParts(this, mObjectName, getSensor("Body"), NULL);
    
    if (MR::isValidSwitchAppear(this)) {
        MR::syncStageSwitchAppear(this);
        kill();
    } else 
        appear();
}

void AnmModelSwitchMultiMove::control () {
    if (MR::isValidSwitchAppear(this) && MR::isOnSwitchAppear(this)) {
        MR::startAction(this, "MoveAppear");
        appear();
    }

    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) 
        MR::startAction(this, "MoveA");
    else if (MR::isValidSwitchB(this) && MR::isOnSwitchB(this)) 
        MR::startAction(this, "MoveB");
    
    if (MR::isActionEnd(this)) 
        MR::startAction(this, mObjectName);
}