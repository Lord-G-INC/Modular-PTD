#include "syati.h"
#include "Game/Screen/PauseMenuExt.h"

PauseMenuExt* createPauseMenuExt() {
    return new PauseMenuExt();
}

kmCall(0x804712C0, createPauseMenuExt);
kmWrite32(0x804712C4, 0x48000010); // b 0x10

PauseMenuExt::PauseMenuExt() : PauseMenu() {
    mButtonNew = 0;
    mButtonNewFollowPos = TVec2f(0.0f, 0.0f);
    mDisplayMode = 0;
    mIsUsedNewButton = false;
}

#if defined TWN || defined KOR
#define REGIONOFF 0x10
#else
#define REGIONOFF 0
#endif

//SMSS #define STAGE_CHECK MR::isStageMarioFaceShipOrWorldMap() || (MR::isEqualStageName("YosshiHomeGalaxy") && MR::getCurrentScenarioNo() == 1) || MR::isEqualStageName("PrisonGalaxy")

#ifdef SMSS
    #define STAGE_CHECK MR::isStageMarioFaceShipOrWorldMap() || MR::isEqualStageName("PrisonGalaxy")
#else
    #define STAGE_CHECK MR::isStageMarioFaceShipOrWorldMap() || MR::isEqualStageName("PeachCastleGalaxy")
#endif


void setButtonAnimNames(ButtonPaneController* pButton) {
    if (pButton) {
        pButton->mAnimNameAppear = "ButtonAppear_restartbutton";
        pButton->mAnimNameDecide = "ButtonDecide_restartbutton";
        pButton->mAnimNameEnd = "ButtonEnd_restartbutton";
        pButton->mAnimNameSelectIn = "ButtonSelectIn_restartbutton";
        pButton->mAnimNameSelectOut = "ButtonSelectOut_restartbutton";
        pButton->mAnimNameWait = "ButtonWait_restartbutton";
    }
}

void PauseMenuInitNewButton(PauseMenuExt* pPauseMenu, const Nerve* pNerve) { 
    pPauseMenu->mButtonNew = 0;
    pPauseMenu->mIsUsedNewButton = 0;
    pPauseMenu->mButtonNewFollowPos = TVec2f(0.0f, 0.0f);

    MR::createAndAddPaneCtrl(pPauseMenu, "NewButton", 1);

    bool isNewCometInStage = false;

    #ifdef PMNBCOMETRESTRICTION
        GalaxyStatusAccessor gsa = MR::makeGalaxyStatusAccessor(MR::getCurrentStageName());
        isNewCometInStage = gsa.isValidComet(MR::getCurrentScenarioNo()) && !gsa.hasPowerStar(MR::getCurrentScenarioNo());
    #endif

    if (!(STAGE_CHECK) || !isNewCometInStage) {
        pPauseMenu->mButtonNew = new ButtonPaneController(pPauseMenu, "NBackNew", "BoxButton4", 0, 1);
        pPauseMenu->mButtonNew->mFadeAfterSelect = false;
    
        MR::setTextBoxFormatRecursive(pPauseMenu, "Text4", L"Restart Stage");

        MR::setFollowPos(&pPauseMenu->mButtonNewFollowPos, pPauseMenu, "NewButton");
        MR::setFollowTypeReplace(pPauseMenu, "NewButton");
        MR::showPaneRecursive(pPauseMenu, "NewButton");
        
        setButtonAnimNames(pPauseMenu->mButtonTop);
        setButtonAnimNames(pPauseMenu->mButtonBottom);
        setButtonAnimNames(pPauseMenu->mButtonNew);
    }
    else
        MR::hidePaneRecursive(pPauseMenu, "NBackNew");

    pPauseMenu->initNerve(pNerve);
}

kmCall(0x8048702C+REGIONOFF, PauseMenuInitNewButton);

void ButtonControl(TVec2f* pPos, PauseMenuExt* pPauseMenu, const char* pStr) {
    MR::copyPaneTrans(&pPauseMenu->mButtonTopFollowPos, pPauseMenu, pStr);

    if (pPauseMenu->mButtonNew) {
        MR::copyPaneTrans(&pPauseMenu->mButtonNewFollowPos, pPauseMenu, "NewButtonPos");
        pPauseMenu->mButtonNew->update();
    }
}

kmCall(0x8048727C+REGIONOFF, ButtonControl);

void PauseMenuSetButtonPosition(PauseMenuExt* pPauseMenu, const char* pStr1, const char* pStr2, f32 frame, u32 u) {
    #ifdef BLUECOINSYSTEM
    if (pPauseMenu->mDisplayMode != 2) {
        frame = 2.0f;
    }
    #endif
    MR::startPaneAnimAndSetFrameAndStop(pPauseMenu, pStr1, pPauseMenu->mButtonNew ? "ButtonPosition_restartbutton" : pStr2, frame, u);
}

kmCall(0x804874D4, PauseMenuSetButtonPosition);

void ForceToWaitNewButton(PauseMenuExt* pPauseMenu) {
    pPauseMenu->mButtonTop->forceToWait();

    if (pPauseMenu->mButtonNew)
        pPauseMenu->mButtonNew->forceToWait();
}

kmWrite32(0x80487504+REGIONOFF, 0x60000000); // nop (Skip overwriting r3)
kmCall(0x80487508+REGIONOFF, ForceToWaitNewButton);

void PauseMenuAppearNewButton(PauseMenuExt* pPauseMenu) {
    pPauseMenu->mButtonTop->appear();

    if (pPauseMenu->mButtonNew)
        pPauseMenu->mButtonNew->appear();
}

kmWrite32(0x80487560+REGIONOFF, 0x7FE3FB78); // mr r3, r31 (PauseMenuExt* into r3)
kmCall(0x80487564+REGIONOFF, PauseMenuAppearNewButton);

bool IsNewButtonPressed(PauseMenuExt* pPauseMenu) {
    bool isPressed = false;

    if (pPauseMenu->mButtonBottom && pPauseMenu->mButtonBottom->trySelect()) {
        pPauseMenu->mIsUsedNewButton = false;
        isPressed = true;
    }

    if (pPauseMenu->mButtonNew && pPauseMenu->mButtonNew->trySelect()) {
        pPauseMenu->mIsUsedNewButton = true;
        isPressed = true;
    }

    return isPressed;
}

kmWrite32(0x804877B4+REGIONOFF, 0x7F63DB78); // mr r3, r27 (PauseMenuExt* into r3)
kmCall(0x804877C0+REGIONOFF, IsNewButtonPressed);

void PauseMenuSetInfoWindowStr(PauseMenuExt* pPauseMenu, const char* pStr) {
    pPauseMenu->mSysInfoWindow->appear(pStr, SysInfoWindow::SysInfoType_2, SysInfoWindow::SysInfoTextPos_0, SysInfoWindow::SysInfoMessageType_1);

    if (pPauseMenu->mIsUsedNewButton) 
        MR::setTextBoxFormatRecursive(pPauseMenu->mSysInfoWindow, "TxtConfirm", L"Restart current stage?");
}

kmWrite32(0x80487C4C+REGIONOFF, 0x7FE3FB78); // mr r3, r31 (PauseMenuExt* into r4)
kmCall(0x80487C5C+REGIONOFF, PauseMenuSetInfoWindowStr);

void DisappearNewButton(PauseMenuExt* pPauseMenu) {
    pPauseMenu->mButtonTop->disappear();

    if (pPauseMenu->mButtonNew)
        pPauseMenu->mButtonNew->disappear();
}

kmWrite32(0x80487B10+REGIONOFF, 0x7FC3F378); // mr r3, r30 (PauseMenuExt* into r3)
kmCall(0x80487B14+REGIONOFF, DisappearNewButton);

bool IsNewButtonTimingForSelectedSE(PauseMenuExt* pPauseMenu) {
    return (pPauseMenu->mButtonBottom && pPauseMenu->mButtonBottom->isTimingForSelectedSe()) || (pPauseMenu->mButtonNew && pPauseMenu->mButtonNew->isTimingForSelectedSe());
}

kmWrite32(0x804879C8+REGIONOFF, 0x7FC3F378); // mr r3, r30
kmCall(0x804879CC+REGIONOFF, IsNewButtonTimingForSelectedSE);

bool PauseMenuValidateButton(PauseMenuExt* pPauseMenu) {
    return (pPauseMenu->mButtonBottom && pPauseMenu->mButtonBottom->_24) || (pPauseMenu->mButtonNew && pPauseMenu->mButtonNew->_24);
}

kmWrite32(0x804879B0+REGIONOFF, 0x7FC3F378); // mr r3, r30
kmCall(0x804879B4+REGIONOFF, PauseMenuValidateButton);
kmWrite32(0x804879B8+REGIONOFF, 0x2C030000); // cmpwi r3, 0
kmWrite32(0x804879BC+REGIONOFF, 0x41820080); // beq 0x80
kmWrite32(0x804879C0+REGIONOFF, 0x48000008); // b 0x8


bool IsNewButtonDecidedWait(PauseMenuExt* pPauseMenu) {
    return (pPauseMenu->mButtonBottom && pPauseMenu->mButtonBottom->isDecidedWait()) || (pPauseMenu->mButtonNew && pPauseMenu->mButtonNew->isDecidedWait());
}

kmWrite32(0x80487A00+REGIONOFF, 0x7FC3F378); // mr r3, r30 (PauseMenuExt* into r3)
kmCall(0x80487A04+REGIONOFF, IsNewButtonDecidedWait);

kmWrite32(0x80487A3C+REGIONOFF, 0x807E0034); // lwz r3, 0x34(r30)

void DoNewButtonAction(PauseMenuExt* pPauseMenu, bool isValid) {
    if (!pPauseMenu->mIsUsedNewButton)
        isValid ? GameSequenceFunction::notifyToGameSequenceProgressToEndScene() : GameSequenceFunction::requestChangeStageWorldMap();
    else {
        GameSequenceFunction::requestChangeScenarioSelect(MR::getCurrentStageName());
		GameSequenceFunction::requestChangeStage(MR::getCurrentStageName(), MR::getCurrentScenarioNo(), MR::getCurrentSelectedScenarioNo(), JMapIdInfo(0, 0));
    }
}

kmWrite32(0x80487CA4+REGIONOFF, 0x7C641B78); // mr r4, r3 (isValid into r4)
kmWrite32(0x80487CA8+REGIONOFF, 0x7FE3FB78); // mr r3, r31 (PauseMenuExt* into r3)
kmCall(0x80487CAC+REGIONOFF, DoNewButtonAction);
kmWrite32(0x80487CB0+REGIONOFF, 0x48000008); // b 0x8 (Skip useless instructions)

void addStarPointerMovePositionNewButton(PauseMenuExt* pPauseMenu, const char* pStr, TVec2f* pOffsetVec) {
    StarPointerUtil::addStarPointerMovePositionFromPane(pPauseMenu, pStr, pOffsetVec);
    
    if (pPauseMenu->mButtonNew)
        StarPointerUtil::addStarPointerMovePositionFromPane(pPauseMenu, "BoxButton4", pOffsetVec);
}

kmCall(0x804875F0, addStarPointerMovePositionNewButton);

void setupNewConnection1to2(PauseMenuExt* pPauseMenu) {
    if (pPauseMenu->mButtonNew)
        StarPointerUtil::setConnectionMovePositionDown2Way("BoxButton4", "BoxButton2");

    StarPointerUtil::setConnectionMovePositionDown2Way("BoxButton1", "BoxButton2");
}

kmWrite32(0x80487640+REGIONOFF, 0x60000000); // nop
kmWrite32(0x80487644+REGIONOFF, 0x7FC3F378); // mr r3, r30

kmCall(0x80487648, setupNewConnection1to2);
void setupButtonConnection(PauseMenuExt* pPauseMenu) {
    StarPointerUtil::setDefaultAllMovePosition("BoxButton1");

    if (pPauseMenu->mButtonTop && pPauseMenu->mButtonNew) {
        StarPointerUtil::setConnectionMovePositionRight2Way("BoxButton1", "BoxButton4");
    }
}

#ifdef BLUECOINSYSTEM
extern void PauseMenuIDListControls(PauseMenuExt* pPauseMenu);
#endif

kmWrite32(0x804876C0+REGIONOFF, 0x7FC3F378); // mr r3, r30
kmCall(0x804876C4+REGIONOFF, setupButtonConnection);

bool PauseMenuIsNewButtonPointingTrigger(PauseMenuExt* pPauseMenu) {
    #ifdef BLUECOINSYSTEM
        PauseMenuIDListControls(pPauseMenu);
    #endif
    
    return (pPauseMenu->mButtonTop && pPauseMenu->mButtonTop->isPointingTrigger()) || (pPauseMenu->mButtonNew && pPauseMenu->mButtonNew->isPointingTrigger());
}

kmWrite32(0x80487714+REGIONOFF, 0x7F63DB78); // mr r3, r27 (PauseMenuExt* into r3)
kmCall(0x80487720+REGIONOFF, PauseMenuIsNewButtonPointingTrigger);

#ifdef WIP
s32 gRestartTimer = 0;

void handleRestartStageButtonCombination(NerveExecutor* pExecutor) {
    pExecutor->updateNerve();

    if (MR::testSubPadButtonC(0) && MR::testCorePadButtonDown(0) && !STAGE_CHECK)
        gRestartTimer++;
    else
        gRestartTimer = 0;

    if (gRestartTimer == 30) {
        GameSequenceFunction::requestChangeScenarioSelect(MR::getCurrentStageName());
		GameSequenceFunction::requestChangeStage(MR::getCurrentStageName(), MR::getCurrentScenarioNo(), MR::getCurrentSelectedScenarioNo(), JMapIdInfo(0, 0));
        gRestartTimer = 0;
    }
}

kmCall(0x804518B0, handleRestartStageButtonCombination);
#endif