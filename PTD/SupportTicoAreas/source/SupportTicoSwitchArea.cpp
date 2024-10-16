#include "SupportTicoSwitchArea.h"

/*
* An area that turns on its SW_A if SupportTico is active.
*/

SupportTicoSwitchArea::SupportTicoSwitchArea (const char *pName) : AreaObj(pName) {
    
}

void SupportTicoSwitchArea::init (const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

void SupportTicoSwitchArea::movement () {
    if (isInVolume(*MR::getPlayerPos()) && MR::isSupportPlayActive()) 
        onSwitchA();
    else 
        offSwitchA();
}