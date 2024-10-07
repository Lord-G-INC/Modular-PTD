#pragma once

#include "syati.h"
#include "Game/Ride/Tamakoro.h"

namespace MR {
	void startLastStageBGM();
};

class TamakoroExt : public Tamakoro {
public:
	TamakoroExt(const char* pName);
	virtual ~TamakoroExt();
	s32 mMusicNum;
};