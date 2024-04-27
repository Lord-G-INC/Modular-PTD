#pragma once

#include "syati.h"
#include "Game/NPC/TalkMessageCtrl.h"

namespace pt {
    void setMessageArg(TalkMessageCtrl *pTalkCtrl, const wchar_t *pText);
    void setTextBoxArgStringNumberFontRecursive(LayoutActor* pLayout, const char* paneName, s32 num, s32 index);
};
