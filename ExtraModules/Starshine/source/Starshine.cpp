#include "syati.h"
#include "Game/Screen/CounterLayoutAppearer.h"

CounterLayoutAppearer* changeCoinCounterAppearanceSpot(LayoutActor* pActor) {
    return new CounterLayoutAppearer(pActor, TVec2f(-50.0f, 0.0f));
}

kmWrite32(0x80464300, 0x7FC3F378); // mr r3, r30
kmCall(0x80464304, changeCoinCounterAppearanceSpot);
kmWrite32(0x80464308, 0x48000024); // b 0x24

// Always attach CoinCounter to the "Position0001" pane 
//instead of picking from multiple based off of coin amount
kmWrite32(0x80464630, 0x48000070); // b 0x70
kmWrite32(0x804646A4, 0x48000008); // b 8