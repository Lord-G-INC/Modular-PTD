#include "StageInstrumentTable.h"

void* gStageInstrumentTableArc = pt::loadArcAndFile("/AudioRes/Info/StageInstrumentTable.arc", "/StageInstrumentTable.bcsv", 0);

void SetUpStageInstrumentEntry(StageInstrumentEntry* pEntry, const char* currSceneName, const char* currStageName) {
    pEntry->pName = "dummy";
    pEntry->pStage = "dummy";
    pEntry->_8[0] = 0;
    pEntry->_8[1] = 255;
    pEntry->_8[2] = 0;
    pEntry->_8[3] = 0;

    if (!MR::isEqualString(currSceneName, "Logo")) {
        JMapInfo info;
        info.attach(gStageInstrumentTableArc);
        s32 elementNum = MR::getCsvDataElementNum(&info);
        const char* type;
        const char* name;
        s32 targetRow = -1;

        for (s32 i = 0; i < elementNum; i++) {
            MR::getCsvDataStr(&type, &info, "Type", i);
            MR::getCsvDataStr(&name, &info, "StageName", i);

            if (MR::isEqualString(type, currSceneName) && MR::isEqualString(name, currStageName)) {
                targetRow = i;
                break;
            }
        }

        if (targetRow > -1) {
            pEntry->pName = type;
            pEntry->pStage = name;

            s32 code = 0;
            char field[10];
            
            for (s32 i = 0; i < 4; i++) {
                snprintf(field, 10, "Param%02d", i);
                MR::getCsvDataS32(&code, &info, field, targetRow);
    
                pEntry->_8[i] = (u8)code;
            }
        }
    }
}

extern "C" {
    bool unk_80085100(StageInstrumentEntry*);
    void unk_80085620(u32*, StageInstrumentEntry*, u32);
    void unk_80085510(u32*, StageInstrumentEntry*);
};

#if defined TWN || defined KOR
#define REGIONOFF 0x70
#else
#define REGIONOFF 0
#endif

void unkAudioLoad(u32* mgr, const char* pStr1, const char* pStr2, u32 u1) {
    StageInstrumentEntry entry;

    SetUpStageInstrumentEntry(&entry, pStr1, pStr2);
    if (entry.pName != 0) {
        if (unk_80085100(&entry) != 0) {
            unk_80085620(mgr, &entry, u1);
        }
    }
}

kmCall(0x804AE4C4+REGIONOFF, unkAudioLoad);


void unkAudSceneMgrLoadAudio(u32* mgr, const char* pStr1, const char* pStr2) {
    StageInstrumentEntry entry;
    SetUpStageInstrumentEntry(&entry, pStr1, pStr2);
    unk_80085510(mgr, &entry);
}

kmCall(0x804AE44C+REGIONOFF, unkAudSceneMgrLoadAudio);