#include "DolphinSwitchArea.h"

DolphinSwitchArea::DolphinSwitchArea (const char *pName) : AreaObj(pName) {

}

DolphinSwitchArea::~DolphinSwitchArea () {

}

void DolphinSwitchArea::init (const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

void DolphinSwitchArea::movement () {
    if (isInVolume(*MR::getPlayerPos()) && isDolphinEmulator()) 
        onSwitchA();
}

bool DolphinSwitchArea::isDolphinEmulator () {
    s32 fd = IOS_Open("/dev/dolphin", 0);
    if (fd >= 0) {
        IOS_Close(fd);
        return true;
    }
    if (pt::getIOSVersion() != 37) 
        return true;
    return false;
}