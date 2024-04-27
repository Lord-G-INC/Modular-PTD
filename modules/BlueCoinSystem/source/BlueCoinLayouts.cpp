#include "BlueCoinUtil.h"
#include "BlueCoinLayouts.h"
#include "Game/Screen/GameSceneLayoutHolder.h"
#include "Game/Screen/PauseMenuExt.h"
#include "Game/Screen/CounterLayoutControllerExt.h"


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

// Add 0x4 to CounterLayoutController for BlueCoinCounter ptr
//kmWrite32(0x80471780, 0x38600050);

CounterLayoutControllerExt* createCounterLayoutControllerExt() {
    return new CounterLayoutControllerExt();
}

CounterLayoutControllerExt::CounterLayoutControllerExt() : CounterLayoutController() {}

kmCall(0x80471780, createCounterLayoutControllerExt);
kmWrite32(0x80471784, 0x48000010);

void initBlueCoinLayout(CounterLayoutControllerExt* pController) {
    MR::connectToSceneLayout(pController);
    
    if (!MR::isStageFileSelect()) {
        pController->mPTDBlueCoinCounter = new BlueCoinCounter("BlueCoinCounter");
        pController->mPTDBlueCoinCounter->initWithoutIter();
    }
}

kmCall(0x804657A0, initBlueCoinLayout);

void appearBlueCoinLayout(CounterLayoutControllerExt* pController) {
    if (!MR::isStageFileSelect() && !BlueCoinUtil::isBlueCoinTextBoxAppeared()) {
        ((BlueCoinCounter*)pController->mPTDBlueCoinCounter)->mWaitTime = -1;
        pController->mPTDBlueCoinCounter->setNerve(&NrvBlueCoinCounter::NrvAppear::sInstance);
    } 

        pController->showAllLayout();
}

kmCall(0x80466128, appearBlueCoinLayout);

void disappearBlueCoinLayout(CounterLayoutControllerExt* pController) {
    if (!MR::isStageFileSelect() && !BlueCoinUtil::isBlueCoinTextBoxAppeared()) {
        pController->mPTDBlueCoinCounter->setNerve(&NrvBlueCoinCounter::NrvDisappear::sInstance);
    }
        
    pController->hideAllLayout();
}

kmCall(0x80466198, disappearBlueCoinLayout);

void killBlueCoinCounter(CounterLayoutControllerExt* pController) {
    if (!MR::isStageFileSelect() && !BlueCoinUtil::isBlueCoinTextBoxAppeared()) {
        MR::hideLayout(pController->mPTDBlueCoinCounter);
        pController->mPTDBlueCoinCounter->setNerve(&NrvBlueCoinCounter::NrvDisappear::sInstance);
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

wchar_t* gWStr = new wchar_t[32];
wchar_t* gCompleteIcon = new wchar_t[2];
wchar_t* gStarIcon = new wchar_t[2];
wchar_t* gBButtonIcon = new wchar_t[2];

s32 setUpBlueCoinInfo(PauseMenu* pPauseMenu) {
    setPauseMenuBlueCoinStageCount(pPauseMenu);

    s32 rangemin = BlueCoinUtil::getBlueCoinRange(0, 0);

    setUpBlueCoinFlagsInfo(pPauseMenu);

    ((PauseMenuExt*)pPauseMenu)->mDisplayMode = 0;

    if (rangemin != -1) {
        s32 totalCoins = (BlueCoinUtil::getBlueCoinRange(0, 1)-rangemin)+1;
        s32 newLineOff = 0;
        s32 collectedCount = 0;
        bool newLineAdded = 0;

        if (totalCoins > 30)
            totalCoins = 30;

        MR::addPictureFontCode(&gStarIcon[0], 0xC1);
        MR::addPictureFontCode(&gBButtonIcon[0], 0x31);

        MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinPage", gStarIcon);
        MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinBButton", gBButtonIcon);

        MR::hidePaneRecursive(pPauseMenu, "ShaCoinListWin");
        MR::hidePaneRecursive(pPauseMenu, "ShaCoinListFlags");
        MR::hidePaneRecursive(pPauseMenu, "TxtCoinComplete");

        MR::showPaneRecursive(pPauseMenu, "TxtCoinPage");
        MR::showPaneRecursive(pPauseMenu, "TxtCoinBButton");

        if (!MR::isStageNoPauseMenuStars()) {
            MR::showPaneRecursive(pPauseMenu, "Stars");
            MR::showPaneRecursive(pPauseMenu, "ScenarioTitle");
        }

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

        MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinComplete", gCompleteIcon);
        MR::setTextBoxFormatRecursive(pPauseMenu, "ShaCoinListWin", gWStr);
    }
    else
        ((PauseMenuExt*)pPauseMenu)->mDisplayMode = 3;

    return MR::getCoinNum();
}

kmCall(0x80487090, setUpBlueCoinInfo);

wchar_t* gStarIconIDList = new wchar_t[2];

void PauseMenuIDListControls(PauseMenu* pPauseMenu) {
    PauseMenuExt* pPauseMenuExt = (PauseMenuExt*)pPauseMenu;
    if (MR::testCorePadTriggerB(0) && pPauseMenuExt->mDisplayMode != 3) {
        if (pPauseMenuExt->mDisplayMode == 0) {
            if (!MR::isStageNoPauseMenuStars()) {
                MR::hidePaneRecursive(pPauseMenu, "Stars");
                MR::hidePaneRecursive(pPauseMenu, "ScenarioTitle");
            }
            MR::showPaneRecursive(pPauseMenu, "ShaCoinListWin");
            MR::showPaneRecursive(pPauseMenu, "TxtCoinComplete");
            pPauseMenuExt->mDisplayMode = 1;
        }
        else if (pPauseMenuExt->mDisplayMode == 1 || pPauseMenuExt->mDisplayMode == 2) {
            if (!MR::isStageNoPauseMenuStars()) {
                MR::showPaneRecursive(pPauseMenu, "Stars");
                MR::showPaneRecursive(pPauseMenu, "ScenarioTitle");
            }
            MR::hidePaneRecursive(pPauseMenu, "ShaCoinListWin");
            MR::hidePaneRecursive(pPauseMenu, "TxtCoinComplete");
            MR::hidePaneRecursive(pPauseMenu, "ShaCoinListFlags");
            pPauseMenuExt->mDisplayMode = 0;  
        }
        if (MR::testSubPadButtonC(0)) {
            if (!MR::isStageNoPauseMenuStars()) {
                MR::hidePaneRecursive(pPauseMenu, "Stars");
                MR::hidePaneRecursive(pPauseMenu, "ScenarioTitle");
            }
            MR::hidePaneRecursive(pPauseMenu, "ShaCoinListWin");
            MR::hidePaneRecursive(pPauseMenu, "TxtCoinComplete");
            MR::showPaneRecursive(pPauseMenu, "ShaCoinListFlags");
            pPauseMenuExt->mDisplayMode = 2;
        }

        MR::addPictureFontCode(&gStarIconIDList[0], pPauseMenuExt->mDisplayMode > 0 ? 0xC2 : 0xC1);
        MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinPage", gStarIconIDList);
    }
}

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

void initGalaxyInfoBlueCoinCount(LayoutActor* actor) {
    MR::setTextBoxFormatRecursive(actor, "ShaBlueCoinGalax", counterPictureFonts2);
}

//#ifdef NOGLE
//    kmCall(0x804A952C, initGalaxyInfoBlueCoinCount);
//

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

//#ifdef NOGLE
//kmWrite32(0x804A95E4, 0x809B003C);
//kmCall(0x804A95E8, setGalaxyInfoBlueCoinCount);
//