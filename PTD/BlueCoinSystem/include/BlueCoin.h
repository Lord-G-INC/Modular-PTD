#pragma once

#include "syati.h"
#include "BlueCoinUtil.h"

class BlueCoin : public Coin {
    public:
    BlueCoin(const char*);
    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMessage(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    void appearAndMove();
    void collect();

    s32 mID;
    f32 mLaunchVelocity;
    bool mIsCollected;
};
