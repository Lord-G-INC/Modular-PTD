#include "Game/MapObj/ExtTamakoro.h"

TamakoroExt::TamakoroExt(const char* pName) : Tamakoro(pName) {
    mDummyDisplayModel = NULL;
    mItemType = -1;
    mMusicNum = -1;
    mModelId = -1;
    mBallColor = 0.0f;
}

TamakoroExt::~TamakoroExt() {}

TamakoroExt* createTamakoroExt(const char* pName) {
    return new TamakoroExt(pName);
}

kmCall(0x8033C8C4, createTamakoroExt); // bl createTamakoroExt
kmWrite32(0x8033C8C8, 0x48000014); // b 0x10

void TamakoroDeclarePowerStar(const JMapInfoIter& rIter, TamakoroExt* pTamakoro) {
    MR::getJMapInfoArg0NoInit(rIter, &pTamakoro->mItemType);
    MR::getJMapInfoArg1NoInit(rIter, &pTamakoro->mBallColor);
    MR::getJMapInfoArg2NoInit(rIter, &pTamakoro->mMusicNum);
    MR::getJMapInfoArg7NoInit(rIter, &pTamakoro->mModelId);

	if (pTamakoro->mMusicNum == -1 && MR::isEqualStageName("TamakoroSliderGalaxy"))
		pTamakoro->mMusicNum = 0;

    if (pTamakoro->mItemType == 0) {
        pTamakoro->mIsPurpleCoin = true;
    }

    if (pTamakoro->mItemType == -1) {
        MR::declarePowerStar(pTamakoro);
    }
}

kmWrite32(0x804444BC, 0x7F64DB78); // mr r4, r27
kmCall(0x804444C0, TamakoroDeclarePowerStar);
kmWrite32(0x804444C4, 0x48000014); // b 0x14

void TamakoroCreateDummyModel(TamakoroExt* pTamakoro, const JMapInfoIter& rIter) {
    MR::startBva(pTamakoro, "Tamakoro");
    MR::startBrk(pTamakoro, "Color");

    if (pTamakoro->mModelId != -1) {
        DummyDisplayModel* pModel = MR::createDummyDisplayModel(pTamakoro, rIter, -1, pTamakoro->mTranslation, TVec3f(0.0f, 0.0f, 0.0f));

        if (pModel) {
            pModel->initFixedPosition((MtxPtr)pTamakoro->getBaseMtx(), TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
            pModel->initWithoutIter();
            pTamakoro->mDummyDisplayModel = pModel;
        }
    }

    #ifdef REDCOINSYSTEM
    if (pTamakoro->mModelId == 24) {
        RedCoin* pCoin = RedCoinUtil::tryLinkToChildRedCoin(pTamakoro, rIter);
    }
    #endif
}

kmWrite32(0x8044458C, 0x7F84E378); // mr r4, r28
kmCall(0x80444590, TamakoroCreateDummyModel); // bl TamakoroCreateDummyModel


void TamakoroPurpleCoinSetBrkFrame(TamakoroExt* pTamakoro) {
    if (pTamakoro->mBallColor == -1.0f)
        MR::setBrkFrameAndStop(pTamakoro, 1.0f);
    else 
        MR::setBrkFrameAndStop(pTamakoro, pTamakoro->mBallColor);
}

kmCall(0x804445DC, TamakoroPurpleCoinSetBrkFrame);

void TamakoroDetermineAnimFrame(TamakoroExt* pTamakoro, const JMapInfoIter& rIter) {

    if (pTamakoro->mItemType == -1 && pTamakoro->mModelId == -1) {
        MR::setBvaFrameAndStop(pTamakoro, MR::hasPowerStarInCurrentStageWithDeclarer(pTamakoro->mName, -1));
    }
    else {
        MR::setBvaFrameAndStop(pTamakoro, 3.0f);
    }

    if (pTamakoro->mBallColor == -1.0f)
        MR::setBrkFrameAndStop(pTamakoro, 0.0f);
    else
        MR::setBrkFrameAndStop(pTamakoro, pTamakoro->mBallColor);
}

kmWrite32(0x804445E4, 0x7F63DB78); // mr r3, r27
kmWrite32(0x804445E8, 0x7F84E378); // mr r4, r28
kmCall(0x804445EC, TamakoroDetermineAnimFrame); // bl TamakoroDetermineAnimFrame
kmWrite32(0x804445F0, 0x48000034); // b 0x54

void TamakoroHandleBreak(TamakoroExt* pTamakoro, const char* pStr) {
    if (pTamakoro->mMusicNum != 1) // only stop music if there is star ball music playing
		MR::stopStageBGM(10);

    if (pTamakoro->mModelId == 24) {
        RedCoinUtil::tryAppearLinkedRedCoin(pTamakoro, pTamakoro->mTranslation);
    }
}


kmWrite32(0x80446B78, 0x7FC3F378); // mr r3, r30
kmCall(0x80446B7C, TamakoroHandleBreak); // bl TamakoroHandleBreak

void TamakoroHandleAppearPowerStar(TamakoroExt* pTamakoro) {
    if (pTamakoro->mMusicNum != 1) // only start last stage music if there is star ball music playing
		MR::startLastStageBGM();

    if (pTamakoro->mItemType == -1) {
        MR::requestAppearPowerStar(pTamakoro, pTamakoro->mTranslation);
    }
}

kmWrite32(0x80446C0C, 0x60000000); // nop
kmWrite32(0x80446C10, 0x7FC3F378); // mr r3, r30
kmCall(0x80446C14, TamakoroHandleAppearPowerStar);
kmWrite32(0x80446C18, 0x48000010); // b 0x10

void TamakoroStopStageBgm(TamakoroExt* pTamakoro) {
    MR::emitEffect(pTamakoro, "Break");
    MR::startActionSound(pTamakoro, "SmRideEnd", -1, -1, -1);
    MR::startSoundPlayer("SE_PV_JUMP_S", -1, -1);

	if (pTamakoro->mMusicNum != 1) {
		MR::stopStageBGM(40);
	}

    #ifdef REDCOINSYSTEM
    if (pTamakoro->mModelId == 24) {
        RedCoinUtil::tryAppearLinkedRedCoin(pTamakoro, pTamakoro->mTranslation);
    }
    #endif
}

kmCall(0x80446B48, TamakoroStopStageBgm); // bl TamakoroStopStageBgm
kmWrite32(0x80446B4C, 0x48000034); // b 0x34

void TamakoroStartStageBgm(TamakoroExt* pTamakoro) {
	const char* seqName = "BGM_TAMAKORO";
    
    if (pTamakoro->mMusicNum == 0)
        seqName = "BGM_TAMAKORO_2";

	if (pTamakoro->mMusicNum != 1) {
	    MR::startStageBGM(seqName, false);
	    pTamakoro->mAudioCtrl->init(seqName);
    }
}

kmBranch(0x80447790, TamakoroStartStageBgm); // b TamakoroStartStageBgm





StageInstrumentEntry* LoadNewStageBgm(const char* pStr1, const char* pStr2) {
    OSReport("%s, %s\n", pStr1, pStr2);

    return &gStageInstrumentTable[3];
}

kmCall(0x800854E4, LoadNewStageBgm);
kmCall(0x800855DC, LoadNewStageBgm);