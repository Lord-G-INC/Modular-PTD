#pragma once

#include "Game/LiveActor/LiveActor.h"

class CameraTargetDemoActor : public LiveActor {
public:
    CameraTargetDemoActor(MtxPtr, const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual MtxPtr getBaseMtx() const;

    void setTargetMtx(MtxPtr pNewTargetMtx);

    TPos3f mMtx;
};