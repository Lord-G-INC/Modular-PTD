#pragma once

#include "syati.h"
#include "Game/Screen/CountUpPaneRumbler.h"

class RedCoinCounter : public LayoutActor {
public:
    RedCoinCounter(const char* pName);
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();
    void setStarIcon(s32 starID, s32 iconID);
    void startCountUp(s32 count, bool hasAllCoins, bool isRedCoinSwitchUsed);
    void exeAppear();
    void exeAppearWithUpdate();
    void exeDisappear();
    void exeCountUp();
    void exeComplete();

    CountUpPaneRumbler* mPaneRumbler;
    s32 mRedCoinCount;
    s32 mLayoutMode;
    bool mHasAllRedCoins;
};

namespace NrvRedCoinCounter {
    NERVE(NrvAppear);
    NERVE(NrvAppearWithUpdate);
    NERVE(NrvDisappear);
    NERVE(NrvCountUp);
    NERVE(NrvComplete);
    NERVE(NrvHide); 
};

class RedCoinCounterPlayer : public LayoutActor {
public:
    RedCoinCounterPlayer(const char* pName);
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    void exeAppear();
    void calcScreenPos();

    LiveActor* mLastRedCoin;
    s32 mNumCoins;
};

namespace NrvRedCoinCounterPlayer {
    NERVE(NrvHide);
    NERVE(NrvAppear);
}