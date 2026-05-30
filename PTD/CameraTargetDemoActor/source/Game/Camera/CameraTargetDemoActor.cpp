#include "Game/Camera/CameraTargetDemoActor.h"

namespace {
    void makeRTFromMtxPtr(TVec3f* pOutTrans, TVec3f* pOutRot, MtxPtr src, bool toDegree) {
        if (pOutTrans) {
            ((TPos3f*)src)->getTrans(*pOutTrans);
        }

        if (pOutRot) {
            ((TRot3f*)src)->getEuler(*pOutRot);

            if (toDegree) {
                TVec3f scaled;
                pOutRot->scale(57.29578f, scaled);
                *pOutRot = scaled;
            }
        }
    }
}

CameraTargetDemoActor::CameraTargetDemoActor(MtxPtr pMtx, const char* pName) : LiveActor(pName) {
    mMtx.set(pMtx);
}

void CameraTargetDemoActor::init(const JMapInfoIter& rIter) {
    LiveActor::init(rIter);
    makeActorAppeared();
}

void CameraTargetDemoActor::setTargetMtx(MtxPtr pNewTargetMtx) {
    mMtx.set(pNewTargetMtx);
    makeRTFromMtxPtr(&mTranslation, &mRotation, pNewTargetMtx, true);
}

MtxPtr CameraTargetDemoActor::getBaseMtx() const {
    return (const MtxPtr)mMtx.mMtx;
}