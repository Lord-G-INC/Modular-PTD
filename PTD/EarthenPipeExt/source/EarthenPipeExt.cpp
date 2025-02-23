#include "syati.h"

void extInit (EarthenPipe *pPipe, const JMapInfoIter &rIter) {
    MR::initDefaultPos(pPipe, rIter);
    MR::useStageSwitchReadAppear(pPipe, rIter);
    if (MR::isConnectedWithRail(rIter)) {
        pPipe->initRailRider(rIter);
        MR::moveCoordAndTransToNearestRailPos(pPipe);
        pPipe->mRailRider->setSpeed(30.0f);
    }
}
kmCall(0x80296C24, extInit);

bool isNerveShowing (EarthenPipe *pPipe) {
    return pPipe->isNerve(&NrvEarthenPipe::EarthenPipeNrvShow::sInstance) || pPipe->isNerve(&NrvEarthenPipe::EarthenPipeNrvShowUp::sInstance) || pPipe->isNerve(&NrvEarthenPipe::EarthenPipeNrvWaitToShowUp::sInstance);
}

bool isNerveHiding (EarthenPipe *pPipe) {
    return pPipe->isNerve(&NrvEarthenPipe::EarthenPipeNrvHide::sInstance) || pPipe->isNerve(&NrvEarthenPipe::EarthenPipeNrvHideDown::sInstance) || pPipe->isNerve(&NrvEarthenPipe::EarthenPipeNrvWaitToHideDown::sInstance);
}

bool isAbleToShowOrHide (EarthenPipe *pPipe) {
    return !pPipe->isNerve(&NrvEarthenPipe::EarthenPipeNrvReady::sInstance) && !pPipe->isNerve(&NrvEarthenPipe::EarthenPipeNrvTargetPipeShowUp::sInstance) && !pPipe->isNerve(&NrvEarthenPipe::EarthenPipeNrvPlayerIn::sInstance) && !pPipe->isNerve(&NrvEarthenPipe::EarthenPipeNrvPlayerOut::sInstance);
}

void extControl (EarthenPipe *pPipe) {
    MR::updateMaterial(pPipe);
    if (MR::isValidSwitchAppear(pPipe)) {
        if (MR::isOnSwitchAppear(pPipe) && isAbleToShowOrHide(pPipe) && !isNerveShowing(pPipe)) 
            pPipe->setNerve(&NrvEarthenPipe::EarthenPipeNrvShowUp::sInstance);
        else if (!MR::isOnSwitchAppear(pPipe) && isAbleToShowOrHide(pPipe) && !isNerveHiding(pPipe)) 
            pPipe->setNerve(&NrvEarthenPipe::EarthenPipeNrvHideDown::sInstance);
    }
    if (MR::isExistRail(pPipe)) {
        pPipe->mRailRider->move();
        MR::moveTransToCurrentRailPos(pPipe);
        if (pPipe->mRailRider->isReachedGoal()) 
            pPipe->mRailRider->reverse();
    }
}
kmCall(0x80297B34, extControl);