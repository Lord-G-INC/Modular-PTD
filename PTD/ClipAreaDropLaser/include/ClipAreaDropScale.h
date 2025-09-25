#pragma once

#include "syati.h" 

class ClipAreaDropScale : public ClipArea {
public:
    ClipAreaDropScale(const char*);
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual ~ClipAreaDropScale();

    void setBaseSize(f32);
    void exeWait();

    ClipAreaShapeSphere* _C0;
    f32 _C4;
};

namespace NrvClipAreaDropScale {
    NERVE_DECL_EXE(ClipAreaDropScaleNrvWait, ClipAreaDropScale, Wait);
}

