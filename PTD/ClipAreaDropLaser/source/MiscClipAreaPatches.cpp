#include "syati.h"

void ClipAreaMovableReadSwitchAppear(LiveActor* pActor, const JMapInfoIter& rIter) {
    MR::addBaseMatrixFollowTarget(pActor, rIter, 0, 0);

    if (!MR::useStageSwitchSyncAppear(pActor, rIter))
        pActor->makeActorAppeared();
}

kmCall(0x8028672C, ClipAreaMovableReadSwitchAppear);
kmWrite32(0x80286730, PPC_B(0x14)); // b 0x14




void redirBoxcalcVolumeMatrix(ClipAreaShapeBox* pBox, TPos3f* pPos, const TPos3f& rPos, const TVec3f& rVec) {
    pPos->set(rPos);
    if (pBox->_C == 1) {
        MR::addTransMtxLocalY((MtxPtr)pPos, pBox->mRadius*rVec.y);
    }
    
    TVec3f stack_14 = TVec3f(rVec);
    stack_14.scale(pBox->mRadius);
    TVec3f stack_8(stack_14);
    stack_8.scale(0.01f);
    MR::preScaleMtx((MtxPtr)pPos, stack_8);
}

kmWritePointer(0x8069DD6C, redirBoxcalcVolumeMatrix);