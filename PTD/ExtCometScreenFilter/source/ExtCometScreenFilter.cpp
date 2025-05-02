#include "syati.h"
#include "Game/NPC/CometEventKeeper.h"

f32 getCometFilterFrame(CometEventKeeper *pKeeper) {
    /*
    Colors:
     - 0: Red
     - 1: White
     - 2: Yellow
     - 3: Purple
     - 4: Blue
    */
    if (pKeeper->isStartEvent("Red"))
        return 0.0f;
    if (pKeeper->isStartEvent("Dark"))
        return 1.0f;
    if (pKeeper->isStartEvent("Purple"))
        return 3.0f;
    if (pKeeper->isStartEvent("Exterminate"))
        return 2.0f;
    if (pKeeper->isStartEvent("Mimic"))
        return 4.0f;
    if (pKeeper->isStartEvent("Quick"))
        return 4.0f;
    return 0.0f;
}

void setCometType(LayoutActor *pFilter, CometEventKeeper *pKeeper) {
    MR::startAnim(pFilter, "Color", 0);
    MR::setAnimFrameAndStop(pFilter, getCometFilterFrame(pKeeper), 0);
}

kmWrite32(0x80372958, 0x7FA4EB78); // mr r4, r29
kmCall(0x8037295C, setCometType);