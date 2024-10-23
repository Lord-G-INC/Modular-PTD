#include "TrollArea.h"

extern "C" {
    void OSShutdownSystem();
    void OSReturnToMenu();
}

TrollArea::TrollArea (const char *pName) : AreaObj(pName) {
    mTrollMode = -1;
}

void TrollArea::init (const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
    MR::getJMapInfoArg0NoInit(rIter, &mTrollMode);
}

void TrollArea::movement () {
    if (isInVolume(*MR::getPlayerPos())) {
        switch (mTrollMode) {
            case -1:
               crashGame();
                break;
            case 0:
                OSShutdownSystem();
                break;
            case 1:
                OSReturnToMenu();
                break;
            case 2:
                MR::tryRumblePadVeryStrong(MarioAccess::getPlayerActor(), 0);
                break;
            case 3:
                MR::startSystemSE(getRandomSoundId(), -1, -1);
                break;
        }
    }
}

JAISoundID getRandomSoundId () {
    s32 randomSoundID;
    switch (MR::getRandom(0L, 0xDL)) {
        case 0:
            randomSoundID = MR::getRandom(0L, 0x191L);
            break;
        case 1:
            randomSoundID = MR::getRandom(0x10000L, 0x10084L);
            break;
        case 2:
            randomSoundID = MR::getRandom(0x20000L, 0x200CEL);
            break;
        case 3:
            randomSoundID = MR::getRandom(0x30000L, 0x30152L);
            break;
        case 4:
            randomSoundID = MR::getRandom(0x40000L, 0x40387L);
            break;
        case 5:
            randomSoundID = MR::getRandom(0x50000L, 0x505BAL);
            break;
        case 6:
            randomSoundID = MR::getRandom(0x60000L, 0x60089L);
            break;
        case 7:
            randomSoundID = MR::getRandom(0x70000L, 0x7007CL);
            break;
        case 8:
            randomSoundID = MR::getRandom(0x80000L, 0x8008DL);
            break;
        case 9:
            randomSoundID = MR::getRandom(0x90000L, 0x90314L);
            break;
        case 0xA:
            randomSoundID = MR::getRandom(0xA0000L, 0xA01B8L);
            break;
        case 0xB:
            randomSoundID = MR::getRandom(0xB0000L, 0xB0188L);
            break;
        case 0xC:
            randomSoundID = MR::getRandom(0xC0000L, 0xC0016L);
            break;
        case 0xD:
            randomSoundID = MR::getRandom(0xD0000L, 0xD0015L);
            break;
    }
    JAISoundID id = {randomSoundID};
    return id;
}

asm void crashGame () {
    lis r12, 0xDEAD
    ori r12, r12, 0xBEEF
    mtctr r12
    bctrl
    blr
}