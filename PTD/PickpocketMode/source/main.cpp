#include "syati.h"
#include "PickpocketHolder.h"

void DropPurpleCoins(MarioActor* pMario, u16 u) {
    pMario->decLife(u);

    PickpocketHolder* pHolder = (PickpocketHolder*)MR::getSceneObjHolder()->getObj(0x68);
    pHolder->dropCoins(true);

    OSReport("Oh no my pockets\n");
}

void DropPurpleCoinsWithoutVelocityInfluence(MarioActor* pMario, u16 u) {
    pMario->decLife(u);

    PickpocketHolder* pHolder = (PickpocketHolder*)MR::getSceneObjHolder()->getObj(0x68);
    pHolder->dropCoins(false);

    OSReport("Oh no my pockets\n");
}


kmCall(0x80394990, DropPurpleCoinsWithoutVelocityInfluence);

kmBranch(0x803B9254, DropPurpleCoins);
kmBranch(0x803B9264, DropPurpleCoins);
kmBranch(0x803B9274, DropPurpleCoins);

kmCall(0x803E84EC, DropPurpleCoins);