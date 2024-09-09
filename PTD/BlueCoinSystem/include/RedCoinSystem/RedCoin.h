#pragma once

#include "syati.h"
#include "Game/MapObj/Coin.h"
#include "Game/Screen/CountUpPaneRumbler.h"


    /* --- RED COIN --- */
    
    class RedCoin : public Coin {
    public:
        RedCoin(const char* pName);
        virtual void init(const JMapInfoIter& rIter);
        virtual void control();
        virtual bool receiveMessage(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
        void collect();
        void appearAndMove();
        void initAirBubble();
    
        LiveActorGroup* mGroup;
        f32 mLaunchVelocity;
        bool mIsCollected;
        bool mHasRewardedCoins;
        bool mRedCoinCounterPlayerPos;
    };