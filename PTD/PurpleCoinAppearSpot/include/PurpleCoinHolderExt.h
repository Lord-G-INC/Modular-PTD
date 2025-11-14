#pragma once

#include "Game/MapObj/CoinHolder.h"

class PurpleCoinHolderExt : public CoinHolder {
public:
    PurpleCoinHolderExt(const char* pName);
    virtual ~PurpleCoinHolderExt();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();

    LiveActorGroup* mGroup;
};


namespace PurpleCoinUtil {
	void declareCoin(const NameObj* pObj, s32 amount);
	void appearCoinPop(const NameObj* pObj, const TVec3f& rVec, s32 l);
    void appearCoinPopToDirection(const NameObj* pObj, const TVec3f& rPos, const TVec3f rVel, s32 l);
};