#pragma once
#include "syati.h"

class DonutBlock : public LiveActor {
public:
    DonutBlock(const char *pName);
    ~DonutBlock();

    virtual void init(const JMapInfoIter &rIter);
    virtual void attackSensor(HitSensor *pReceiver, HitSensor *pSender);

    void exeWait();
    void exeFallStart();
    void exeFall();
    void exeRespawn();

    void startFall();
    void emitEffectAndPlayAudio();

    s32 mFallDelay;
    s32 mFallDuration;
    s32 mRespawnDelay;
    f32 mFallSpeed;
    f32 mRailSpeed;
    s32 mRailDelay;
    bool mAllowEnemyFall;
    bool mAllowWallFall;

    TVec3f mOriginalPos;
    s32 mRailDelayTimer;
};

namespace NrvDonutBlock {
    NERVE(NrvWait);
    NERVE(NrvFallStart);
    NERVE(NrvFall);
    NERVE(NrvRespawn);
}