#include "BlueCoinUtil.h"
#include "BlueCoinLayouts.h"
#include "Game/Screen/CounterLayoutControllerExt.h"
#include "Game/Screen/GameSceneLayoutHolder.h"
#include "Game/Screen/PauseMenuExt.h"

#if defined TWN || defined KOR
#define REGIONOFF 0x10
#else
#define REGIONOFF 0
#endif

BlueCoinCounter::BlueCoinCounter(const char* pName) : LayoutActor(pName, 0) {
    mSysInfoWindow = 0;
    mBlueCoinDisplayNum = 0;
    _28 = 0;
    mIsAppear = 0;
}

void BlueCoinCounter::init(const JMapInfoIter& rIter) {
    initLayoutManager("BlueCoinCounterInStage", 2);
    initEffectKeeper(0, 0, 0);
    MR::connectToSceneLayout(this);
    MR::registerDemoSimpleCastAll(this);
    MR::createAndAddPaneCtrl(this, "Counter", 1);
    s32 blueCoinNum = BlueCoinUtil::getTotalBlueCoinNumCurrentFile(true);
    MR::setTextBoxNumberRecursive(this, "Counter", blueCoinNum);
    mBlueCoinDisplayNum = blueCoinNum;

    #ifdef SMSS
        mAppearer = new CounterLayoutAppearer(this, TVec2f(0.0f, 0.0f));
    #else
        mAppearer = new CounterLayoutAppearer(this, TVec2f(-50.0f, 0.0f));
    #endif

    mPaneRumbler = new CountUpPaneRumbler(this, "Counter");
    mPaneRumbler->mRumbleCalculator->mRumbleStrength = 8.0f;

    initNerve(&NrvBlueCoinCounter::NrvHide::sInstance);   

    if (!BlueCoinUtil::hasSeenBlueCoinTextBoxCurrentFile()) {
        mSysInfoWindow = MR::createSysInfoWindowMiniExecuteWithChildren();
        MR::connectToSceneLayout(mSysInfoWindow);
        MR::registerDemoSimpleCastAll(mSysInfoWindow);
    }

    appear();
}

void BlueCoinCounter::appear() {
    mAppearer->reset();
    mPaneRumbler->reset();
    _28 = 0;
    mIsAppear = false;
    MR::hideLayout(this);
    setNerve(&NrvBlueCoinCounter::NrvHide::sInstance);
    LayoutActor::appear();
}

void BlueCoinCounter::forceAppear() {
    if (!isNerve(&NrvBlueCoinCounter::NrvWait::sInstance)) {
        appear();
        setNerve(&NrvBlueCoinCounter::NrvAppear::sInstance);
    }

    mIsAppear = true;
}

void BlueCoinCounter::disappear() {
    mIsAppear = false;
    setNerve(&NrvBlueCoinCounter::NrvDisappear::sInstance);
}

void BlueCoinCounter::control() {
    updateCounter();
    mAppearer->updateNerve();
    mPaneRumbler->update();
}

void BlueCoinCounter::updateCounter() {
    s32 blueCoinNum = BlueCoinUtil::getTotalBlueCoinNumCurrentFile(true);
    s32 var = _28;
    mBlueCoinCount = blueCoinNum;

    if (var > 0) {
        _28 = var - 1;
    }
    else {
        if (mBlueCoinDisplayNum != blueCoinNum && !isNerve(&NrvBlueCoinCounter::NrvShowTextBox::sInstance)) {
            if (isNerve(&NrvBlueCoinCounter::NrvWait::sInstance)) {
                u32 v4 = mBlueCoinDisplayNum;
                _28 = 3;

                if (mBlueCoinDisplayNum < blueCoinNum)
                    mBlueCoinDisplayNum = v4 + 1;
                else 
                    mBlueCoinDisplayNum = v4 - 1;

                MR::startAnim(this, "Flash", 0);
                MR::emitEffect(this, "BlueCoinCounterInStageLight");
                mPaneRumbler->start();
            }

            if (!isNerve(&NrvBlueCoinCounter::NrvAppear::sInstance)) {
                if (!isNerve(&NrvBlueCoinCounter::NrvWait::sInstance))
                    setNerve(&NrvBlueCoinCounter::NrvAppear::sInstance);
                else
                    setNerve(&NrvBlueCoinCounter::NrvWait::sInstance);
            }
        }
    }

    MR::setTextBoxNumberRecursive(this, "Counter", mBlueCoinDisplayNum);
}

void BlueCoinCounter::setCounter() {
    s32 blueCoinNum = BlueCoinUtil::getTotalBlueCoinNumCurrentFile(true);
    mBlueCoinDisplayNum = blueCoinNum;
    mBlueCoinCount = blueCoinNum;
}

void BlueCoinCounter::exeHide() {
    if (MR::isFirstStep(this)) {
        _28 = 0;
        MR::hideLayout(this);
    }
}

void BlueCoinCounter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        mAppearer->appear(TVec2f(0.0f, 0.0f));
        MR::startAnim(this, "Wait", 1);
    }

    if (mAppearer->isAppeared()) {
        setNerve(&NrvBlueCoinCounter::NrvWait::sInstance);
    }
}

void BlueCoinCounter::exeWait() {
    if (!mIsAppear && mBlueCoinDisplayNum == mBlueCoinCount) {
        if (CounterLayoutController::isWaitToDisappearCounter(this)) {
            setNerve(&NrvBlueCoinCounter::NrvDisappear::sInstance);
        }
    }
}

void BlueCoinCounter::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mAppearer->disappear();
    }

    if (mAppearer->isDisappeared()) {
        setNerve(&NrvBlueCoinCounter::NrvHide::sInstance);
    }
}

void BlueCoinCounter::exeShowTextBox() {
    if (MR::isFirstStep(this)) {
        MR::hideLayout(this);
        MR::tryStartDemoWithoutCinemaFrame((LiveActor*)this, "BlueCoinFirstTimeText");
    }

    if (MR::isStep(this, 3))
        mSysInfoWindow->appear("BlueCoinCounter_OnFirstBlueCoin", SysInfoWindow::SysInfoType_0, SysInfoWindow::SysInfoTextPos_0, SysInfoWindow::SysInfoMessageType_1);

    if (mSysInfoWindow->isDisappear() && MR::isDead(mSysInfoWindow)) {
        MR::endDemo(this, "BlueCoinFirstTimeText");
        BlueCoinUtil::setSeenBlueCoinTextBoxCurrentFile();
        setNerve(&NrvBlueCoinCounter::NrvAppear::sInstance);
    }
}

BlueCoinCounter::~BlueCoinCounter() {

}


bool fixBlueCoinWindowCrash() {
    if (!MR::isStageFileSelect() && !MR::isEqualStageName("PeachCastleGalaxy") && !MR::isStageStoryBook() && !BlueCoinUtil::hasSeenBlueCoinTextBoxCurrentFile())
        return MR::isPlayerDead() || BlueCoinUtil::isBlueCoinTextBoxAppeared();

    return MR::isPlayerDead();
}

//kmCall(0x80451C40, fixBlueCoinWindowCrash);

CounterLayoutControllerExt::CounterLayoutControllerExt() : CounterLayoutController() {
    mBlueCoinCounter = 0;
}

CounterLayoutControllerExt* createCounterLayoutControllerExt() {
    return new CounterLayoutControllerExt();
}

kmCall(0x80471784, createCounterLayoutControllerExt);
kmWrite32(0x80471788, 0x4800000C);

namespace NrvBlueCoinCounter {
	void NrvHide::execute(Spine* pSpine) const {
        ((BlueCoinCounter*)pSpine->mExecutor)->exeHide();
    }

	void NrvAppear::execute(Spine* pSpine) const {
        ((BlueCoinCounter*)pSpine->mExecutor)->exeAppear();
    }

	void NrvWait::execute(Spine* pSpine) const {
        ((BlueCoinCounter*)pSpine->mExecutor)->exeWait();
    }

	void NrvDisappear::execute(Spine* pSpine) const {
        ((BlueCoinCounter*)pSpine->mExecutor)->exeDisappear();
    }

	void NrvShowTextBox::execute(Spine* pSpine) const {
        ((BlueCoinCounter*)pSpine->mExecutor)->exeShowTextBox();
    }

	NrvHide(NrvHide::sInstance);
    NrvAppear(NrvAppear::sInstance);
    NrvWait(NrvWait::sInstance);
    NrvDisappear(NrvDisappear::sInstance);
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

bool isBlueCoinCounterDead(CounterLayoutControllerExt* pController) {
    return (MR::isDead(pController->mCoinCounter) || MR::isDead(pController->mBlueCoinCounter));
}

kmWrite32(0x80465C00, 0x7F23CB78); // mr r3, r25
kmCall(0x80465C04, isBlueCoinCounterDead);

bool isBlueCoinCounterHidden(CounterLayoutControllerExt* pController) {
    return (MR::isHiddenLayout(pController->mCoinCounter) || MR::isHiddenLayout(pController->mBlueCoinCounter));
}

kmWrite32(0x80465C10, 0x7F23CB78); // mr r3, r25
kmCall(0x80465C14, isBlueCoinCounterHidden);


bool isBlueCoinCounterWait(CounterLayoutControllerExt* pController) {
    return (pController->mCoinCounter->isWait() || pController->mBlueCoinCounter->isNerve(&NrvBlueCoinCounter::NrvWait::sInstance));
}

kmWrite32(0x80465C20, 0x7F23CB78); // mr r3, r25
kmCall(0x80465C24, isBlueCoinCounterWait);


void appearBlueCoinLayout(CounterLayoutControllerExt* pController) {
    if (pController->mBlueCoinCounter) {
        pController->mBlueCoinCounter->forceAppear();
    } 

    pController->showAllLayout();
}


kmCall(0x80466128, appearBlueCoinLayout);


void appearBlueCoinLayoutWithoutStar(CounterLayoutControllerExt* pController) {
    pController->mCoinCounter->appear();

    if (pController->mBlueCoinCounter) {
        pController->mBlueCoinCounter->appear();
    } 
}

kmCall(0x80465EC4, appearBlueCoinLayoutWithoutStar);
kmWrite32(0x80465EC8, 0x48000010); // b 0x10

void disappearBlueCoinLayout(CounterLayoutControllerExt* pController) {
    if (pController->mBlueCoinCounter) {
        pController->mBlueCoinCounter->disappear();
    }
        
    pController->hideAllLayout();
}

kmCall(0x80466198, disappearBlueCoinLayout);

void killBlueCoinCounter(CounterLayoutControllerExt* pController) {
    if (pController->mBlueCoinCounter && !MR::isDemoActive("BlueCoinFirstTimeText")) {
        pController->mBlueCoinCounter->kill();
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

kmCall(0x80486D60+REGIONOFF, initPauseMenuBlueCoin);


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

kmCall(0x80487090+REGIONOFF, setUpBlueCoinInfo);

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

kmWrite32(0x80487714+REGIONOFF, 0x7F63DB78); // mr r3, r27 (PauseMenuExt* into r3)
kmCall(0x80487720+REGIONOFF, PauseMenuIsNewButtonPointingTrigger);

void PauseMenuMoveButtonForBlueCoin(PauseMenuExt* pPauseMenu, const char* pStr1, const char* pStr2, f32 frame, u32 u) {
    if (pPauseMenu->mDisplayMode != 2) {
        frame = 2.0f;
    }
    MR::startPaneAnimAndSetFrameAndStop(pPauseMenu, pStr1, pStr2, frame, u);
}

kmCall(0x804874D4+REGIONOFF, PauseMenuMoveButtonForBlueCoin);
#endif

void initBlueCoinCounterFileInfo(LayoutActor* pLayout) {
    MR::connectToSceneLayout(pLayout);
    MR::showPaneRecursive(pLayout, "BlueCoinFileInfo");
    MR::setTextBoxFormatRecursive(pLayout, "ShaBlueCoinFileInfo", counterPictureFonts);
}

kmCall(0x8046D908, initBlueCoinCounterFileInfo);

void setBlueCoinCounterFileInfo(LayoutActor* pLayout, const char* pStr, s32 fileID) {
    MR::setTextBoxArgNumberRecursive(pLayout, "ShaBlueCoinFileInfo", BlueCoinUtil::getTotalBlueCoinNum(fileID - 1, false), 0);
    MR::setTextBoxNumberRecursive(pLayout, pStr, fileID);
}

kmCall(0x8046DCF8, setBlueCoinCounterFileInfo);

#ifdef DISABLED
void initGalaxyInfoBlueCoinCount(LayoutActor* actor) {
    MR::setTextBoxFormatRecursive(actor, "ShaBlueCoinGalax", counterPictureFonts2);
}

#if defined TWN || defined KOR
kmCall(0x804A952C, initGalaxyInfoBlueCoinCount);
#else
kmCall(0x804A952C, initGalaxyInfoBlueCoinCount);
#endif

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
