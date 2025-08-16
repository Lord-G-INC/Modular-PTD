#pragma once
#include "syati.h"
#include "Game/NPC/TalkNodeCtrl.h"

typedef struct BranchFlowExt {
	u16 mIndex;
	bool (*mFuncPtr)(u16);
};

typedef struct EventFlowExt {
	u16 mIndex;
	void (*mFuncPtr)(u16);
};

namespace NewMessageFlow {
    bool isPowerStarGotCurrentStage(u16 id);
};

namespace NewEventFlow {
	void warpPlayer(u16 param);
};