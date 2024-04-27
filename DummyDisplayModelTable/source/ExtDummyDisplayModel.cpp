#include "syati.h"
//#include "pt/MapObj/BlueCoinSystem/BlueCoinUtil.h"
extern void* gDummyDisplayModelTable;

namespace pt {
    /*
    * New Dummy items
    *
    * New variants can be easily added and created here. This extends the existing set of dummy models. This replaces
    * all calls to MR::tryCreateDummyModel, but uses the original function to initialize the base game's items.
    */

    DummyDisplayModel* tryCreateNewDummyModel(LiveActor *pHost, const JMapInfoIter &rIter, s32 defaultId, int v4) {
        s32 modelId = MR::getDummyDisplayModelId(rIter, defaultId);

        if (modelId < 0) {
            return NULL;
        }

        if (modelId <= 14) {
            return MR::tryCreateDummyModel(pHost, rIter, modelId, v4);
        }

        s32 colorId = 0;

        if (MR::isValidInfo(rIter)) {
            MR::getJMapInfoArg6NoInit(rIter, &colorId);
        }

        #if defined USEBLUECOIN && !defined SM64BLUECOIN
            if (modelId == 25 && BlueCoinUtil::isBlueCoinGotCurrentFile(colorId))
                modelId++;
        #endif
    
        JMapInfo table;
        table.attach(gDummyDisplayModelTable);
        DummyDisplayModelInfo* pInfo = new DummyDisplayModelInfo;
        MR::getCsvDataStr(&pInfo->mModelName, &table, "ModelName", modelId - 15);
        MR::getCsvDataStrOrNULL(&pInfo->mInitName, &table, "InitName", modelId - 15);
        MR::getCsvDataVec(&pInfo->mOffset, &table, "Offset", modelId - 15);
        MR::getCsvDataS32(&pInfo->_14, &table, "Unknown", modelId - 15);
        MR::getCsvDataStr(&pInfo->mAnimName, &table, "AnimName", modelId - 15);
        MR::getCsvDataBool(&pInfo->mHasColorChange, &table, "HasColorChange", modelId - 15);

        DummyDisplayModel *pModel = new DummyDisplayModel(pHost, pInfo, v4, modelId, colorId);
        pModel->initWithoutIter();
        return pModel;
    }

    // Hooks
    kmCall(0x80295BB0, tryCreateNewDummyModel);
    kmCall(0x80295C28, tryCreateNewDummyModel);
    kmCall(0x80295CAC, tryCreateNewDummyModel);
    kmCall(0x80295D04, tryCreateNewDummyModel);
    kmCall(0x80295D88, tryCreateNewDummyModel);

    // Skip repeated reading of Obj_arg7 field
    kmWrite32(0x801D0314, 0x7CA32B78);
    kmWrite32(0x801D0318, 0x60000000);

    asm void spinCustomDisplayModels() {
        lwz r4, 0xA4(r30)
        li r0, 3
        addi r3, r4, -0x18
        subfc r0, r3, r0
        addze r0, r3
        subf r0, r0, r3
        andc r0, r4, r0
    }

    kmCall(0x80295A10, spinCustomDisplayModels);
}