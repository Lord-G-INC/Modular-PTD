#include "ToxicArea.h"
#include "Game/Player/MarioAnimator.h"
#include "Game/Screen/GameSceneLayoutHolder.h"

/*
PTD: The Toxic Gas from SM64 makes its way into SMG2!
  (ObjArg0) The time (in frames) it takes for the player to die inside this area.
  (ObjArg1) Coin restore amount, from 0 to 100.
  (ObjArg2) Bubble restore amount, from 0 to 100.
  
  (SW_A) Enable/disable area.
*/

ToxicArea::ToxicArea(const char *pName) : AreaObj(pName) {
    mIncAmount = 0.002f;
    mCoinRestore = 0.2f;
    mBubbleRestore = 0.3f;
}

void ToxicArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);

    if (mObjArg0 >= -1) {
        mIncAmount = 1.0f / mObjArg0;
    }

    if (mObjArg1 >= -1) {
        mCoinRestore = mObjArg1 / 100.0f;
    }

    if (mObjArg2 >= -1) {
        mBubbleRestore = mObjArg2 / 100.0f;
    }

    MR::connectToSceneAreaObj(this);
}

bool ToxicArea::isInVolume(const TVec3f &mTranslation) const {
    if (isValidSwitchA() && !isOnSwitchA()) {
        return false;
    }

    return AreaObj::isInVolume(mTranslation);
}

const char* ToxicArea::getManagerName() const {
    return "ToxicArea";
}

#define TRACK_DAMAGE_WAIT "\x83\x5F\x83\x81\x81\x5B\x83\x57\x83\x45\x83\x47\x83\x43\x83\x67"
#define TOXIC_RESTORE 0.015f // ~= 1 / 60

struct {
    f32 toxic;
    u32 time;
    bool isMeterActive;
    bool isSoundActive;
} gToxicInfo;

void initToxic() {
    gToxicInfo.toxic = 0.0f;
    gToxicInfo.time = 0;
    gToxicInfo.isMeterActive = false;
    gToxicInfo.isSoundActive = false;
}

bool updateToxic(const char *pAreaName, const TVec3f &rPosition) {
    bool isInCometHopperArea = MR::isInAreaObj(pAreaName, rPosition); // original call

    ToxicArea *pToxicArea = (ToxicArea *)MR::getAreaObjAtPlayerPos("ToxicArea");
    if (pToxicArea) {
        gToxicInfo.toxic += pToxicArea->mIncAmount;
        gToxicInfo.time++;
    }
    else {
        if (gToxicInfo.toxic > 0.0f) {
            gToxicInfo.toxic -= TOXIC_RESTORE;
        }        

        if (gToxicInfo.time != 0) {
            gToxicInfo.time = 0;
            gToxicInfo.isSoundActive = true;

            MR::startSystemSE("SE_SY_LV_INC_AIR_WALK_TIMER", -1, -1);
        }
    }

    MarioSubMeter *pMarioSubMeter = MR::getGameSceneLayoutHolder()->mMarioSubMeter;

    if (gToxicInfo.toxic > 0.0f) {
        if (gToxicInfo.toxic >= 1.0f) {
            pMarioSubMeter->frameOutSubMeter();
            MR::forceKillPlayerByGroundRace();

            initToxic();
            return isInCometHopperArea;
        }

        if (gToxicInfo.time % 60 == 0) {
            MR::startSoundPlayer("SE_PV_TIRED", -1, -1);
        }

        if (!gToxicInfo.isMeterActive) {
            gToxicInfo.isMeterActive = true;
            pMarioSubMeter->activeSubMeterWater();
        }

        pMarioSubMeter->setWaterLifeRatio(1.0f - gToxicInfo.toxic);
        MarioAccess::getPlayerActor()->mMarioAnimator->changeTrackAnim(3, TRACK_DAMAGE_WAIT);   
    }
    else {
        if (gToxicInfo.isSoundActive) {
            gToxicInfo.isSoundActive = false;
            
            MR::stopSystemSE("SE_SY_LV_INC_AIR_WALK_TIMER", 0);
            MR::startSystemSE("SE_SY_INC_AIR_WALK_TIMER_F", -1, -1);
        }

        if (gToxicInfo.isMeterActive) {
            gToxicInfo.isMeterActive = false;
            pMarioSubMeter->frameOutSubMeter();
        }

        if (gToxicInfo.toxic < 0.0f) {
            gToxicInfo.toxic = 0.0f;
        }
    }

    return isInCometHopperArea;
}

void decToxicCoin() {
    ToxicArea *pToxicArea = (ToxicArea*)MR::getAreaObjAtPlayerPos("ToxicArea");

    if (pToxicArea)
        gToxicInfo.toxic -= pToxicArea->mCoinRestore;
}

void decToxicBubble() {
    ToxicArea *pToxicArea = (ToxicArea*)MR::getAreaObjAtPlayerPos("ToxicArea");

    if (pToxicArea)
        gToxicInfo.toxic -= pToxicArea->mBubbleRestore;
}

kmBranch(0x803B7BAC, initToxic);
kmCall(0x803B26A0, updateToxic);
kmBranch(0x8028D1D4, decToxicCoin);
kmBranch(0x8026E9A4, decToxicBubble);