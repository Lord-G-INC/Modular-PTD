#include "ClipAreaDropScale.h"

ClipAreaDropScale::ClipAreaDropScale(const char* pName, u32 shape) : ClipArea(pName) {
    _C4 = 0.0f;
    mShape = shape;

    ClipAreaShape* pShape = 0;
    
    if (shape == 0) {
        mSphere = new ClipAreaShapeSphere("VolumeSphere");
        pShape = mSphere;
    }
    else if (shape == 1) {
        mBox = new ClipAreaShapeBox("VolumeBoxSmall", 0);
        pShape = mBox;
    }

    setShape(pShape);
}

void ClipAreaDropScale::init(const JMapInfoIter& rIter) {
    initBaseMatrix(rIter);
    MR::connectToScene(this, 27, -1, -1, 46);
    MR::invalidateClipping(this);
    initNerve(&NrvClipAreaDropScale::ClipAreaDropScaleNrvWait::sInstance, 0);
    makeActorDead();
}

void ClipAreaDropScale::appear() {
    LiveActor::appear();

    mSphere->mRadius = 0.0f;
    setNerve(&NrvClipAreaDropScale::ClipAreaDropScaleNrvWait::sInstance);
}

void ClipAreaDropScale::control() {
    TMtx34f mtx;
    mtx.identity();
    mtx.mMtx[0][3] = mTranslation.x;
    mtx.mMtx[1][3] = mTranslation.y;
    mtx.mMtx[2][3] = mTranslation.z;
    mBaseMtx.setInline(mtx);
}

void ClipAreaDropScale::setBaseSize(f32 v1) {
    _C4 = v1;
}

void ClipAreaDropScale::exeWait() {
    f32 f = 0;
    if (MR::isLessStep(this, 15))
        f = MR::calcNerveEaseOutValue(this, 15, 0.0f, _C4);
    else
        f = MR::calcNerveEaseInOutValue(this, 0x3C, 0xF0, _C4, 0.0);

    mSphere->mRadius = f;
    
    if (MR::isGreaterStep(this, 240))
        kill();
}

ClipAreaDropScale::~ClipAreaDropScale() {}


namespace NrvClipAreaDropScale {
    ClipAreaDropScaleNrvWait(ClipAreaDropScaleNrvWait::sInstance);
}