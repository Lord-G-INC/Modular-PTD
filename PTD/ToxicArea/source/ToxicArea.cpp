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

f32 gToxic = 1.0f;
u32 gTimeInside;
bool gIsMeterActive;
bool gIsSoundActive;

namespace ToxicController {
    void main() {
        if (MR::isPlayerDead())
            return;

        ToxicArea* pToxicArea = (ToxicArea*)MR::getAreaObjAtPlayerPos("ToxicArea");

        if (pToxicArea != NULL) {
            gToxic -= pToxicArea->mIncAmount;
            gTimeInside++;
        }
        else {
            gToxic += TOXIC_RESTORE_EXIT;

            if (gTimeInside != 0) {
                gIsSoundActive = true;
                MR::startSystemSE("SE_SY_LV_INC_AIR_WALK_TIMER", -1, -1);
            }

            gTimeInside = 0;
        }

        if (gToxic < 1.0f) {
            MarioSubMeter* pMarioSubMeter = MR::getGameSceneLayoutHolder()->mMarioSubMeter;

            if (gToxic <= 0.0f) {
                pMarioSubMeter->frameOutSubMeter();
                MarioAccess::forceKill(ACTMES_ENEMY_ATTACK_EXTRA_DAMAGE, 0);

                gToxic = 1.0f;
                gTimeInside = 0;
                return;
            }
            
            if (gTimeInside % 60 == 0) {
                MR::startSoundPlayer("SE_PV_TIRED", -1, -1);
            }

            if (!gIsMeterActive) {
                pMarioSubMeter->activeSubMeterWater();
                gIsMeterActive = true;
            }

            pMarioSubMeter->setWaterLifeRatio(gToxic);
            MarioAccess::getPlayerActor()->mMarioAnimator->changeTrackAnim(3, TRACK_DAMAGE_WAIT);            
        }
        else {
            if (gToxic > 1.0f) {
                gToxic = 1.0f;
            }

            if (gIsSoundActive) {
                gIsSoundActive = false;
                
                MR::stopSystemSE("SE_SY_LV_INC_AIR_WALK_TIMER", 0);
                MR::startSystemSE("SE_SY_INC_AIR_WALK_TIMER_F", -1, -1);
            }

            if (gIsMeterActive) {
                gIsMeterActive = false;
                MR::getGameSceneLayoutHolder()->mMarioSubMeter->frameOutSubMeter();
            }
        }
    }
    
    inline f32 getToxic() {
        return gToxic;
    }

    inline void setToxic(f32 value) {
        gToxic = value;
    }

    inline void incToxic(f32 value) {
        gToxic -= value;
    }

    inline void decToxic(f32 value) {
        gToxic += value;
    }

    void decToxicCoin() {
        ToxicArea* pToxicArea = (ToxicArea*)MR::getAreaObjAtPlayerPos("ToxicArea");

        if (pToxicArea != NULL)
            gToxic += pToxicArea->mCoinRestore;
    }

    void decToxicBubble() {
        ToxicArea* pToxicArea = (ToxicArea*)MR::getAreaObjAtPlayerPos("ToxicArea");

        if (pToxicArea != NULL)
            gToxic += pToxicArea->mBubbleRestore;
    }
}

kmBranch(0x8028D1D4, ToxicController::decToxicCoin);   // Coin Get
kmBranch(0x8026E9A4, ToxicController::decToxicBubble); // Bubble Break

ToxicArea::ToxicArea(const char *pName) : AreaObj(pName) { }

void ToxicArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);

    if (mObjArg0 == -1) mIncAmount = 0.002f; // = 1 / 500
    else mIncAmount = 1.0f / mObjArg0;

    if (mObjArg1 == -1) mCoinRestore = 0.2f;
    else mCoinRestore = mObjArg1 / 100.0f;

    if (mObjArg2 == -1) mBubbleRestore = 0.3f;
    else mBubbleRestore = mObjArg2 / 100.0f;
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