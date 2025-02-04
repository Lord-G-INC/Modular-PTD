#include "BlueCoinList.h"

BlueCoinList::BlueCoinList(const char* pName) : LayoutActor(pName, false) {

}

void BlueCoinList::init(const JMapInfoIter& rIter) {
    initLayoutManager("BlueCoinList", 2);
    MR::connectToSceneLayoutOnPause(this);
    mBackButton = new BackButtonCancelB("BackButtonBlueCoinList", 0);
    mBackButton->initWithoutIter();
    MR::connectToSceneLayoutOnPause(mBackButton);
    MR::createAndAddPaneCtrl(this, "Cursor", 1);
    initNerve(&NrvBlueCoinList::NrvInit::sInstance);
}

void BlueCoinList::appear() {
    setNerve(&NrvBlueCoinList::NrvAppear::sInstance);
    LayoutActor::appear();
}

const wchar_t* amongus[4] = {L"TemplateTestGalaxy", L"e", L"amogos", L"abominatiogus 1234356789abcdef"};

void BlueCoinList::exeAppear() {
    mBackButton->appear();
    wchar_t txt[16];
    for (s32 i = 0; i < 7; i++) {
        char paneName[8];
        snprintf(paneName, 8, "Galaxy%d", i+1);

        char txtPaneName[12];
        snprintf(txtPaneName, 12, "TxtGalaxy%d", i+1);

        MR::setTextBoxFormatRecursive(this, paneName, amongus[(i%4)]);

    }

    TBox2f box;
    MR::calcTextBoxRectRecursive(&box, this, "TxtGalaxy2");
    OSReport("Box %0.3f\n", box.mPointMax.x-260.0f);
    MR::startPaneAnimAndSetFrameAndStop(this, "Cursor", "ScaleCursor", box.mPointMax.x-260.0f, 0);



    for (s32 i = 0; i < 15; i++) {
        MR::addPictureFontCode(&txt[i], 0x8A);
    }


    txt[15] = 0;

    
    MR::setTextBoxFormatRecursive(this, "CoinListWin", txt);
    setNerve(&NrvBlueCoinList::NrvWait::sInstance);
}

void BlueCoinList::exeWait() {
    if (mBackButton->_30)
        setNerve(&NrvBlueCoinList::NrvClose::sInstance);
}

void BlueCoinList::exeClose() {
    kill();
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