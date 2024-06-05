#include "BlueCoinUtil.h"
#include "BlueCoinLayouts.h"
#include "Game/Screen/CounterLayoutControllerExt.h"
#include "Game/Screen/GameSceneLayoutHolder.h"
#include "Game/Screen/PauseMenuExt.h"

// HUD

BlueCoinCounter::BlueCoinCounter(const char* pName) : LayoutActor(pName, 0) {
    mWaitTime = -1;
    mSysInfoWindow = 0;
}

void BlueCoinCounter::init(const JMapInfoIter& rIter) {
    initLayoutManager("BlueCoinCounterInStage", 2);
    initEffectKeeper(0, 0, 0);
    MR::registerDemoSimpleCastAll(this);
    MR::connectToSceneLayout(this);
    MR::createAndAddPaneCtrl(this, "Counter", 1);
    MR::setTextBoxNumberRecursive(this, "ShaNumber", BlueCoinUtil::getTotalBlueCoinNumCurrentFile(true));

    mAppearer = new CounterLayoutAppearer(this, TVec2f(-50.0f, 0.0f));

    mPaneRumbler = new CountUpPaneRumbler(this, "Counter");
    mPaneRumbler->mRumbleCalculator->mRumbleStrength = 8.0f;
    mPaneRumbler->reset();

    appear();
    initNerve(&NrvBlueCoinCounter::NrvDisappear::sInstance);
    MR::hideLayout(this);    

    if (!BlueCoinUtil::hasSeenBlueCoinTextBoxCurrentFile()) {
        mSysInfoWindow = MR::createSysInfoWindowMiniExecuteWithChildren();
        MR::connectToSceneLayout(mSysInfoWindow);
        MR::registerDemoSimpleCastAll(mSysInfoWindow);
    }
}

void BlueCoinCounter::control() {
    if (mWaitTime > 0)
        mWaitTime--;

    if (mWaitTime == 0 && !isNerve(&NrvBlueCoinCounter::NrvShowTextBox::sInstance))
        setNerve(&NrvBlueCoinCounter::NrvDisappear::sInstance);

    mAppearer->updateNerve();
    mPaneRumbler->update();
}

void BlueCoinCounter::exeAppear() {
    if (MR::isFirstStep(this) || MR::isStep(this, 1) && mAppearer->isDisappeared()) {
        if (!mAppearer->isAppeared()) {
            MR::showLayout(this);
            mAppearer->appear(TVec2f(0.0f, 0.0f));
            MR::startAnim(this, "Wait", 1);
        }

        if (mWaitTime == 0)
            mWaitTime = -1;
    }
}
  
void BlueCoinCounter::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mAppearer->disappear();
        mWaitTime = -1;
    }

    if (MR::isStep(this, 10))
        MR::hideLayout(this);
}

void BlueCoinCounter::startCountUp() { 
    if (BlueCoinUtil::hasSeenBlueCoinTextBoxCurrentFile()) {
        if (mAppearer->isDisappeared()) {
            setNerve(&NrvBlueCoinCounter::NrvAppearAndUpdate::sInstance);
            mWaitTime = 120;
        }
        else {
            updateCounter();

            if (mWaitTime > 0)
                mWaitTime = 120;
        }
    }
    else
        setNerve(&NrvBlueCoinCounter::NrvShowTextBox::sInstance);
}   

void BlueCoinCounter::exeAppearAndUpdate() {
    if (MR::isFirstStep(this))
        exeAppear();

    if (MR::isStep(this, 15))
        updateCounter();
}

void BlueCoinCounter::updateCounter() {
    MR::setTextBoxNumberRecursive(this, "Counter", BlueCoinUtil::getTotalBlueCoinNumCurrentFile(true));
    MR::startPaneAnim(this, "Counter", "Flash", 0);
    mPaneRumbler->start();
}

void BlueCoinCounter::exeShowTextBox() {
    if (MR::isStep(this, 3)) {
        mAppearer->disappear();
        mWaitTime = -1;
        mSysInfoWindow->appear("BlueCoinCounter_OnFirstBlueCoin", SysInfoWindow::SysInfoType_0, SysInfoWindow::SysInfoTextPos_0, SysInfoWindow::SysInfoMessageType_1);
        MR::deactivateDefaultGameLayout();
        MR::hideLayout(this);
        MR::suspendAllSceneNameObj();
        mSysInfoWindow->requestResume();
        mSysInfoWindow->mIconAButton->requestResume();
        requestResume();
    }

    if (mSysInfoWindow->isDisappear()) {
        MR::resumeAllSceneNameObj();
        mWaitTime = 120;
        MR::activateDefaultGameLayout();
        BlueCoinUtil::setSeenBlueCoinTextBoxCurrentFile();
        setNerve(&NrvBlueCoinCounter::NrvAppearAndUpdate::sInstance);
    }
}

bool fixBlueCoinWindowCrash() {
    if (!MR::isStageFileSelect() && !MR::isEqualStageName("PeachCastleGalaxy") && !MR::isStageStoryBook() && !BlueCoinUtil::hasSeenBlueCoinTextBoxCurrentFile())
        return MR::isPlayerDead() || BlueCoinUtil::isBlueCoinTextBoxAppeared();

    return MR::isPlayerDead();
}

kmCall(0x80451C40, fixBlueCoinWindowCrash);

CounterLayoutControllerExt::CounterLayoutControllerExt() : CounterLayoutController() {
    mBlueCoinCounter = 0;
}

CounterLayoutControllerExt* createCounterLayoutControllerExt() {
    return new CounterLayoutControllerExt();
}

kmCall(0x80471784, createCounterLayoutControllerExt);
kmWrite32(0x80471788, 0x4800000C);

namespace NrvBlueCoinCounter {
	void NrvAppear::execute(Spine* pSpine) const {
        ((BlueCoinCounter*)pSpine->mExecutor)->exeAppear();
	}

	void NrvDisappear::execute(Spine* pSpine) const {
        ((BlueCoinCounter*)pSpine->mExecutor)->exeDisappear();
	}

	void NrvAppearAndUpdate::execute(Spine* pSpine) const {
        ((BlueCoinCounter*)pSpine->mExecutor)->exeAppearAndUpdate();
	}

	void NrvShowTextBox::execute(Spine* pSpine) const {
        ((BlueCoinCounter*)pSpine->mExecutor)->exeShowTextBox();
	}

	NrvAppear(NrvAppear::sInstance);
    NrvDisappear(NrvDisappear::sInstance);
    NrvAppearAndUpdate(NrvAppearAndUpdate::sInstance);
    NrvShowTextBox(NrvShowTextBox::sInstance);
}

void createBlueCoinCounter(CounterLayoutControllerExt* pController, const Nerve* pNerve) {
    if (!(MR::isStageFileSelect() || MR::isStageWorldMap())) {   
        pController->mBlueCoinCounter = new BlueCoinCounter("BlueCoinCounter");
        pController->mBlueCoinCounter->initWithoutIter();
    }

    pController->initNerve(pNerve);
}

kmCall(0x804657AC, createBlueCoinCounter);

void appearBlueCoinLayout(CounterLayoutControllerExt* pController) {
    if (pController->mBlueCoinCounter && !BlueCoinUtil::isBlueCoinTextBoxAppeared()) {
        pController->mBlueCoinCounter->mWaitTime = -1;
        pController->mBlueCoinCounter->setNerve(&NrvBlueCoinCounter::NrvAppear::sInstance);
    } 

    pController->showAllLayout();
}

kmCall(0x80466128, appearBlueCoinLayout);

void disappearBlueCoinLayout(CounterLayoutControllerExt* pController) {
    if (pController->mBlueCoinCounter && !BlueCoinUtil::isBlueCoinTextBoxAppeared()) {
        pController->mBlueCoinCounter->setNerve(&NrvBlueCoinCounter::NrvDisappear::sInstance);
    }
        
    pController->hideAllLayout();
}

kmCall(0x80466198, disappearBlueCoinLayout);

void killBlueCoinCounter(CounterLayoutControllerExt* pController) {
    if (pController->mBlueCoinCounter && !BlueCoinUtil::isBlueCoinTextBoxAppeared()) {
        MR::hideLayout(pController->mBlueCoinCounter);
        pController->mBlueCoinCounter->setNerve(&NrvBlueCoinCounter::NrvDisappear::sInstance);
    }

    pController->killAllCoounter();
}

kmCall(0x8046590C, killBlueCoinCounter);

// PAUSE MENU

void initBlueCoinStageCounters(PauseMenu* pPauseMenu) {
    MR::connectToSceneLayoutOnPause(pPauseMenu);

    MR::setTextBoxFormatRecursive(pPauseMenu, "ShaBlueCoinTotal", counterPictureFonts);
    MR::setTextBoxFormatRecursive(pPauseMenu, "ShaBlueCoinStage", counterPictureFonts);
}

kmCall(0x80486D60, initBlueCoinStageCounters);

void setPauseMenuBlueCoinStageCount(PauseMenu* pPauseMenu) {
    s32 rangeCollected = BlueCoinUtil::getBlueCoinRangeData(0, true);
    s32 rangeTotal = BlueCoinUtil::getBlueCoinRangeData(0, false);

    MR::setTextBoxArgNumberRecursive(pPauseMenu, "ShaBlueCoinTotal", BlueCoinUtil::getTotalBlueCoinNumCurrentFile(false), 0);

    if (rangeCollected > -1) {
        MR::setTextBoxArgNumberRecursive(pPauseMenu, "ShaBlueCoinStage", rangeCollected, 0);
    
        MR::showPaneRecursive(pPauseMenu, "ShaBlueCoinStage");

        if (rangeTotal > -1)
            BlueCoinUtil::getBlueCoinPaneNameFromTable(pPauseMenu, 0);
    }
    else
        MR::hidePaneRecursive(pPauseMenu, "ShaBlueCoinStage");
}

wchar_t* gWFlagsStr = new wchar_t[34];

void setUpBlueCoinFlagsInfo(PauseMenu* pPauseMenu) {
    bool newLineAdded = false;
    s32 newLineOff = 0;
    gWFlagsStr[33] = 0;

    for (s32 i = 0; i < 33; i++) {
        newLineAdded = false;

        if (i == 16) {
            MR::addNewLine(&gWFlagsStr[i]);
            newLineOff++;
            newLineAdded = true;
        }

        if (!newLineAdded) {
            bool isOn = BlueCoinUtil::isOnBlueCoinFlagCurrentFile(i-newLineOff);

            MR::addPictureFontCode(&gWFlagsStr[i], isOn ? 0x8A : 0xC0);
        }
    }

    MR::setTextBoxFormatRecursive(pPauseMenu, "ShaCoinListFlags", gWFlagsStr);
}

wchar_t gWStr[32];
wchar_t gCompleteIcon[2];
wchar_t gStarIcon[2];
wchar_t gBButtonIcon[2];

s32 setUpBlueCoinInfo(PauseMenu* pPauseMenu) {
    setPauseMenuBlueCoinStageCount(pPauseMenu);

    s32 rangemin = BlueCoinUtil::getBlueCoinRange(0, false);

    bool stageCheck = MR::isStageStoryBook() || MR::isStageMarioFaceShipOrWorldMap() || MR::isStageNoPauseMenuStars();

    setUpBlueCoinFlagsInfo(pPauseMenu);

    ((PauseMenuExt*)pPauseMenu)->mDisplayMode = 0;  

    if (rangemin != -1) {
        if (stageCheck)
            ((PauseMenuExt*)pPauseMenu)->mDisplayMode = 3;
        else {
            MR::addPictureFontCode(&gStarIcon[0], 0xC1);
            MR::addPictureFontCode(&gBButtonIcon[0], 0x31);

            MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinPage", gStarIcon);
            MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinBButton", gBButtonIcon);

            MR::showPaneRecursive(pPauseMenu, "Stars");
            MR::showPaneRecursive(pPauseMenu, "ScenarioTitle");
            MR::showPaneRecursive(pPauseMenu, "TxtCoinPage");
        }

        s32 totalCoins = (BlueCoinUtil::getBlueCoinRange(0, true)-rangemin)+1;
        s32 newLineOff = 0;
        s32 collectedCount = 0;
        bool newLineAdded = 0;

        if (totalCoins > 30)
            totalCoins = 30;

        MR::hidePaneRecursive(pPauseMenu, "ShaCoinListWin");
        MR::hidePaneRecursive(pPauseMenu, "ShaCoinListFlags");
        MR::hidePaneRecursive(pPauseMenu, "TxtCoinComplete");

        MR::showPaneRecursive(pPauseMenu, "TxtCoinBButton");

        for (s32 i = 0; i < totalCoins + (totalCoins > 15) ? 1 : 0; i++) {
            newLineAdded = 0;

            if (i == totalCoins/2 && totalCoins > 15) {
                MR::addNewLine(&gWStr[i]);
                newLineOff++;
                newLineAdded = true;
            }

            if (!newLineAdded) {
                bool isGot = BlueCoinUtil::isBlueCoinGotCurrentFile((i+rangemin)-newLineOff);
                int icon = 0xC0;

                if (isGot) {
                    icon = 0x8A;
                    collectedCount++;
                }

                MR::addPictureFontCode(&gWStr[i], icon);
            }
        }

        MR::addPictureFontCode(&gCompleteIcon[0], collectedCount == totalCoins ? 0x50 : 0x52);

        if (stageCheck)
            MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinBButton", gCompleteIcon);
        else
            MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinComplete", gCompleteIcon);

        MR::setTextBoxFormatRecursive(pPauseMenu, "ShaCoinListWin", gWStr);
    }
    else
        ((PauseMenuExt*)pPauseMenu)->mDisplayMode = 5;


    return MR::getCoinNum();
}

kmCall(0x80487090, setUpBlueCoinInfo);

wchar_t* gStarIconIDList = new wchar_t[2];

#define NO_STATE 0
#define COIN_LIST 1
#define FLAG_LIST 2
#define COIN_LIST_NOSTAR 3
#define FLAG_LIST_NOSTAR 4
#define LIST_DISABLED 5

void PauseMenuIDListControls(PauseMenu* pPauseMenu) {
    PauseMenuExt* pPauseMenuExt = (PauseMenuExt*)pPauseMenu;

    if (pPauseMenuExt->mDisplayMode != 5) {
        if (MR::isStageStoryBook() || MR::isStageMarioFaceShipOrWorldMap() || MR::isStageNoPauseMenuStars()) {
            MR::showPane(pPauseMenuExt, "StageInfo");
            MR::showPane(pPauseMenuExt, "WinBase");
            MR::showPane(pPauseMenu, "BlueCoinAmounts");
            MR::showPane(pPauseMenu, "CoinListIcons");
            MR::showPaneRecursive(pPauseMenuExt, "StageTitle");
            MR::setTextBoxMessageRecursive(pPauseMenuExt, "StageTitle", MR::getCurrentGalaxyNameOnCurrentLanguage());

            if (pPauseMenuExt->mDisplayMode == 3) {
                MR::hidePaneRecursive(pPauseMenuExt, "ShaCoinListFlags");
                MR::showPaneRecursive(pPauseMenuExt, "ShaCoinListWin");
                MR::showPaneRecursive(pPauseMenuExt, "TxtCoinBButton");
            }
            if (pPauseMenuExt->mDisplayMode == 4) {
                MR::showPaneRecursive(pPauseMenuExt, "ShaCoinListFlags");
                MR::hidePaneRecursive(pPauseMenuExt, "ShaCoinListWin");
                MR::hidePaneRecursive(pPauseMenuExt, "TxtCoinBButton");
            }

            if (MR::testCorePadTriggerB(0)) {
                if (pPauseMenuExt->mDisplayMode == 4)
                    pPauseMenuExt->mDisplayMode = 3;
            
                if (MR::testSubPadButtonC(0) && pPauseMenuExt->mDisplayMode == 3)
                    pPauseMenuExt->mDisplayMode = 4;
            }
        }
        else {
            if (MR::testCorePadTriggerB(0)) {
                if (pPauseMenuExt->mDisplayMode == 1 || pPauseMenuExt->mDisplayMode == 2) {
                    MR::hidePaneRecursive(pPauseMenuExt, "ShaCoinListWin");
                    MR::hidePaneRecursive(pPauseMenuExt, "TxtCoinComplete");
                    MR::hidePaneRecursive(pPauseMenuExt, "ShaCoinListFlags");

                    MR::showPaneRecursive(pPauseMenuExt, "Stars");
                    MR::showPaneRecursive(pPauseMenuExt, "ScenarioTitle");
                    pPauseMenuExt->mDisplayMode = 0;  
                }
                else if (pPauseMenuExt->mDisplayMode == 0) {
                    MR::showPaneRecursive(pPauseMenuExt, "ShaCoinListWin");
                    MR::showPaneRecursive(pPauseMenuExt, "TxtCoinComplete");

                    MR::hidePaneRecursive(pPauseMenuExt, "Stars");
                    MR::hidePaneRecursive(pPauseMenuExt, "ScenarioTitle");
                    pPauseMenuExt->mDisplayMode = 1;
                }

                if (MR::testSubPadButtonC(0)) {
                    MR::hidePaneRecursive(pPauseMenuExt, "ShaCoinListWin");
                    MR::hidePaneRecursive(pPauseMenuExt, "TxtCoinComplete");
                    MR::showPaneRecursive(pPauseMenuExt, "ShaCoinListFlags");
                    
                    MR::hidePaneRecursive(pPauseMenuExt, "Stars");
                    MR::hidePaneRecursive(pPauseMenuExt, "ScenarioTitle");
                    pPauseMenuExt->mDisplayMode = 2;
                }

                MR::addPictureFontCode(&gStarIconIDList[0], pPauseMenuExt->mDisplayMode > 0 ? 0xC2 : 0xC1);
                MR::setTextBoxFormatRecursive(pPauseMenuExt, "TxtCoinPage", gStarIconIDList);
            }
        }
    }
}

#ifndef PAUSEMENUNEWBUTTON
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

bool PauseMenuIsNewButtonPointingTrigger(PauseMenuExt* pPauseMenu) {
    PauseMenuIDListControls(pPauseMenu);
    return (pPauseMenu->mButtonTop && pPauseMenu->mButtonTop->isPointingTrigger());
}

kmWrite32(0x80487714, 0x7F63DB78); // mr r3, r27 (PauseMenuExt* into r3)
kmCall(0x80487720, PauseMenuIsNewButtonPointingTrigger);

void PauseMenuMoveButtonForBlueCoin(PauseMenuExt* pPauseMenu, const char* pStr1, const char* pStr2, f32 frame, u32 u) {
    if (pPauseMenu->mDisplayMode == 3 || pPauseMenu->mDisplayMode == 4) {
        frame = 2.0f;
    }
    MR::startPaneAnimAndSetFrameAndStop(pPauseMenu, pStr1, pStr2, frame, u);
}

kmCall(0x804874D4, PauseMenuMoveButtonForBlueCoin);
#endif

// FILE INFO

void initBlueCoinCounterFileInfo(LayoutActor* pLayout) {
    MR::connectToSceneLayout(pLayout);
    MR::showPaneRecursive(pLayout, "BlueCoinFileInfo");
    MR::setTextBoxFormatRecursive(pLayout, "ShaBlueCoinFileInfo", counterPictureFonts);
}

kmCall(0x8046D908, initBlueCoinCounterFileInfo);

void setBlueCoinCounterFileInfo(LayoutActor* pLayout, const Nerve* pNerve) {
    s32 fileID = 1;
    asm("lwz %0, 0x2C(r31)" : "=r" (fileID));

    MR::setTextBoxArgNumberRecursive(pLayout, "ShaBlueCoinFileInfo", BlueCoinUtil::getTotalBlueCoinNum(fileID - 1, false), 0);
    pLayout->setNerve(pNerve);
}

kmCall(0x8046D9BC, setBlueCoinCounterFileInfo);

#ifdef DISABLED
void initGalaxyInfoBlueCoinCount(LayoutActor* actor) {
    MR::setTextBoxFormatRecursive(actor, "ShaBlueCoinGalax", counterPictureFonts2);
}

kmCall(0x804A952C, initGalaxyInfoBlueCoinCount);


void setGalaxyInfoBlueCoinCount(LayoutActor* actor, const char* pGalaxyName, const wchar_t* pWStr) {
    MR::setTextBoxMessageRecursive(actor, "StarIcon", pWStr);

    s32 rangeNum = BlueCoinUtil::getBlueCoinRangeData(pGalaxyName, true);
    MR::hidePaneRecursive(actor, "BlueCoin");

    if (rangeNum != -1) {
        MR::showPane(actor, "BlueCoin");
        MR::showPaneRecursive(actor, "ShaBlueCoinGalax");
        BlueCoinUtil::getBlueCoinPaneNameFromTable(actor, pGalaxyName);
        MR::setTextBoxFormatRecursive(actor, "ShaBlueCoinGalax", L"%ls%d", counterPictureFonts2, rangeNum);
    }
}

kmWrite32(0x804A95E4, 0x809B003C); // lwz r4, 0x3C(r27)
kmCall(0x804A95E8, setGalaxyInfoBlueCoinCount);
#endif
