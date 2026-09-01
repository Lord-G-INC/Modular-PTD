#include "PickpocketHolder.h"

PickpocketHolder::PickpocketHolder(const char* pName) : DeriveActorGroup<Coin>("DropGroup", 40) {

    for (int i = 0; i < 40; i++) {
        Coin* pCoin = MR::createCoin(this, "Yes");
        pCoin->initWithoutIter();
        pCoin->kill();
        registerActor(pCoin);
    }

    OSReport("Coins created!\n");
}

void PickpocketHolder::init(const JMapInfoIter& rIter) {
    
}

void PickpocketHolder::dropCoins(bool influencedByVelocity) {
    for (int i = 0; i < 20; i++) {
        TVec3f vec(*MR::getPlayerGravity());
        vec.negate();
        vec.scale(35.0f);

        MR::addRandomVector(&vec, vec, 8.0f);

        OSReport("%d\n", influencedByVelocity);
        TVec3f vel(influencedByVelocity ? *MR::getPlayerVelocity() : TVec3f(0.0f));
        Coin* pCoin = (Coin*)getDeadActor();

        if (pCoin)
            pCoin->appearMove(*MR::getPlayerPos(), vel+vec, 600, 60);
    }
}

PickpocketHolder::~PickpocketHolder() {}


namespace PickpocketModeUtil {
    NameObj* createPickpocketModeHolder() {
        return new PickpocketHolder("Lmao");
    }
}