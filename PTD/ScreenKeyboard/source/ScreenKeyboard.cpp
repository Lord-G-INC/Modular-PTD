#include "ScreenKeyboard.h"

/*
TODO: Add missing buttons
TODO: Pause the game with background blur
*/

ScreenKeyboard::ScreenKeyboard(const char* pName, char* pText, s32 textLength) : LayoutActor(pName, false) {
    mIsCaps = false;

    mText = pText;
    mTextLength = textLength;
    mCurrIndex = 0;

    for (s32 i = 0; i < KEYBOARD_KEY_COUNT; i++) {
        snprintf(mButtonKeyNames[i], 6, "Key%02d", i);
        snprintf(mButtonBoxNames[i], 9, "BoxKey%02d", i);
        snprintf(mButtonTxtNames[i], 9, "TxtKey%02d", i);
    }
}

ScreenKeyboard::~ScreenKeyboard() { }

void ScreenKeyboard::init(const JMapInfoIter &rIter) {
    MR::connectToScene(this, 0x0E /*MovementType_Layout*/, 0x0D /*CalcAnimType_Layout*/, -1, 0x48 /*DrawType_LayoutOnPause*/);

    initLayoutManager("ScreenKeyboard", 1);
    initNerve(&NrvScreenKeyboard::NrvAppear::sInstance);

    updateText();
    updateButtons();

    MR::setTextBoxMessageRecursive(this, mButtonTxtNames[KEYBOARD_SPACE_KEY_INDEX], L"Space");
    MR::setTextBoxMessageRecursive(this, mButtonTxtNames[KEYBOARD_CAPS_KEY_INDEX], L"\u2191");
    MR::setTextBoxMessageRecursive(this, mButtonTxtNames[KEYBOARD_BKSP_KEY_INDEX], L"\u2190");

    MR::onCalcAnim(this);
}

void ScreenKeyboard::appear() {
    setNerve(&NrvScreenKeyboard::NrvAppear::sInstance);
    LayoutActor::appear();
}

void ScreenKeyboard::hide() {
    setNerve(&NrvScreenKeyboard::NrvHide::sInstance);
}

void ScreenKeyboard::pressButton(s32 index) {
    switch (index) {
        case KEYBOARD_CAPS_KEY_INDEX:
            OSReport("Pressed Caps\n");
            tryToggleCaps();
            break;
        case KEYBOARD_BKSP_KEY_INDEX:
            OSReport("Pressed Backspace\n");
            tryBackspace();
            break;
        default:
            OSReport("Pressed '%c'\n", getCharByIndex(index));
            tryInsert(getCharByIndex(index));
            break;
    }

    MR::startSystemSE("SE_SY_TALK_START", -1, -1);
}

void ScreenKeyboard::tryInsert(char c) {
    if (mCurrIndex < mTextLength) {
        mText[mCurrIndex] = c;
        mCurrIndex++;

        updateText();
    }
}

void ScreenKeyboard::tryBackspace() {
    if (mCurrIndex > 0) {
        mCurrIndex--;
        mText[mCurrIndex] = '\0';

        updateText();
    }
}

void ScreenKeyboard::tryToggleCaps() {
    mIsCaps = !mIsCaps;
    updateButtons();
}

void ScreenKeyboard::updateText() {
    MR::setTextBoxFormatRecursive(this, "TxtText", L"%s", mText);
}

void ScreenKeyboard::updateButtons() {
    for (s32 i = 0; i < KEYBOARD_LETTER_KEY_COUNT; i++) {
        OSReport("(ScreenKeyboard) Updating letter key %d\n", i);
        MR::setTextBoxFormatRecursive(this, mButtonTxtNames[i], L"%c", getCharByIndex(i));
    }
}

char ScreenKeyboard::getCharByIndex(s32 index) {
    if (mIsCaps && index != KEYBOARD_SPACE_KEY_INDEX)
        return mKeyText[index] & ~32;

    return mKeyText[index];
}

void ScreenKeyboard::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        MR::startAnim(this, "Appear", 0);
    }
    else if (MR::isStep(this, 20)) {
        setNerve(&NrvScreenKeyboard::NrvNormal::sInstance);
    }
}

void ScreenKeyboard::exeNormal() {
    if (MR::testSubPadTriggerZ(0))
        tryToggleCaps();

    if (MR::testCorePadTriggerB(0))
        tryBackspace();

    for (s32 i = 0; i < KEYBOARD_KEY_COUNT; i++) {
        if (MR::testCorePadTriggerA(0) && MR::isStarPointerPointingPane(this, mButtonBoxNames[i], 0, true, NULL)) {
            pressButton(i);
        }
    }
}

void ScreenKeyboard::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Disappear", 0);
    }
    else if (MR::isStep(this, 20)) {
        MR::hideLayout(this);
    }
}

namespace NrvScreenKeyboard {
    void NrvAppear::execute(Spine* pSpine) const {
        ((ScreenKeyboard*)pSpine->mExecutor)->exeAppear();
    }

    void NrvNormal::execute(Spine* pSpine) const {
        ((ScreenKeyboard*)pSpine->mExecutor)->exeNormal();
    }

    void NrvHide::execute(Spine* pSpine) const {
        ((ScreenKeyboard*)pSpine->mExecutor)->exeHide();
    }

    NrvAppear(NrvAppear::sInstance);
    NrvNormal(NrvNormal::sInstance);
    NrvHide(NrvHide::sInstance);
}