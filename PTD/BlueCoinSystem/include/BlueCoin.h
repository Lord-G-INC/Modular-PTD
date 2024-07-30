#pragma once

#include "syati.h"
#include "BlueCoinUtil.h"

class BlueCoin : public Coin {
    public:
    BlueCoin(const char*);
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual bool receiveMessage(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    void collect();
    void appearAndMove();
    void initAirBubble();

    s32 mID;
    f32 mLaunchVelocity;
    bool mIsCollected;
};
