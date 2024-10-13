#include "PosWarpCtrl.h"

/*
* A very basic object that when placed, allows you to store the player's position by pressing 1.
* Then you can warp to that stored position by pressing 2.
*
* Shoutouts to PKSMG2 for the idea!
*/

PosWarpCtrl::PosWarpCtrl (const char *pName) : NameObj(pName) {
    mStoredPos = TVec3f(0);
    mWasPosStored = false;
}

void PosWarpCtrl::init (const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjMovement(this);
    OSReport("PosWarpCtrl is active\n");
}

void PosWarpCtrl::movement () {
    MarioActor *pMarioActor = MarioAccess::getPlayerActor();
    if (MR::testCorePadTrigger1(0)) {
        mStoredPos = pMarioActor->mTranslation;
        mWasPosStored = true;
        MR::startSystemSE("SE_SY_CAMERA_FP_VIEW", -1, -1);
        OSReport("Stored position\nX: %f\nY: %f\nZ: %f\n", mStoredPos.x, mStoredPos.y, mStoredPos.z);
    }
    if (MR::testCorePadTrigger2(0)) {
        if (!mWasPosStored) {
            MR::startSystemSE("SE_SY_CAMERA_NG", -1, -1);
            OSReport("No position was stored!\n");
        } else {
            MR::setBinderRadius(pMarioActor, 0.0f);
            pMarioActor->mTranslation = mStoredPos;
            MR::setBinderRadius(pMarioActor, 60.0f);
            MR::startSystemSE("SE_SY_CAMERA_MOVE", -1, -1);
            OSReport("Warping to stored position\n");
        }
    }
}