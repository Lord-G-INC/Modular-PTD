#include "BlueCoinUtil.h"
#include "BlueCoinLayouts.h"
#include "Game/Screen/CounterLayoutControllerExt.h"
#include "Game/Screen/GameSceneLayoutHolder.h"
#include "Game/Screen/PauseMenuExt.h"

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

    if (MR::isStep(this, 10)) {
        MR::hideLayout(this);
        MR::setTextBoxNumberRecursive(this, "Counter", BlueCoinUtil::getTotalBlueCoinNumCurrentFile(true));
    }
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
    MR::emitEffect(this, "BlueCoinCounterInStageLight");
    MR::startPaneAnim(this, "Counter", "Flash", 0);
    mPaneRumbler->start();
}

void BlueCoinCounter::exeShowTextBox() {
    if (MR::isStep(this, 3)) {
        MR::tryStartDemoWithoutCinemaFrame((LiveActor*)this, "BlueCoinFirstTimeText");
        mAppearer->disappear();
        mWaitTime = -1;
        mSysInfoWindow->appear("BlueCoinCounter_OnFirstBlueCoin", SysInfoWindow::SysInfoType_0, SysInfoWindow::SysInfoTextPos_0, SysInfoWindow::SysInfoMessageType_1);
    }

    if (mSysInfoWindow->isDisappear() && MR::isDead(mSysInfoWindow)) {
        MR::endDemo(this, "BlueCoinFirstTimeText");
        mWaitTime = 120;
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

void initPauseMenuBlueCoin(PauseMenuExt* pPauseMenu) {
    MR::connectToSceneLayoutOnPause(pPauseMenu);

    MR::setTextBoxFormatRecursive(pPauseMenu, "ShaBlueCoinTotal", counterPictureFonts);
    MR::setTextBoxFormatRecursive(pPauseMenu, "ShaBlueCoinStage", counterPictureFonts);
}

kmCall(0x80486D60, initPauseMenuBlueCoin);

void setPauseMenuBlueCoinStageCount(PauseMenu* pPauseMenu) {
    s32 rangeCollected = BlueCoinUtil::getBlueCoinRangeData(0, true);
    s32 rangeTotal = BlueCoinUtil::getBlueCoinRangeData(0, false);

    MR::setTextBoxArgNumberRecursive(pPauseMenu, "ShaBlueCoinTotal", BlueCoinUtil::getTotalBlueCoinNumCurrentFile(false), 0);
    MR::setTextBoxNumberRecursive(pPauseMenu, "ShaBlueCoinNum", rangeTotal);

    if (rangeCollected > -1) {
        MR::setTextBoxArgNumberRecursive(pPauseMenu, "ShaBlueCoinStage", rangeCollected, 0);
    
        MR::showPaneRecursive(pPauseMenu, "ShaBlueCoinStage");
    }
    else
        MR::hidePaneRecursive(pPauseMenu, "ShaBlueCoinStage");
}

wchar_t gIDListStr[32];
wchar_t gCompleteIcon[2];
wchar_t gStarIcon[2];
wchar_t gBButtonIcon[2];

s32 setUpBlueCoinInfo(PauseMenu* pPauseMenu) {
    setPauseMenuBlueCoinStageCount(pPauseMenu);

    s32 rangemin = BlueCoinUtil::getBlueCoinRange(0, false);

    ((PauseMenuExt*)pPauseMenu)->mDisplayMode = 0;  

    if (rangemin != -1) {
        s32 totalCoins = (BlueCoinUtil::getBlueCoinRange(0, true)-rangemin)+1;
        s32 newLineOff = 0;
        s32 collectedCount = 0;
        bool newLineAdded = false;
        bool stageCheck = MR::isStageMarioFaceShipOrWorldMap() || MR::isStageNoPauseMenuStars() || MR::isStageStoryBook();

        if (totalCoins > 30)
            totalCoins = 30;

        MR::showPaneRecursive(pPauseMenu, "BlueCoinAmounts");
        MR::showPaneRecursive(pPauseMenu, "CoinListIcons");
        MR::hidePane(pPauseMenu, "TxtCoinComplete");

        if (stageCheck) {
            MR::showPane(pPauseMenu, "StageInfo");
            MR::showPane(pPauseMenu, "WinBase");
            MR::showPaneRecursive(pPauseMenu, "StageTitle");
            MR::showPaneRecursive(pPauseMenu, "ShaCoinListWin");
            MR::setTextBoxMessageRecursive(pPauseMenu, "StageTitle", MR::getCurrentGalaxyNameOnCurrentLanguage());
            MR::hidePane(pPauseMenu, "TxtCoinPage");
        }
        else  {
            MR::hidePaneRecursive(pPauseMenu, "ShaCoinListWin");
            MR::showPaneRecursive(pPauseMenu, "Stars");
            MR::showPaneRecursive(pPauseMenu, "ScenarioTitle");

            MR::addPictureFontCode(&gStarIcon[0], 0xC1);
            MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinPage", gStarIcon);

            MR::addPictureFontCode(&gBButtonIcon[0], 0x31);
            MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinBButton", gBButtonIcon);
        }

        for (s32 i = 0; i < totalCoins + (totalCoins > 15) ? 1 : 0; i++) {
            newLineAdded = 0;

            if (i == totalCoins/2 && totalCoins > 15) {
                MR::addNewLine(&gIDListStr[i]);
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

                MR::addPictureFontCode(&gIDListStr[i], icon);
            }
        }

        MR::addPictureFontCode(&gCompleteIcon[0], collectedCount == totalCoins ? 0x50 : 0x52);

        if (stageCheck)
            MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinBButton", gCompleteIcon);
        else
            MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinComplete", gCompleteIcon);

        MR::setTextBoxFormatRecursive(pPauseMenu, "ShaCoinListWin", gIDListStr);
    }
    else
        ((PauseMenuExt*)pPauseMenu)->mDisplayMode = 2;


    return MR::getCoinNum();
}

kmCall(0x80487090, setUpBlueCoinInfo);

wchar_t gStarIconIDList[2];

#define NO_STATE 0
#define COIN_LIST 1
#define COIN_LIST_NOSTAR 2
#define LIST_DISABLED 3

void PauseMenuIDListControls(PauseMenuExt* pPauseMenu) {
    bool stagecheck = MR::isStageNoPauseMenuStars() || MR::isStageStoryBook() || MR::isStageMarioFaceShipOrWorldMap();

    if (pPauseMenu->mDisplayMode != 2 && !stagecheck) {
        if (MR::testCorePadTriggerB(0)) {
            if (pPauseMenu->mDisplayMode == 0) {
                MR::hidePaneRecursive(pPauseMenu, "Stars");
                MR::hidePaneRecursive(pPauseMenu, "ScenarioTitle");
                MR::showPaneRecursive(pPauseMenu, "TxtCoinComplete");
                MR::showPaneRecursive(pPauseMenu, "ShaCoinListWin");
                pPauseMenu->mDisplayMode = 1;
            }
            else if (pPauseMenu->mDisplayMode == 1) {
                MR::showPaneRecursive(pPauseMenu, "Stars");
                MR::showPaneRecursive(pPauseMenu, "ScenarioTitle");
                MR::hidePaneRecursive(pPauseMenu, "TxtCoinComplete");
                MR::hidePaneRecursive(pPauseMenu, "ShaCoinListWin");
                pPauseMenu->mDisplayMode = 0;
            }

        MR::addPictureFontCode(&gStarIconIDList[0], pPauseMenu->mDisplayMode > 0 ? 0xC2 : 0xC1);
        MR::setTextBoxFormatRecursive(pPauseMenu, "TxtCoinPage", gStarIconIDList);
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
    if (pPauseMenu->mDisplayMode != 2) {
        frame = 2.0f;
    }
    MR::startPaneAnimAndSetFrameAndStop(pPauseMenu, pStr1, pStr2, frame, u);
}

kmCall(0x804874D4, PauseMenuMoveButtonForBlueCoin);
#endif

void initBlueCoinCounterFileInfo(LayoutActor* pLayout) {
    MR::connectToSceneLayout(pLayout);
    MR::showPaneRecursive(pLayout, "BlueCoinFileInfo");
    MR::setTextBoxFormatRecursive(pLayout, "ShaBlueCoinFileInfo", counterPictureFonts);
}

kmCall(0x8046D908, initBlueCoinCounterFileInfo);

void setBlueCoinCounterFileInfo(LayoutActor* pLayout, const char* pStr, s32 fileID) {
    MR::setTextBoxArgNumberRecursive(pLayout, "ShaBlueCoinFileInfo", BlueCoinUtil::getTotalBlueCoinNum(fileID - 1, false), 0);
    MR::startAnim(pLayout, pStr, 0);
}

kmWrite32(0x8046D9AC, 0x80BF002C);
kmCall(0x8046D9B0, setBlueCoinCounterFileInfo);

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
        MR::setTextBoxFormatRecursive(actor, "ShaBlueCoinGalax", L"%ls%d", counterPictureFonts2, rangeNum);
    }
}

kmWrite32(0x804A95E4, 0x809B003C); // lwz r4, 0x3C(r27)
kmCall(0x804A95E8, setGalaxyInfoBlueCoinCount);
#endif
