#pragma once

#include "syati.h"

class ClipAreaDropLaser : public LiveActor {
public: 
    ClipAreaDropLaser(const char* pName);
    virtual ~ClipAreaDropLaser();
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void draw() const;
    void exeWait();
    void exeMove();
    void incrementDrawCount();

    TVec3f _90[0x40];
    f32 _390;
    int _394;
    int _398;
    f32 _39C;
    u32 mInitialTrailColor;
    u32 mTrailColor;
};

namespace NrvClipAreaDropLaser {
    NERVE_DECL_EXE(ClipAreaDropLaserNrvWait, ClipAreaDropLaser, Wait);
    NERVE_DECL_EXE(ClipAreaDropLaserNrvMove, ClipAreaDropLaser, Move);
};

namespace MR {
    NameObj* createClipAreaDropHolder();
    void appearClipAreaDrop(const TVec3f&, f32);
};
