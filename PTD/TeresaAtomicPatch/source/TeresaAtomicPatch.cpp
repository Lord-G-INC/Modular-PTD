//#include "TeresaAtomic.h"
#include "syati.h"
#include "Game/MapObj/ClipAreaDropHolder.h"

void newTeresaAtomicInit (TeresaAtomic *pActor, const JMapInfoIter &rIter) {
    MR::processInitFunction(pActor, rIter, false);
    if (MR::isConnectedWithRail(rIter)) {
        MR::moveCoordAndTransToStartPos(pActor);
        MR::sub_800531B0(&pActor->mFrontVec, pActor, 100.0f);
    } else 
        MR::calcFrontVec(&pActor->mFrontVec, pActor);
    
    MR::getJMapInfoArg0NoInit(rIter, &pActor->mSpeed);
    f32 v0 = -1.0f;
    MR::getJMapInfoArg1WithInit(rIter, &v0);
    if (v0 >= 1.0f) {
        MR::createClipAreaDropHolder();
        pActor->_90 = MR::appearClipAreaDrop(pActor->mTranslation, v0);
    }
    pActor->mPowerStarType = -1;
    MR::getJMapInfoArg2WithInit(rIter, &pActor->mPowerStarType);
    if (pActor->mPowerStarType > 0) 
        MR::declarePowerStar(pActor);
    MR::calcGravity(pActor);
    TVec3f v1 = pActor->mTranslation + pActor->mGravity * -900.0f;
    MR::resetPosition(pActor, v1);
    pActor->mChildTeresaNum = 4;
    MR::getJMapInfoArg3NoInit(rIter, &pActor->mChildTeresaNum);
    pActor->mChildTeresas = new PartsModel*[pActor->mChildTeresaNum];
    pActor->mTeresaAtomicPosition = MR::createPartsModelMapObj2(pActor, "取り巻きテレサ位置", "TeresaAtomicPosition", pActor->getBaseMtx());
    MR::startAction(pActor->mTeresaAtomicPosition, "Wait");

    for (s32 i = 0; i < pActor->mChildTeresaNum; i++) {
        pActor->mChildTeresas[i] = new PartsModel(pActor->mTeresaAtomicPosition, "取り巻きテレサ", 0, 0, -1, false);
        MR::processInitFunction(pActor->mChildTeresas[i], "Teresa", "TeresaFollower", false);
        pActor->mChildTeresas[i]->initWithoutIter();
        char jointName[11];
        sprintf(jointName, "Follower%d", i + 1);
        pActor->mChildTeresas[i]->loadFixedPosition(jointName, pActor);
        MR::setSensorFollowPos(pActor, jointName, pActor->mChildTeresas[i]->mTranslation);
    }
    pActor->initNerve(&NrvTeresaAtomic::TeresaAtomicNrvWait::sInstance, 0);
    MR::useStageSwitchSyncAppear(pActor, rIter);
}
kmBranch(0x8020F400, newTeresaAtomicInit);