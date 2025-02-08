#pragma once

#define KEYBOARD_KEY_COUNT 29
#define KEYBOARD_LETTER_KEY_COUNT 26
#define KEYBOARD_CHAR_KEY_COUNT 27 // Includes Space

#define KEYBOARD_SPACE_KEY_INDEX 26
#define KEYBOARD_CAPS_KEY_INDEX 27
#define KEYBOARD_BKSP_KEY_INDEX 28

#include "syati.h"

class ScreenKeyboard : public LayoutActor {
public:
    ScreenKeyboard(const char* pName, char* pText, s32 pTextLength);
    ~ScreenKeyboard();

    virtual void init(const JMapInfoIter &rIter);
    virtual void appear();
    void hide();

    void pressButton(s32 index);
    void tryInsert(char c);
    void tryBackspace();
    void tryToggleCaps();

    void updateText();
    void updateButtons();
    char getCharByIndex(s32 index);

    void exeAppear();
    void exeNormal();
    void exeHide();

    char mButtonKeyNames[KEYBOARD_KEY_COUNT][6];
    char mButtonBoxNames[KEYBOARD_KEY_COUNT][9];
    char mButtonTxtNames[KEYBOARD_KEY_COUNT][9];
    bool mIsCaps;

    char* mText;
    s32 mTextLength;
    s32 mCurrIndex;
};

namespace NrvScreenKeyboard {
    NERVE(NrvAppear);
    NERVE(NrvNormal);
    NERVE(NrvHide);
}

const char mKeyText[KEYBOARD_CHAR_KEY_COUNT] = {
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', // Top
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',      // Middle
    'z', 'x', 'c', 'v', 'b', 'n', 'm',                // Bottom
    ' '
};