#include "syati.h"

class JumpStandAutoNeo : public JumpStandAuto {
public:
    JumpStandAutoNeo(const char *);
    ~JumpStandAutoNeo();
    virtual void init(const JMapInfoIter &rIter);
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual void jumpPlayer(f32 launchForce);
    virtual void exeWait();
    virtual void exeExtend();
    virtual void exeDisabled();
    void moveRail();
    bool isPlayerJumpable();
    static bool isPlayerWallTouch();

    f32 mLaunchForce;
    f32 mHipDropLaunchForce;
    f32 mBtpColorFrame;
    f32 mRailSpeed;
    bool mStopDuringExtend;
    J3DGXColor mTevColor;
};

namespace NrvJumpStandAutoNeo {
    NERVE(NrvWait);
    NERVE(NrvExtend);
    NERVE(NrvDisabled);
};