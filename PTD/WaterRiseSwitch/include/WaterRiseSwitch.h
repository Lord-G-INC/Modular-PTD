#pragma once

#include "syati.h"

#define EXT_SCENE_OBJ_WATER_RISE_HOLDER 71

namespace pt {
	class WaterRiseHolder : public NameObj {
		public:
		WaterRiseHolder(const char* pName);
		virtual void init(const JMapInfoIter& rIter);

		LiveActorGroup* mGroups[4];
	};

	NameObj* createWaterRiseHolder();
	WaterRiseHolder* getWaterRiseHolder();
	
	class WaterRiseSwitch : public LiveActor {
	public:
		WaterRiseSwitch(const char *pName);
		virtual void init(const JMapInfoIter& rIter);
        virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
		virtual void control();
		void exeRiseWater();

		f32 mSpeed;
		f32 mOffsetY;
		s32 mWaterRiseGroup;
		bool mIsRiseActive;
		f32 mCurrentBckFrame;
    };
};