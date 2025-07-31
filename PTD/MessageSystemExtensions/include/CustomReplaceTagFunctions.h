#pragma once

#include "syati.h"

struct ReplaceTagFuncEntry {
    u8 mBytes[4];
    u32 (*mFuncPtr)(wchar_t* pStr, const MessageEditorMessageTag& rMsgTag);
};

namespace ReplaceTagProcessor {
    u32 exePictureGroup(wchar_t*, const MessageEditorMessageTag&);
    u32 exeRaceTimeGroup(wchar_t*, const MessageEditorMessageTag&);
    u32 exeLocalizeGroup(wchar_t*, const MessageEditorMessageTag&);
};

namespace NewReplaceTagFunctions {
    u32 getPowerStarNum(wchar_t* pStr, const MessageEditorMessageTag& rTag);
    u32 getTicoCoinNum(wchar_t* pStr, const MessageEditorMessageTag& rTag);
    u32 getMissCount(wchar_t* pStr, const MessageEditorMessageTag& rTag);
};