
#include "TimerLayout.h"
#include "Game/System/AllData/GameSequenceFunction.h"

namespace TimerLayoutData {
	const char* invalidAnimNames[3] = {
		"SuperSpinDriverStart",
		"SpaceFlyShortStart",
		"EarthenPipeIn"
	};

	const size_t animNames = sizeof(invalidAnimNames)/sizeof(invalidAnimNames[0]);
};

bool isValidPlayerState() {
	const char* pAnimStr = MR::getPlayerCurrentBckName();
	bool isInvalid = false;
	for (s32 i = 0; i < TimerLayoutData::animNames; i++) {
		if (MR::isEqualStringCase(pAnimStr, TimerLayoutData::invalidAnimNames[i]) && !isInvalid) {
			isInvalid = true;
		}
	}

	return !isInvalid;
}

TimerLayout::TimerLayout(const char* pName) : LayoutActor(pName, 0) {
}

void TimerLayout::init(const JMapInfoIter &rIter) {
	initLayoutManager("TimerLayout", 0);
	initNerve(&NrvTimerLayout::NrvWait::sInstance);
	MR::createAndAddPaneCtrl(this, "WinClearTime", 1);
	appear();

	if (MR::isStageMarioFaceShipOrWorldMap() || MR::isStageFileSelect())
		MR::hideLayout(this);
	else
		MR::showLayout(this);
}

void TimerLayout::control() {
	OSReport("%s\n", MR::getPlayerCurrentBckName());
	if (isValidPlayerState()) {
		u32 currTime = GameSequenceFunction::getResultBestTime();
		u32 savedTime = GameDataFunction::getStageSavedBestTime(MR::getCurrentStageName(), MR::getCurrentSelectedScenarioNo());
		s32 demoStarID = GameSequenceFunction::getClearedPowerStarId();
		wchar_t currTimeBuf[16];
		MR::makeClearTimeString(currTimeBuf, currTime);
		MR::setTextBoxFormatRecursive(this, "TxtClearTime", currTimeBuf);

		f32 frame = 0.0f;

		if (savedTime > 0) {
			if (currTime == savedTime)
				frame = 3.0f;
			else {
				if (currTime < savedTime) {
					if (MR::isPowerStarGetDemoActive() && demoStarID > -1 && demoStarID < 9)
						frame = 1.0f;
					else
						frame = 0.0f;
				}
				else
					frame = 2.0f;
			}
		}

		MR::startPaneAnimAndSetFrameAndStop(this, "WinClearTime", "Color", frame, 0);
	}
	else
		MR::startPaneAnimAndSetFrameAndStop(this, "WinClearTime", "Color", 4.0f, 0);
}

TimerLayout::~TimerLayout() {

}

namespace NrvTimerLayout {
	void NrvWait::execute(Spine* pSpine) const {
		TimerLayout* pActor = (TimerLayout*)pSpine->mExecutor;
	}

	NrvWait(NrvWait::sInstance);
}