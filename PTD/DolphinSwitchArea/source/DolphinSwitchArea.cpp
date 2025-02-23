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
    if (isInVolume(*MR::getPlayerPos()) && pt::getIOSVersion() != 37) 
        onSwitchA();
}