#pragma once

#include "syati.h"

class PurpleCoinAppearSpot : public LiveActor {
public:
    PurpleCoinAppearSpot(const char* pName);
    virtual ~PurpleCoinAppearSpot();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual MtxPtr getBaseMtx();

    //Coin** mCoins;
    TMtx34f mMtx;
    s32 mNumCoins;
    s32 mCoinLifeTime;
    f32 mSpeed;
};