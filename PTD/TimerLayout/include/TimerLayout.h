#pragma once

#include "Game/Screen/LayoutActor.h"
#include "syati.h"
#include "Game/Screen/GameSceneLayoutHolder.h"


class TimerLayout : public LayoutActor {
public:
	TimerLayout(const char* pName);
	virtual ~TimerLayout();
	virtual void init(const JMapInfoIter &rIter);
	virtual void control();
};

namespace NrvTimerLayout {
	NERVE(NrvWait);
};
