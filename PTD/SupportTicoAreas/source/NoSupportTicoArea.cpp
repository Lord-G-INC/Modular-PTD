#include "NoSupportTicoArea.h"

/*
* An area that hides SupportTico when Mario enters the area and shows SupportTico when he leaves.
*/

NoSupportTicoArea::NoSupportTicoArea (const char *pName) : AreaObj(pName) {
    
}

void NoSupportTicoArea::init (const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

void NoSupportTicoArea::movement () {
    SupportTico *pSupportTico = (SupportTico *)MR::getSceneObjHolder()->getObj(SCENE_OBJ_SUPPORT_TICO);
    if (isInVolume(*MR::getPlayerPos())) {
        if (pSupportTico->isNerve(&NrvSupportTico::SupportTicoNrvBehavior::sInstance)) {
            MR::getWPad(1)->disconnected();
            pSupportTico->setNerve(&NrvSupportTico::SupportTicoNrvDisappear::sInstance);
        }
    } else {
        if (pSupportTico->isNerve(&NrvSupportTico::SupportTicoNrvDisappear::sInstance)) {
            MR::getWPad(1)->connected();
            pSupportTico->setNerve(&NrvSupportTico::SupportTicoNrvBehavior::sInstance);
            pSupportTico->requestShow();
        }
    }
}