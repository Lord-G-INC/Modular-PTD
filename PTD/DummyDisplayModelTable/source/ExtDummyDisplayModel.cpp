#include "syati.h"
#include "Game/MapObj/ExtDummyDisplayModel.h"

ExtDummyDisplayModel::ExtDummyDisplayModel(LiveActor *pHost, const DummyDisplayModelInfo *pInfo, int i, s32 l1, s32 l2) :
DummyDisplayModel(pHost, pInfo, i, l1, l2) {
    mSpinMode = 0;
}

void* gDummyDisplayModelTable = pt::loadArcAndFile("/SystemData/DummyDisplayModelTable.arc", "/DummyDisplayModelTable.bcsv");
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

        JMapInfo table;
        table.attach(gDummyDisplayModelTable);

        const char* modelName;
        MR::getCsvDataStr(&modelName, &table, "ModelName", modelId - 15);

        #ifdef BLUECOINSYSTEM
            if (MR::isEqualString(modelName, "BlueCoin") && BlueCoinUtil::isBlueCoinGotCurrentFile(colorId))
                modelId++;
                MR::getCsvDataStr(&modelName, &table, "ModelName", modelId - 15);
        #endif
    
        DummyDisplayModelInfo* pInfo = new DummyDisplayModelInfo;
        pInfo->mModelName = modelName;
        MR::getCsvDataStrOrNULL(&pInfo->mInitName, &table, "InitName", modelId - 15);
        MR::getCsvDataVec(&pInfo->mOffset, &table, "Offset", modelId - 15);
        MR::getCsvDataS32(&pInfo->_14, &table, "Unknown", modelId - 15);
        MR::getCsvDataStr(&pInfo->mAnimName, &table, "AnimName", modelId - 15);
        MR::getCsvDataBool(&pInfo->mHasColorChange, &table, "HasColorChange", modelId - 15);
        
        ExtDummyDisplayModel *pModel = new ExtDummyDisplayModel(pHost, pInfo, v4, modelId, colorId);
        pModel->initWithoutIter();
        MR::getCsvDataS32(&pModel->mSpinMode, &table, "UseSpin", modelId-15);
        return pModel;
    }

    // Hooks
    kmCall(0x80295BB0, tryCreateNewDummyModel);
    kmCall(0x80295C28, tryCreateNewDummyModel);
    kmCall(0x80295CAC, tryCreateNewDummyModel);
    kmCall(0x80295D04, tryCreateNewDummyModel);
    kmCall(0x80295D88, tryCreateNewDummyModel);

    // Skip repeated reading of Obj_arg7 field
    kmWrite32(0x801D0314, 0x7CA32B78); // mr r3, r5
    kmWrite32(0x801D0318, 0x60000000); // nop

    asm void spinCustomDisplayModels() {
        lwz r0, 0xA4(r30) // this->mModelId

        cmpwi r0, 0xF
        blt end

        lwz r5, 0xB4(r30) // this->mSpinMode

        li r0, -1

        cmpwi r5, 1
        beq spinCoin
        b end

        spinCoin:
        li r0, 0
        b end

        end:
    }

    kmCall(0x80295A10, spinCustomDisplayModels);
}