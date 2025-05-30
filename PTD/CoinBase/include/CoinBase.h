#pragma once

#include "syati.h"

#define DARKCOMETEXIST_FALSE false
#define DARKCOMETEXIST_TRUE true

#define PLAYSOUND_FALSE false
#define PLAYSOUND_TRUE true

#define SWAPPEAR_FALSE false
#define SWAPPEAR_TRUE true

#define SWB_FALSE false
#define SWB_TRUE true

#define SENSORSCALING_FALSE false
#define SENSORSCALING_TRUE true

#define INITFUNCTION_FALSE false
#define INITFUNCTION_TRUE true

typedef struct CoinInfo {
    void setDefault();

    bool mExistInDarkComet;
    bool mPlaySound;
    bool mUseSyncAppear;
    bool mUseReadSwitchB;
    bool mIgnoreSensorScaling;
    bool mInitFunction;
    s32 mCoinAddNo;
    char mMirrorActorName[32];
};

class CoinBase : public Coin {
public:
    CoinBase(const char* pName);
    virtual ~CoinBase();
    virtual void init(const JMapInfoIter& rIter);
    virtual void makeActorAppeared();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);


    virtual bool vRequestGetCoin();
    void noticeGetCoin();
    void appearAndMove(f32 vel, const char* soundName);

    CoinInfo mCoinInfo;
};