#pragma once

#include "syati.h"

namespace pt {
	class WaterRiseSwitch : public LiveActor
	{
	public:
		WaterRiseSwitch(const char *pName);
		virtual void init(const JMapInfoIter& rIter);
        virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
		virtual void control();
		void exeRiseWater(LiveActorGroup *);

		f32 mSpeed;
		f32 mOffsetY;
		bool mIsRiseActive;
		f32 mCurrentBckFrame;
    };
};