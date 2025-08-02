#include "ModuleData_CustomReplaceTagFunctions.h"

namespace NewReplaceTagFunction {
    u32 appendString(wchar_t* pStr, u32 num) {
        wchar_t wbuf[8];
        swprintf(wbuf, 8, L"%d", num);
        size_t len = wcslen(wbuf);
        MR::copyString(pStr, wbuf, len);
        return len;
    }

    inline u32 getReplaceParam(const MessageEditorMessageTag& rTag) {
        return *((u16*)rTag._0+1);
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

    u32 getCoinNum(wchar_t* pStr, const MessageEditorMessageTag& rTag) {
        u32 coinNum = 0;

        if (getReplaceParam(rTag) == 0)
            coinNum = MR::getCoinNum();
        else
            coinNum = GameDataFunction::getStockedCoinNum();
        
        return appendString(pStr, coinNum);
    }

    u32 getStarPieceNum(wchar_t* pStr, const MessageEditorMessageTag& rTag) {
        u32 starPieceNum = 0;

        if (getReplaceParam(rTag) == 0)
            starPieceNum = MR::getStarPieceNum();
        else
            starPieceNum = GameDataFunction::getStockedStarPieceNum();
        
        return appendString(pStr, starPieceNum);
    }

    u32 getPurpleCoinNum(wchar_t* pStr, const MessageEditorMessageTag& rTag) {
        u32 param = getReplaceParam(rTag);
        s32 purpleCoinNum = (u32)MR::getPurpleCoinNum();

        if (param != 0) {
            purpleCoinNum = param - purpleCoinNum;
        
            if (purpleCoinNum < 0) 
                purpleCoinNum = 0;
        }
        return appendString(pStr, (u32)purpleCoinNum);
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