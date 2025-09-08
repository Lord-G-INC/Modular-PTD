#include "PosWarpCtrl.h"

/*
* A very basic object that when placed, allows you to store the player's position by pressing 1.
* Then you can warp to that stored position by pressing 2.
*
* Shoutouts to PKSMG2 for the idea!
*/

PosWarpCtrl::PosWarpCtrl (const char *pName) : NameObj(pName) {
    mStoredPos = TVec3f(0);
    mMode = PLAYER_MODE_NORMAL;
    mMorphTimer = 0;
    mLifeNum = 3;
    mWasStored = false;
}

void PosWarpCtrl::init (const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjMovement(this);
    OSReport("PosWarpCtrl is active\n");
}

void PosWarpCtrl::movement () {
    MarioActor *pMarioActor = MarioAccess::getPlayerActor();
    if (MR::testCorePadTrigger1(0)) {
        mWasStored = true;
        mStoredPos = pMarioActor->mTranslation;
        mMode = pMarioActor->mPlayerMode;
        mMorphTimer = pMarioActor->mMorphTimer;
        mLifeNum = pMarioActor->mLifeNum;

        MR::startSystemSE("SE_SY_CAMERA_FP_VIEW", -1, -1);
        OSReport("-------------\nStored\nX: %f\nY: %f\nZ: %f\nPlayer Mode: %d\nMorph Timer: %d\nLife Num: %d\n-------------\n", mStoredPos.x, mStoredPos.y, mStoredPos.z, mMode, mMorphTimer, mLifeNum);
    }
    if (MR::testCorePadTrigger2(0)) {
        if (!mWasStored) {
            MR::startSystemSE("SE_SY_CAMERA_NG", -1, -1);
            OSReport("No position was stored!\n");
        } else {
            MR::setBinderRadius(pMarioActor, 0.0f);
            pMarioActor->mTranslation = mStoredPos;
            pMarioActor->mPlayerMode = mMode;
            pMarioActor->mMorphTimer = mMorphTimer;
            pMarioActor->mLifeNum = mLifeNum;
            MR::setBinderRadius(pMarioActor, 60.0f);
            
            MR::startSystemSE("SE_SY_CAMERA_MOVE", -1, -1);
            OSReport("Loading stored state\n");
        }
    }
}

MarioActor *PKSMG2 (MarioActor *pMarioActor) {
    PosWarpCtrl *pCtrl = new PosWarpCtrl("PosWarpCtrl");
    pCtrl->initWithoutIter();
    return pMarioActor;
}
kmBranch(0x803B7790, PKSMG2);