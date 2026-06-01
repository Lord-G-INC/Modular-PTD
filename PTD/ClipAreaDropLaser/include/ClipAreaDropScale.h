#pragma once

#include "syati.h" 

class ClipAreaDropScale : public ClipArea {
public:
    ClipAreaDropScale(const char*, u32 shape);
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual ~ClipAreaDropScale();

    void setBaseSize(f32);
    void exeWait();

    union {
        ClipAreaShapeSphere* mSphere;
        ClipAreaShapeBox* mBox;
    };
    u32 mShape;
    f32 _C4;
};

const size_t test = sizeof (ClipAreaDropScale);

namespace NrvClipAreaDropScale {
    NERVE_DECL_EXE(ClipAreaDropScaleNrvWait, ClipAreaDropScale, Wait);
}