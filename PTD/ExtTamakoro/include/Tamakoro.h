#pragma once

#include "syati.h"

class SphereAccelSensorController;
class TamakoroTutorial;

class AudTamakoroBgmCtrl {
	public:
	void init(const char *);
};

namespace MR {
	void startLastStageBGM();
};

class Tamakoro : public LiveActor
{
public:
	SphereAccelSensorController *_90; 
	TamakoroTutorial *_94;
	JointController *_98;
	Coin *mPurpleCoin;
	TQuat4f _A0;
	TQuat4f _B0;
	TVec3f _C0;
	char _CC[0xC]; // unused? Not necessarily a TVec3f.
	TVec3f _D8;
	TVec3f _E4;
	TVec3f _F0;
	TVec3f _FC;
	TVec3f _108;
	TVec3f _114;
	TVec3f _120;
	TVec3f _12C;
	f32 _138;
	f32 _13C;
	f32 _140;
	f32 _144; 
	u32 _148;
	u32 _14C;
	AudTamakoroBgmCtrl *_150;
	bool _154;
	bool _155;
	bool mIsUsePurpleCoin; // _156
	s32 mMusicNum; // CUSTOM, don't use for documentation purposes.
};