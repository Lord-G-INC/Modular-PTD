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
    u32 getRgba(s32 railPoint);

    TVec3f mDrawPoints[0x40];
    s32 mNumPointsToDraw;
    s32 mPointIndexToSkipDraw;
    s32 mDrawCount;
    f32 mSpeed;
    u32 mTrailColor;
    s32 mCooldown;
    s32 mCooldownTimer;
    u32 mTimerToResetDraw;
};

namespace NrvClipAreaDropLaser {
    NERVE_DECL_EXE(ClipAreaDropLaserNrvWait, ClipAreaDropLaser, Wait);
    NERVE_DECL_EXE(ClipAreaDropLaserNrvMove, ClipAreaDropLaser, Move);
};

namespace MR {
    NameObj* createClipAreaDropHolder();
    void appearClipAreaDrop(const TVec3f&, f32);
};
