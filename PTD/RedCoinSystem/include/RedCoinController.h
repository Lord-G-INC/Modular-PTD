	#pragma once

	#include "syati.h"
    #include "Game/Screen/CountUpPaneRumbler.h"
    #include "RedCoin.h"
    #include "RedCoinLayouts.h"
    #include "RedCoinSwitch.h"

    class RedCoinController : public LiveActor {
    public:
        RedCoinController(const char* pName);
    
        virtual void init(const JMapInfoIter& rIter);
        virtual void initAfterPlacement();
        virtual void movement();
        void startCountUp(LiveActor* pRedCoin);
        void calcCounterVisibility();
        void calcCounterTimer();
        void resetAllRedCoins();
        void appearFromSwitch();
    
        RedCoinCounter* mRedCoinCounter;
        RedCoinCounterPlayer* mRedCoinPlayerCounter;
        RedCoinSwitch* mRedCoinSwitch;
        s32 mNumCoins;
        s32 mLinkedCoins;
        s32 mElapsed;
        bool mRewardCoins; // Obj_arg0
        bool mHasAllRedCoins;
        bool mIsValidCounterAppear;
    };