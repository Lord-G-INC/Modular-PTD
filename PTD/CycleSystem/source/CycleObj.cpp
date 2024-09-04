#include "CycleObj.h"
#include "Game/Util.h"

/* --- CYCLE OBJECT --- */
/*
    A cycle object used in combination with the CycleController.
    Must be linked to the CycleController using Group ID.

    Objects that must appear during this cycle part must have a set SW_AWAKE similar to this CycleObj's SW_A.
*/

CycleObj::CycleObj(const char *pName) : LiveActor(pName)
{
}

void CycleObj::init(const JMapInfoIter &rIter)
{
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    MR::joinToGroupArray(this, rIter, "CycleController", 16);

    // Get cycle obj index from Obj_arg0
    cycleObjIndex = 0;
    MR::getJMapInfoArg0NoInit(rIter, &cycleObjIndex);
    // Prevent negative index
    if (cycleObjIndex < 0)
    {
        cycleObjIndex = 0;
    }

    // Get stageBgmInfo index from Obj_arg1
    stageBgmInfoIndex = 0;
    MR::getJMapInfoArg1NoInit(rIter, &stageBgmInfoIndex);
    // Prevent negative index
    if (stageBgmInfoIndex < 0)
    {
        stageBgmInfoIndex = 0;
    }

    // Get music channel from Obj_arg2
    bgmChannel = 0;
    MR::getJMapInfoArg2NoInit(rIter, &bgmChannel);
    // Prevent negative index
    if (bgmChannel < 0)
    {
        bgmChannel = 0;
    }

    // Get music fade time from Obj_arg3
    musicFadeTime = 90;
    MR::getJMapInfoArg3NoInit(rIter, &musicFadeTime);
    // Prevent negative index
    if (musicFadeTime < 0)
    {
        musicFadeTime = 90;
    }

    // Use the SW_A
    MR::useStageSwitchReadA(this, rIter);
}

void CycleObj::activate()
{
    if (MR::isValidSwitchA(this)) {
        OSReport("--- CycleObj --- Activating SW_A for cycle obj %d\n", cycleObjIndex);
        MR::onSwitchA(this);
    }
}

void CycleObj::deactivate()
{
    if (MR::isValidSwitchA(this)) {
        OSReport("--- CycleObj --- Deactivating SW_A for cycle obj %d\n", cycleObjIndex);
        MR::offSwitchA(this);
    }
}

const char *CycleObj::getBgmName() {
    void *stageBgmInfoBCSV = pt::loadArcAndFile("/AudioRes/Info/StageBgmInfo.arc", "/StageBgmInfo.bcsv");
    JMapInfo mapInfo = JMapInfo();
    mapInfo.attach(stageBgmInfoBCSV);

    s32 tableRows = MR::getCsvDataElementNum(&mapInfo);
    const char *stageName = MR::getCurrentStageName();
    const char *rowStageName;

    s32 rowIndex;
    for (s32 i = 0; i < tableRows; i++) {
        MR::getCsvDataStr(&rowStageName, &mapInfo, "StageName", i);

        if (MR::isEqualString(stageName, rowStageName)) {
            rowIndex = i;
            break;
        }
    }

    const char *bgmName;
    char columnName[17];
    sprintf(columnName, "ChangeBgmIdName%ld", stageBgmInfoIndex);

    MR::getCsvDataStr(&bgmName, &mapInfo, columnName, rowIndex);

    return bgmName;
}