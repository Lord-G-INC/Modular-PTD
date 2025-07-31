#include "ModuleData_CustomReplaceTagFunctions.h"

namespace NewReplaceTagFunctions {
    u32 appendString(wchar_t* pStr, u32 num) {
        wchar_t wbuf[8];
        swprintf(wbuf, 8, L"%d", num);
        size_t len = wcslen(wbuf);
        MR::copyString(pStr, wbuf, len);
        return len;
    }

    u32 getPowerStarNum(wchar_t* pStr, const MessageEditorMessageTag& rTag) {
        return appendString(pStr, (u32)MR::getPowerStarNum());
    }

    u32 getTicoCoinNum(wchar_t* pStr, const MessageEditorMessageTag& rTag) {
        return appendString(pStr, (u32)GameDataFunction::calcTicoCoinNum());
    }

    u32 getMissCount(wchar_t* pStr, const MessageEditorMessageTag& rTag) {
        return appendString(pStr, (u32)GameDataFunction::getPlayerMissNum());
    }
}

ReplaceTagFuncEntry* ReplTagExpTable(int index) {
    ReplaceTagFuncEntry* pEntry = 0;
    for (s32 i = 0; i < cReplaceTagFuncTableSize; i++) {
        pEntry = &cReplaceTagFuncTable[i];

        if (pEntry->mBytes[0] == index) 
            return pEntry;
    }

    return 0;
}

kmCall(0x8048C7FC, ReplTagExpTable);