#pragma once

#include "syati.h"

	class MorphSwitchCtrl : public NameObj {
	public:
		MorphSwitchCtrl(const char *pName);

		virtual void init(const JMapInfoIter &rIter);
		virtual void movement();

		StageSwitchCtrl* mStageSwitchCtrl;
		s32 mMorph;
		bool mDeactivate;
		bool mAction;
	};
