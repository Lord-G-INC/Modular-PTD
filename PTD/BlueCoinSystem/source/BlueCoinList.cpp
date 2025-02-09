#include "BlueCoinList.h"

BlueCoinList::BlueCoinList(const char* pName) : LayoutActor(pName, false) {
    mBackButton = 0;
    mCursorPosition = 0;
    mBlueCoinPageCount = 0;
    mRangeTable = 0;
    mCurrentPage = 1;
    mMaxPages = 1;

    for (s32 i = 0; i < 7; i++) {
        mListEntries[i] = new ListEntry;
        memset(mListEntries[i]->pStageName, 0, 48);
        mListEntries[i]->coinNum = 0;
        mListEntries[i]->rangeMin = 0;
        mListEntries[i]->rangeMax = 0;
        mListEntries[i]->isBlankSlot = false;
    }
}

void BlueCoinList::init(const JMapInfoIter& rIter) {
    initLayoutManager("BlueCoinList", 2);
    MR::connectToSceneLayoutOnPause(this);
    mBackButton = new BackButtonCancelB("BackButtonBlueCoinList", 0);
    mBackButton->initWithoutIter();
    MR::connectToSceneLayoutOnPause(mBackButton);
    MR::createAndAddPaneCtrl(this, "Cursor", 2);
    MR::createAndAddPaneCtrl(this, "WinGalaxy", 1);
    initNerve(&NrvBlueCoinList::NrvInit::sInstance);
    mRangeTable = BlueCoinUtil::getBlueCoinIDRangeTable();
}

void BlueCoinList::appear() {
    setNerve(&NrvBlueCoinList::NrvAppear::sInstance);
    LayoutActor::appear();
}

void BlueCoinList::exeAppear() {
    mBackButton->appear();
    
    populateListEntries();
    updateTextBoxes();
    updateBlueCoinTextPane();

    MR::setTextBoxGameMessageRecursive(this, "InfoTotals", "BlueCoinList_Totals");
    MR::setTextBoxGameMessageRecursive(this, "InfoPage", "BlueCoinList_Page");
    MR::setTextBoxArgNumberRecursive(this, "InfoPage", mMaxPages, 1);
    MR::startPaneAnimAndSetFrameAndStop(this, "WinGalaxy", "TotalsVisibility", 0.0f, 0);

    MR::setTextBoxNumberRecursive(this, "CounterBlueCoin", BlueCoinUtil::getTotalBlueCoinNumCurrentFile(false));

    setNerve(&NrvBlueCoinList::NrvWait::sInstance);

    mCursorPosition = 0;
    setCursorPosition(0);
}

void BlueCoinList::exeWait() {
    if (mBackButton->_30)
        setNerve(&NrvBlueCoinList::NrvClose::sInstance);

    s32 pageDirection = 0;
    s32 cursorDirection = 0;

    if (MR::testCorePadTriggerLeft(0))
        pageDirection = -1;
    if (MR::testCorePadTriggerRight(0))
        pageDirection = +1;

    if (pageDirection != 0) {
        mCurrentPage = mCurrentPage + pageDirection;
        
        if (mCurrentPage < 1) 
            mCurrentPage = mMaxPages;

        if (mCurrentPage > mMaxPages) 
            mCurrentPage = 1;

        populateListEntries();
        updateTextBoxes();
    }

    if (MR::testCorePadTriggerUp(0))
        cursorDirection = -1;
    if (MR::testCorePadTriggerDown(0))
        cursorDirection = 1;


    if (cursorDirection != 0 && pageDirection == 0) {
        mCursorPosition = (mCursorPosition +cursorDirection) % 8;

        if (mCursorPosition == -1)
            mCursorPosition = 7;
    }
    

    if (cursorDirection != 0 || pageDirection != 0) {
        setCursorPosition(mCursorPosition);
        updateBlueCoinTextPane();
    }
}

void BlueCoinList::exeClose() {
    kill();
}

void BlueCoinList::setCursorPosition(s32 slot) {
    char paneName[13];
    if (slot < 7)
        snprintf(paneName, 13, "TxtGalaxy%d", slot);
    else
        MR::copyString(paneName, "TxtTotals", 11);

    TBox2f txtPaneBox;
    MR::calcTextBoxRectRecursive(&txtPaneBox, this, paneName);
    f32 strLength = txtPaneBox.mPointMax.x-txtPaneBox.mPointMin.x;

    MR::startPaneAnimAndSetFrameAndStop(this, "Cursor", "CursorScale", strLength+32.0f, 0);
    MR::startPaneAnimAndSetFrameAndStop(this, "Cursor", "CursorPosition", (f32)slot, 1);
}

void BlueCoinList::populateListEntries() {
    for (s32 i = 0; i < 7; i++) {
        setEntryBlank(getEntry(i));
    }

    for (s32 i = 0; i < MR::getCsvDataElementNum(mRangeTable); i++) {
        s32 pageNum = 0;
        MR::getCsvDataS32(&pageNum, mRangeTable, "ListPage", i);

        if (mMaxPages < pageNum)
            mMaxPages = pageNum;

        if (pageNum == mCurrentPage) {
            s32 slotNum = 0;
            MR::getCsvDataS32(&slotNum, mRangeTable, "ListSlot", i);
            
            if (slotNum < 8) {
                const char* name = 0;
                MR::getCsvDataStrOrNULL(&name, mRangeTable, "StageName", i);

                if (name) {
                    ListEntry* entry = getEntry(slotNum-1);
                    MR::copyString(entry->pStageName, name, 48);
                    entry->rangeMin = BlueCoinUtil::getBlueCoinRange(name, false);
                    entry->rangeMax = BlueCoinUtil::getBlueCoinRange(name, true);
                    entry->coinNum = BlueCoinUtil::getBlueCoinRangeData(name, true);
                    entry->isBlankSlot = false;
                    setEntryNotBlank(entry);
                }
            }
        }
    }

    printListDebugInfo();
}

void BlueCoinList::updateTextBoxes() {
    for (s32 i = 0; i < 7; i++) {
        ListEntry* entry = getEntry(i);
        char txtPaneName[16];
        char coinMiPaneName[17];
        char coinMaPaneName[17];
        char coinGalaxy[15];
        char listPageName[22];
        s32 coinMiArg = 0;
        s32 coinMaArg = 0;

        snprintf(txtPaneName, 16, "StageGalaxy%d", i);
        snprintf(coinMiPaneName, 17, "CoinMiGalaxy%d", i);
        snprintf(coinMaPaneName, 17, "CoinMaGalaxy%d", i);
        snprintf(coinGalaxy, 15, "CoinGalaxy%d", i);

        if (!isEntryBlank(entry)) {
            MR::setTextBoxFormatRecursive(this, txtPaneName, MR::getGalaxyNameShortOnCurrentLanguage(entry->pStageName));
            MR::showPaneRecursive(this, coinGalaxy);
            coinMiArg = entry->coinNum;
            coinMaArg = (entry->rangeMax-entry->rangeMin)+1;
        }
        else {
            MR::setTextBoxFormatRecursive(this, txtPaneName, L"----");
            MR::hidePaneRecursive(this, coinGalaxy);
        }


        MR::setTextBoxGameMessageRecursive(this, coinMiPaneName, "BlueCoinList_Counter");
        MR::setTextBoxArgNumberRecursive(this, coinMiPaneName, coinMiArg, 0);
        MR::setTextBoxGameMessageRecursive(this, coinMaPaneName, "BlueCoinList_CounterMax");
        MR::setTextBoxArgNumberRecursive(this, coinMaPaneName, coinMaArg, 0);

        snprintf(listPageName, 22, "BlueCoinList_Page%d", mCurrentPage);
        MR::setTextBoxGameMessageRecursive(this, "TextPage", listPageName);
        MR::setTextBoxArgNumberRecursive(this, "InfoPage", mCurrentPage, 0);
    }

}

void BlueCoinList::updateBlueCoinTextPane() {
    wchar_t IDListStr[32];
    s32 pos = mCursorPosition;
    if (pos == 7)
        pos = 6;
    
    ListEntry* entry = getEntry(pos);
    s32 newLineOff = 0;
    bool newLineAdded = 0;

    if (!isEntryBlank(entry)) {
        s32 totalCoins = (entry->rangeMax-entry->rangeMin)+1;
        for (s32 i = 0; i < totalCoins + (s32)(totalCoins > 15); i++) {
            newLineAdded = 0;

            if (i == totalCoins/2 && totalCoins > 15) {
                MR::addNewLine(&IDListStr[i]);
                newLineOff++;
                newLineAdded = true;
            }

            if (!newLineAdded) {
                bool isGot = BlueCoinUtil::isBlueCoinGotCurrentFile((i+entry->rangeMin)-newLineOff);
                int icon = 0xC0;

                if (isGot)
                    icon = 0x8A;

                MR::addPictureFontCode(&IDListStr[i], icon);
            }
        }
    }

    MR::setTextBoxFormatRecursive(this, "CoinListWin", IDListStr);

    MR::startPaneAnimAndSetFrameAndStop(this, "WinGalaxy", "TotalsVisibility", (f32)(mCursorPosition == 7), 0);
}

BlueCoinList::ListEntry* BlueCoinList::getEntry(s32 slot) {
    if (slot < 0 || slot > 6)
        return 0;

    return mListEntries[slot];
};

bool BlueCoinList::isEntryBlank(ListEntry* pEntry) {
    return pEntry->isBlankSlot;
}

void BlueCoinList::setEntryBlank(ListEntry* pEntry) {
    pEntry->isBlankSlot = true;
}

void BlueCoinList::setEntryNotBlank(ListEntry* pEntry) {
    pEntry->isBlankSlot = false;
}

void BlueCoinList::printListDebugInfo() {
    OSReport("----ENTRIES----\n");
    for (s32 i = 0; i < 7; i++) {
        ListEntry* entry = getEntry(i);

        OSReport("Entry %d, %s, [%d - %d], %d, Blank? %s\n",
        i,
        entry->pStageName,
        entry->rangeMin, entry->rangeMax,
        entry->coinNum, 
        isEntryBlank(entry) ? "Yes" : "No");
    }

    OSReport("------------\n");
}


BlueCoinList::~BlueCoinList() {

}

namespace NrvBlueCoinList {
    void NrvInit::execute(Spine* pSpine) const {}

    void NrvAppear::execute(Spine* pSpine) const {
        ((BlueCoinList*)pSpine->mExecutor)->exeAppear();
    }

    void NrvWait::execute(Spine* pSpine) const {
        ((BlueCoinList*)pSpine->mExecutor)->exeWait();
    }

    void NrvClose::execute(Spine* pSpine) const {
        ((BlueCoinList*)pSpine->mExecutor)->exeClose();
    }


    NrvInit(NrvInit::sInstance);
    NrvAppear(NrvAppear::sInstance);
    NrvWait(NrvWait::sInstance);
    NrvClose(NrvClose::sInstance);
}


kmWrite32(0x80488638, 0x38800000);