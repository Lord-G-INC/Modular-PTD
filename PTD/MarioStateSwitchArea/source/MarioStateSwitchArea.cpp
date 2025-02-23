#include "MarioStateSwitchArea.h"

MarioStateSwitchArea::MarioStateSwitchArea (const char *pName) : AreaObj(pName) {
    mKeepOn = false;
    mIsDebugMode = false;
}

MarioStateSwitchArea::~MarioStateSwitchArea () {

}

void MarioStateSwitchArea::init (const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
    MR::getJMapInfoArg0WithInit(rIter, &mStatusCheckCode);
    MR::getJMapInfoArg1WithInit(rIter, &mBitfieldCheck);
    MR::getJMapInfoArg2NoInit(rIter, &mKeepOn);
    MR::getJMapInfoArg3NoInit(rIter, &mIsDebugMode);
}

void MarioStateSwitchArea::movement () {
    if (isInVolume(*MR::getPlayerPos())) {
        Mario *pMario = MarioAccess::getPlayerActor()->mMario;
        if (mIsDebugMode) 
            OSReport("MarioStateSwitchArea\nBitfield value: %d\n--------------\n", pMario->_C);
        
        if (pMario->isStatusActive(mStatusCheckCode) || pMario->_C == mBitfieldCheck) 
            onSwitchA();
        else if (!mKeepOn) 
            offSwitchA();
    }
}