#include "BlueCoinList.h"

BlueCoinList::BlueCoinList(const char* pName) : LayoutActor(pName, false) {
    mBackButton = 0;
    mCursorPosition = 0;
    mBlueCoinPageCount = 0;
    mRangeTable = 0;
    mCurrentPage = 1;

    for (s32 i = 0; i < 7; i++) {
        mListEntries[i] = 0;
    }
}

void BlueCoinList::init(const JMapInfoIter& rIter) {
    initLayoutManager("BlueCoinList", 2);
    MR::connectToSceneLayoutOnPause(this);
    mBackButton = new BackButtonCancelB("BackButtonBlueCoinList", 0);
    mBackButton->initWithoutIter();
    MR::connectToSceneLayoutOnPause(mBackButton);
    MR::createAndAddPaneCtrl(this, "Cursor", 2);
    initNerve(&NrvBlueCoinList::NrvInit::sInstance);
    mRangeTable = BlueCoinUtil::getBlueCoinIDRangeTable();

    for (s32 i = 0; i < 7; i++) {
        mListEntries[i] = new ListEntry;
        memset(mListEntries[i]->pStageName, 0, 48);
        mListEntries[i]->isBlankSlot = true;
    }
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

    setNerve(&NrvBlueCoinList::NrvWait::sInstance);

    mCursorPosition = 0;
    setCursorPosition(0);
    
    for (s32 i = 0; i < 7; i++) {
        ListEntry* entry = mListEntries[i];
        OSReport("%s, %d, %d, %d\n", entry->pStageName, entry->rangeMax-entry->rangeMin, entry->coinNum, entry->isBlankSlot);
    }
}

void BlueCoinList::exeWait() {
    if (mBackButton->_30)
        setNerve(&NrvBlueCoinList::NrvClose::sInstance);

    s32 pageDirection = 0;

    if (MR::testCorePadTriggerLeft(0))
        pageDirection = -1;
    if (MR::testCorePadTriggerRight(0))
        pageDirection = +1;

    if (pageDirection != 0) {
        mCurrentPage = mCurrentPage + pageDirection;
        
        if (mCurrentPage < 1) 
            mCurrentPage = 2;

        if (mCurrentPage > 2) 
            mCurrentPage = 1;

        populateListEntries();
        updateTextBoxes();
        setCursorPosition(mCursorPosition);
    }

    s32 cursorDirection = 0;

    if (MR::testCorePadTriggerUp(0))
        cursorDirection = -1;
    if (MR::testCorePadTriggerDown(0))
        cursorDirection = 1;


    if (cursorDirection != 0) {
        mCursorPosition = mCursorPosition + cursorDirection; 

        if (mCursorPosition < 0)
            mCursorPosition = 6;

        if (mCursorPosition > 6)
            mCursorPosition = 0;

        setCursorPosition(mCursorPosition);
    }

    if (cursorDirection != 0 || pageDirection != 0)
        updateBlueCoinTextPane();
}

void BlueCoinList::exeClose() {
    kill();
}

void BlueCoinList::setCursorPosition(s32 slot) {

    OSReport("%d\n", slot);

    char paneName[13];
    snprintf(paneName, 13, "TxtGalaxy%d", slot);

    TBox2f txtPaneBox;
    MR::calcTextBoxRectRecursive(&txtPaneBox, this, paneName);
    f32 strLength = txtPaneBox.mPointMax.x-txtPaneBox.mPointMin.x;

    //OSReport("%.03f - %.03f = %.03f + 32.0 = %.03f\n", txtPaneBox.mPointMax.x, txtPaneBox.mPointMin.x, strLength, strLength+32.0f);
    MR::startPaneAnimAndSetFrameAndStop(this, "Cursor", "CursorScale", strLength+32.0f, 0);
    MR::startPaneAnimAndSetFrameAndStop(this, "Cursor", "CursorPosition", (f32)slot, 1);
}

void BlueCoinList::populateListEntries() {
    for (s32 i = 0; i < 7; i++) {
        mListEntries[i]->isBlankSlot = true;
    }

    for (s32 i = 0; i < MR::getCsvDataElementNum(mRangeTable); i++) {
        s32 pageNum = 0;
        MR::getCsvDataS32(&pageNum, mRangeTable, "ListPage", i);

        if (pageNum == mCurrentPage) {
            s32 slotNum = 0;
            MR::getCsvDataS32(&slotNum, mRangeTable, "ListSlot", i);
            
            if (slotNum < 8) {
                const char* name = 0;
                MR::getCsvDataStrOrNULL(&name, mRangeTable, "StageName", i);
                OSReport("Name %s\n", name);
                if (name) {
                    MR::copyString(mListEntries[slotNum-1]->pStageName, name, 48);
                    mListEntries[slotNum-1]->rangeMin = BlueCoinUtil::getBlueCoinRange(name, false);
                    mListEntries[slotNum-1]->rangeMax = BlueCoinUtil::getBlueCoinRange(name, true);
                    mListEntries[slotNum-1]->coinNum = BlueCoinUtil::getBlueCoinRangeData(name, true);
                    mListEntries[slotNum-1]->isBlankSlot = false;
                }
            }
        }
    }
}

void BlueCoinList::updateTextBoxes() {
    for (s32 i = 0; i < 7; i++) {
        char paneName[8];
        snprintf(paneName, 8, "Galaxy%d", i);

        char txtPaneName[12];
        snprintf(txtPaneName, 12, "TxtGalaxy%d", i);


        if (!mListEntries[i]->isBlankSlot)
            MR::setTextBoxFormatRecursive(this, paneName, MR::getGalaxyNameOnCurrentLanguage(mListEntries[i]->pStageName));
        else {
            MR::setTextBoxFormatRecursive(this, paneName, L"----");
        }

    }
}

void BlueCoinList::updateBlueCoinTextPane() {
    wchar_t IDListStr[32];
    ListEntry* entry = mListEntries[mCursorPosition];
    s32 newLineOff = 0;
    bool newLineAdded = 0;

    if (!entry->isBlankSlot) {
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