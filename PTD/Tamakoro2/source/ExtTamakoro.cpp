#include "Game/MapObj/ExtTamakoro.h"

TamakoroExt::TamakoroExt(const char* pName) : Tamakoro(pName) {
    mDummyDisplayModel = NULL;
    mItemType = -1;
    mMusicNum = -1;
    mModelId = -1;
    mBvaFrame = -1.0f;
    mBrkFrame = -1.0f;
    mIsBind = false;
}

TamakoroExt::~TamakoroExt() {}

TamakoroExt* createTamakoroExt(const char* pName) {
    return new TamakoroExt(pName);
}

kmCall(0x8033C8C4, createTamakoroExt); // bl createTamakoroExt
kmWrite32(0x8033C8C8, 0x48000014); // b 0x10

void TamakoroDeclarePowerStar(const JMapInfoIter& rIter, TamakoroExt* pTamakoro) {
    MR::getJMapInfoArg0NoInit(rIter, &pTamakoro->mItemType);
    MR::getJMapInfoArg1NoInit(rIter, &pTamakoro->mBrkFrame);
    MR::getJMapInfoArg2NoInit(rIter, &pTamakoro->mBvaFrame);
    MR::getJMapInfoArg3NoInit(rIter, &pTamakoro->mMusicNum);
    MR::getJMapInfoArg7NoInit(rIter, &pTamakoro->mModelId);

	if (pTamakoro->mMusicNum == -1 && MR::isEqualStageName("TamakoroSliderGalaxy"))
		pTamakoro->mMusicNum = 0;
}

kmWrite32(0x804444BC, 0x7F64DB78); // mr r4, r27
kmCall(0x804444C0, TamakoroDeclarePowerStar); // bl TamakoroDeclarePowerStar
kmWrite32(0x804444C4, 0x48000014); // b 0x14

void TamakoroCreateDummyModel(TamakoroExt* pTamakoro, const JMapInfoIter& rIter) {
    if (pTamakoro->mBvaFrame != -2)
        MR::startBva(pTamakoro, "Tamakoro");

    MR::startBrk(pTamakoro, "Color");

    if (pTamakoro->mModelId != -1) {
        DummyDisplayModel* pModel = MR::createDummyDisplayModelCrystalItem(pTamakoro, rIter, pTamakoro->mTranslation, TVec3f(0.0f, 0.0f, 0.0f));

        if (pModel) {
            pModel->initFixedPosition((MtxPtr)pTamakoro->getBaseMtx(), TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
            pTamakoro->mDummyDisplayModel = pModel;
        }
    }

    switch (pTamakoro->mItemType) {
        case -1:
            MR::declarePowerStar(pTamakoro);
        break;
        case 0:
            pTamakoro->mIsPurpleCoin = true;
        break;
        case 1:
            MR::declareCoin(pTamakoro, 1);
        break;
        case 2:
            MR::declareStarPiece(pTamakoro, 6);
        break;
        case 3:
            RedCoinUtil::tryLinkToChildRedCoin(pTamakoro, rIter);
        break;
        #ifdef BLUECOINSYSTEM
        case 4:
            if (pTamakoro->mDummyDisplayModel)
                BlueCoinUtil::tryCreateBlueCoinForSpawningActorActionKeeper(pTamakoro, pTamakoro->mDummyDisplayModel->mColorFrame);
        break;
        #endif
    }
}

kmWrite32(0x8044458C, 0x7F84E378); // mr r4, r28
kmCall(0x80444590, TamakoroCreateDummyModel); // bl TamakoroCreateDummyModel


void TamakoroPurpleCoinSetBrkFrame(TamakoroExt* pTamakoro) {
    if (pTamakoro->mBrkFrame == -1.0f)
        MR::setBrkFrameAndStop(pTamakoro, 1.0f);
    else 
        MR::setBrkFrameAndStop(pTamakoro, pTamakoro->mBrkFrame);
}

kmCall(0x804445DC, TamakoroPurpleCoinSetBrkFrame);

void TamakoroDetermineAnimFrame(TamakoroExt* pTamakoro, const JMapInfoIter& rIter) {
    if (pTamakoro->mBvaFrame != -2) {
        if (pTamakoro->mItemType == -1) {
            if (pTamakoro->mBvaFrame == -1.0f)
                pTamakoro->mBvaFrame = (f32)MR::hasPowerStarInCurrentStageWithDeclarer(pTamakoro->mName, -1);

            if (pTamakoro->mBrkFrame == -1.0f)
                pTamakoro->mBrkFrame = 0.0f;
        }
    
    MR::setBvaFrameAndStop(pTamakoro, pTamakoro->mBvaFrame);
    }

    MR::setBrkFrameAndStop(pTamakoro, pTamakoro->mBrkFrame);
}

kmWrite32(0x804445E4, 0x7F63DB78); // mr r3, r27
kmWrite32(0x804445E8, 0x7F84E378); // mr r4, r28
kmCall(0x804445EC, TamakoroDetermineAnimFrame); // bl TamakoroDetermineAnimFrame
kmWrite32(0x804445F0, 0x48000034); // b 0x54

void TamakoroHandleBreak(TamakoroExt* pTamakoro, const char* pStr) {
    if (pTamakoro->mMusicNum != 3) // only stop music if there is star ball music playing
		MR::stopStageBGM(10);
}


kmWrite32(0x80446B78, 0x7FC3F378); // mr r3, r30
kmCall(0x80446B7C, TamakoroHandleBreak); // bl TamakoroHandleBreak

void TamakoroHandleAppearPowerStar(TamakoroExt* pTamakoro) {
    if (pTamakoro->mMusicNum != 3) // only start last stage music if there is star ball music playing
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
    
    if (MR::isValidSwitchDead(pTamakoro))
        MR::onSwitchDead(pTamakoro);

	if (pTamakoro->mMusicNum != 3) {
		MR::stopStageBGM(40);
	}

    pTamakoro->mIsBind = false;

    switch (pTamakoro->mItemType) {
        case 1:
            MR::appearCoinPop(pTamakoro, pTamakoro->mTranslation, 1);
        break;
        case 2:
            MR::appearStarPiece(pTamakoro, pTamakoro->mTranslation, 6, 10.0f, 40.0f, 0);
        break;
        case 3:
            RedCoinUtil::tryAppearLinkedRedCoin(pTamakoro, pTamakoro->mTranslation);
        break;
        #ifdef BLUECOINSYSTEM
        case 4:
            BlueCoinUtil::appearBlueCoinActionKeeper(pTamakoro, pTamakoro->mTranslation);
        break;
        #endif
    }
}

kmCall(0x80446B48, TamakoroStopStageBgm); // bl TamakoroStopStageBgm
kmWrite32(0x80446B4C, 0x48000034); // b 0x34

void TamakoroStartStageBgm(TamakoroExt* pTamakoro) {
    TamakoroBgmParamEntry* pEntry = &gTamakoroBgmParamTable[0];

    if (pTamakoro->mMusicNum < 3) {
        pEntry = &gTamakoroBgmParamTable[pTamakoro->mMusicNum+1];
    }

	if (pTamakoro->mMusicNum != 3) {
	    MR::startStageBGM(pEntry->mName, false);
        pTamakoro->mAudioCtrl->mBgmParam = pEntry;
	    pTamakoro->mAudioCtrl->init(pEntry->mName);
    }

    pTamakoro->mIsBind = true;
}

kmBranch(0x80447790, TamakoroStartStageBgm); // b TamakoroStartStageBgm

kmWrite32(0x80222400, 0x48000038); // b 0x38
kmWrite32(0x80222438, 0x807B0020); // lwz r3, 0x20(r27)


void TamakoroControlBgm(TamakoroExt* pTamakoro, f32 f, bool b, s32 l) {
    if (pTamakoro->mIsBind) {
        pTamakoro->mAudioCtrl->control(f, b, l);
    }
}

kmWrite32(0x8044486C, 0x7FE3FB78); // mr r3, r31
kmCall(0x80444878, TamakoroControlBgm); // bl TamakoroControlBgm