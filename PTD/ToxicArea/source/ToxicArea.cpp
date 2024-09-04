#include "ToxicUtil.h"
#include "ToxicArea.h"
#include "Game/Player/MarioAnimator.h"
#include "Game/Screen/GameSceneLayoutHolder.h"

ToxicArea::ToxicArea(const char *pName) : AreaObj(pName) {
    mIsMeterActive = false;
}

void ToxicArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);

    if (mObjArg0 == -1) {
        mDeathToxicLevel = 500;
    } else {
        mDeathToxicLevel = (u16)(mObjArg0 & 0xFFFF);
    }
}

void ToxicArea::movement() {
    if (isInVolume(*MR::getPlayerPos())) {
        GameSceneLayoutHolder *pLayoutHolder = MR::getGameSceneLayoutHolder();
        MarioAnimator *pMarioAnimator = MarioAccess::getPlayerActor()->mMarioAnimator;
        MarioSubMeter *pMarioSubMeter = pLayoutHolder->mMarioSubMeter;

        if (!mIsMeterActive) {
            pMarioSubMeter->activeSubMeterWater();
            mIsMeterActive = true;
        }

        ToxicUtil::incToxic();

        pMarioAnimator->changeTrackAnim(3, "ダメージウエイト");
        pMarioSubMeter->setWaterLifeRatio(1.0f - ToxicUtil::getToxic() / mDeathToxicLevel);

        if (ToxicUtil::getToxic() % 60 == 0) {
            MR::startSoundPlayer("SE_PV_TIRED", -1, -1);
        }

        if (ToxicUtil::getToxic() >= mDeathToxicLevel) {
            pMarioSubMeter->frameOutSubMeter();
            MarioAccess::forceKill(ACTMES_ENEMY_ATTACK_EXTRA_DAMAGE, 0);

            ToxicUtil::setToxic(mDeathToxicLevel);
        }
    } else {
        if (mIsMeterActive) {
            GameSceneLayoutHolder *pLayoutHolder = MR::getGameSceneLayoutHolder();
            MarioSubMeter *pMarioSubMeter = pLayoutHolder->mMarioSubMeter;

            ToxicUtil::setToxic(0);

            pMarioSubMeter->frameOutSubMeter();
            mIsMeterActive = false;
        }
    }
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