#include "syati.h"
#include "Game/Util.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Ride/TamakoroExt.h"
/*
Author: AwesomeTMC
Adds a new Obj_arg to the Star Ball.
Obj_arg3 - Music to play: -1 Default Behavior, 0 Slider, 1 Don't play, 2 Normal Music
*/

TamakoroExt::TamakoroExt(const char* pName) : Tamakoro(pName) {
	mMusicNum = -1;
};

TamakoroExt::~TamakoroExt() {}

TamakoroExt* createTamakoroExt(const char* pName) {
	return new TamakoroExt(pName);
}

kmCall(0x8033C8C4, createTamakoroExt); // bl createTamakoroExt
kmWrite32(0x8033C8C8, 0x48000014); // b 0x18

// Grabs arg 3 when LiveActor is accessible.
// Overwrites a call to MR::useStageSwitchWriteA()
bool useStageSwitchWriteAAndGetArg3(TamakoroExt *pStarBall, JMapInfoIter &rIter)
{
	MR::getJMapInfoArg3NoInit(rIter, &pStarBall->mMusicNum);
	if (pStarBall->mMusicNum == -1 && MR::isEqualStageName("TamakoroSliderGalaxy"))
		pStarBall->mMusicNum = 0;
	return MR::useStageSwitchWriteA(pStarBall, rIter);
}
kmCall(0x804444E0, useStageSwitchWriteAAndGetArg3);

// Define new particles.
// These will be picked using the Star Ball's current BTP frame.
const char* cNewParticles[7] = {"BreakYellow", "BreakBronze", "BreakGreen", "BreakRed", "BreakClear", "BreakBlue", "BreakSilver"};
void TamakoroCustomPowerStarColorsParticles(LiveActor* pActor) {
	MR::emitEffect(pActor, "Break"); // Emit the original particle. The crystal particles were removed so custom ones can be added to replace it.
	MR::emitEffect(pActor, cNewParticles[(s32)MR::getBtpFrame(pActor)]); // Emit custom crystal particles picked by the current BTP frame.
}
kmCall(0x80446B4C, TamakoroCustomPowerStarColorsParticles);

// Manually overwrite the exeBindEnd code, can't change easily with asm unfortunately.
void exeBindEnd(TamakoroExt *pStarBall)
{
	if (MR::isFirstStep(pStarBall))
	{
		MR::tryRumblePadStrong(pStarBall, 0);
		MR::shakeCameraStrong();
		pStarBall->_12C.zero();
		pStarBall->_138 = 0.0f;
		if (MR::isValidSwitchA(pStarBall))
			MR::offSwitchA(pStarBall);

		MR::emitEffect(pStarBall, "Break");

		MR::startActionSound(pStarBall, "SmRideEnd", -1, -1, -1);
		MR::startSoundPlayer("SE_PV_JUMP_S", -1, -1);
		if (pStarBall->mMusicNum != 1) // only stop music if there is star ball music playing
			MR::stopStageBGM(10);
		if (pStarBall->mIsPurpleCoin) {
			pStarBall->mPurpleCoin->appearMove(pStarBall->mTranslation, pStarBall->mGravity * -30.0, -1, -1);
		}
	}
	if (MR::isGreaterStep(pStarBall, 12) && MR::isOnGroundPlayer() || MR::isGreaterStep(pStarBall, 90))
	{
		if (pStarBall->mMusicNum != 1) // only start last stage music if there is star ball music playing
			MR::startLastStageBGM();
		if (!pStarBall->mIsPurpleCoin)
			MR::requestAppearPowerStar(pStarBall, pStarBall->mTranslation);
		pStarBall->kill();
	}
}
kmBranch(0x80446AE0, exeBindEnd);

// Stop the stage's BGM music if arg 3 is not 1
void stopStageBGMIfTamakoroMusic(TamakoroExt *pStarBall)
{
	if (pStarBall->mMusicNum != 1)
	{
		MR::stopStageBGM(40);
	}
}
kmWrite32(0x804452A0, 0x7FC3F378); // change it to pass a star ball ptr
kmCall(0x804452A4, stopStageBGMIfTamakoroMusic); 

// Starts their music choice.
void newStartTamakoroBgm(TamakoroExt *pStarBall)
{
	const char *seqName;
	bool playMusic = true;
	switch (pStarBall->mMusicNum)
	{
	case 1:
		playMusic = false;
		break;
	case 0:
		seqName = "BGM_TAMAKORO_2";
		break;
	default:
		seqName = "BGM_TAMAKORO";
		break;
	}
	if (playMusic)
	{
		MR::startStageBGM(seqName, false);
		pStarBall->mAudioCtrl->init(seqName);
	}
}
kmBranch(0x80447790, newStartTamakoroBgm);