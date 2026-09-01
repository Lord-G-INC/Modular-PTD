#pragma once

#include "syati.h"

class PickpocketHolder : public DeriveActorGroup<Coin> {
    public:
    PickpocketHolder(const char* pName);

    virtual ~PickpocketHolder();
    virtual void init(const JMapInfoIter& rIter);
    void dropCoins(bool influencedByVelocity);
};

namespace PickpocketModeUtil {
    NameObj* createPickpocketModeHolder();
}