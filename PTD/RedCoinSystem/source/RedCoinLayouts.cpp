#include "RedCoinLayouts.h"
#include "RedCoin.h"

RedCoinCounter::RedCoinCounter(const char* pName) : LayoutActor(pName, false) {
    mPaneRumbler = 0;
    mRedCoinCount = 0;
    mLayoutMode = -1;
    mHasAllRedCoins = 0;
}

void RedCoinCounter::init(const JMapInfoIter& rIter) {
    initLayoutManager("RedCoinCounter", 2);
    initEffectKeeper(0, 0, 0);

    MR::registerDemoSimpleCastAll(this);
    MR::connectToSceneLayout(this);
    MR::createAndAddPaneCtrl(this, "Counter", 1);
    MR::setTextBoxNumberRecursive(this, "Counter", 0);

    mPaneRumbler = new CountUpPaneRumbler(this, "Counter");
    mPaneRumbler->mRumbleCalculator->mRumbleStrength = 8.0f;
    mPaneRumbler->reset();

    initNerve(&NrvRedCoinCounter::NrvHide::sInstance);
}

void RedCoinCounter::control() {
    mPaneRumbler->update();
}

void RedCoinCounter::appear() {
    setNerve(&NrvRedCoinCounter::NrvAppear::sInstance);
    LayoutActor::appear();
}

void RedCoinCounter::setStarIcon(s32 starID, s32 iconID) {
    if (starID == 0) {
        iconID = 0x52;
    }
    else {
        if (!MR::hasPowerStarInCurrentStage(starID))
            iconID = 0x52;
    }

    wchar_t str;
    MR::addPictureFontCode(&str, iconID);
    MR::setTextBoxFormatRecursive(this, "TxtStar", &str);
}

void RedCoinCounter::startCountUp(s32 count, bool hasAllCoins, bool isRedCoinSwitchUsed) {
    mRedCoinCount = count;
    mHasAllRedCoins = hasAllCoins;

    if (mRedCoinCount == 1 && mLayoutMode == -1 && !isRedCoinSwitchUsed) {
        LayoutActor::appear();
        setNerve(&NrvRedCoinCounter::NrvAppearWithUpdate::sInstance);
    }
    else 
        setNerve(&NrvRedCoinCounter::NrvCountUp::sInstance);
}

void RedCoinCounter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::startAnim(this, "Wait", 1);
    }
}

void RedCoinCounter::exeAppearWithUpdate() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::startAnim(this, "Wait", 1);
    }

    if (MR::isStep(this, 30))
        setNerve(&NrvRedCoinCounter::NrvCountUp::sInstance);
}

void RedCoinCounter::exeDisappear() {
    if (MR::isFirstStep(this))
        MR::startAnim(this, "End", 0);

    if (MR::isStep(this, 60))
        kill();
}

void RedCoinCounter::exeCountUp() {
    if (MR::isFirstStep(this)) {
        MR::setTextBoxNumberRecursive(this, "Counter", mRedCoinCount);
        MR::emitEffect(this, "RedCoinCounterLight");
        mPaneRumbler->start();

    if (mHasAllRedCoins)
        setNerve(&NrvRedCoinCounter::NrvComplete::sInstance);
    else 
        MR::startPaneAnim(this, "Counter", "Flash", 0);
    }
}

void RedCoinCounter::exeComplete() {
    if (MR::isFirstStep(this))
        MR::startPaneAnim(this, "Counter", "FlashLoop", 0);

    if (MR::isStep(this, 120))
        setNerve(&NrvRedCoinCounter::NrvDisappear::sInstance);
}

namespace NrvRedCoinCounter {
    void NrvAppear::execute(Spine* pSpine) const {
        ((RedCoinCounter*)pSpine->mExecutor)->exeAppear();
    }

    void NrvAppearWithUpdate::execute(Spine* pSpine) const {
        ((RedCoinCounter*)pSpine->mExecutor)->exeAppearWithUpdate();
    }

    void NrvDisappear::execute(Spine* pSpine) const {
        ((RedCoinCounter*)pSpine->mExecutor)->exeDisappear();
    }

    void NrvCountUp::execute(Spine* pSpine) const {
        ((RedCoinCounter*)pSpine->mExecutor)->exeCountUp();
    }

    void NrvComplete::execute(Spine* pSpine) const {
        ((RedCoinCounter*)pSpine->mExecutor)->exeComplete();
    }

    void NrvHide::execute(Spine* pSpine) const {
    }

    NrvAppear(NrvAppear::sInstance);
    NrvAppearWithUpdate(NrvAppearWithUpdate::sInstance);
    NrvDisappear(NrvDisappear::sInstance);
    NrvCountUp(NrvCountUp::sInstance);
    NrvComplete(NrvComplete::sInstance);
    NrvHide(NrvHide::sInstance);
}

RedCoinCounterPlayer::RedCoinCounterPlayer(const char* pName) : LayoutActor(pName, false) {
    mLastRedCoin = 0;
    mNumCoins = 0;
}

void RedCoinCounterPlayer::init(const JMapInfoIter& rIter) {
    initLayoutManager("RedCoinCounterPlayer", 2);
    MR::registerDemoSimpleCastAll(this);
    MR::connectToSceneLayout(this);
    MR::createAndAddPaneCtrl(this, "Text00", 1);
    MR::setTextBoxNumberRecursive(this, "Text00", 0);
    initNerve(&NrvRedCoinCounterPlayer::NrvHide::sInstance);
}

void RedCoinCounterPlayer::appear() {
    setNerve(&NrvRedCoinCounterPlayer::NrvAppear::sInstance);
    LayoutActor::appear();
}

void RedCoinCounterPlayer::kill() {
    setNerve(&NrvRedCoinCounterPlayer::NrvHide::sInstance);
    LayoutActor::kill();
}

void RedCoinCounterPlayer::exeAppear() {
    calcScreenPos();

    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::setTextBoxNumberRecursive(this, "TxtText", mNumCoins);
    }
    
    MR::killAtAnimStopped(this, 0);
}
 
void RedCoinCounterPlayer::calcScreenPos() {
    TVec3f pos;
    TVec3f pos2;
    f32 heightAdd;

    if (((RedCoin*)mLastRedCoin)->mRedCoinCounterPlayerPos) {
        pos = *MarioAccess::getPlayerActor()->getGravityVec();
        pos2 = *MR::getPlayerPos();
        heightAdd = 200.0f;
    }
    else {
        pos = mLastRedCoin->mGravity;
        pos2 = mLastRedCoin->mTranslation;
        heightAdd = 150.0f;
    }

    TVec2f screenPos;
    TVec3f newPos;

    JMAVECScaleAdd((Vec*)&pos, (Vec*)&pos2, (Vec*)&newPos, -heightAdd);
    
    MR::calcScreenPosition(&screenPos, newPos);
    setTrans(screenPos);
}

namespace NrvRedCoinCounterPlayer {
    void NrvRedCoinCounterPlayer::NrvHide::execute(Spine* pSpine) const {}

    void NrvRedCoinCounterPlayer::NrvAppear::execute(Spine* pSpine) const {
        ((RedCoinCounterPlayer*)pSpine->mExecutor)->exeAppear();
    }

    NrvHide(NrvHide::sInstance);
    NrvAppear(NrvAppear::sInstance);
}