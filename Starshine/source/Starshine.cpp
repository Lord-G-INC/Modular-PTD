#include "syati.h"
#include "Game/Screen/CounterLayoutAppearer.h"

CounterLayoutAppearer* changeCoinCounterAppearanceSpot(LayoutActor* pActor) {
    return new CounterLayoutAppearer(pActor, TVec2f(-50.0f, 0.0f));
}

kmWrite32(0x80464300, 0x7FC3F378); // mr r3, r30
kmCall(0x80464304, changeCoinCounterAppearanceSpot);
kmWrite32(0x80464308, 0x48000024); // b 0x24

// Detach the Coin Counter from Position panes
kmWrite32(0x80464630, 0x48000088); // b 0x88
kmWrite32(0x804642DC, 0x48000010); // b 0x10