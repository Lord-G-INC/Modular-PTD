#include "ClipAreaDropScaleHolder.h"

ClipAreaDropScaleHolder::ClipAreaDropScaleHolder() : DeriveActorGroup<ClipAreaDropScale>("クリップエリアのしずく管理", 32) {
    ClipAreaDropScale* pClipArea = 0;
    for (int i = 0; i < 32; i++) {
        pClipArea = new ClipAreaDropScale("クリップエリアのしずくS", "VolumeSphere");
        pClipArea->initWithoutIter();
        registerActor(pClipArea);
    }
}

namespace MR {
    NameObj* createClipAreaDropScaleHolder() {
        return MR::createSceneObj(0x5A);
    }

    ClipAreaDropScale* getDeadClipAreaDropScale() {
        ClipAreaDropScaleHolder* pObj = (ClipAreaDropScaleHolder*)MR::getSceneObjHolder()->getObj(0x5A);
        ClipAreaDropScale* pClipAreaDropScale = (ClipAreaDropScale*)pObj->getDeadActor();
        return pClipAreaDropScale;
    }

    bool appearClipAreaDropScale(const TVec3f& rPos, f32 f) {
        ClipAreaDropScale* pClipArea = getDeadClipAreaDropScale();
        
        if (!pClipArea) 
            return false;

        pClipArea->mTranslation.set(rPos);
        pClipArea->setBaseSize(f);
        pClipArea->appear();
        return true;
    }
}

namespace ClipAreaDropScaleHolderUtil {
    NameObj* createClipAreaDropScaleHolder() {
        return new ClipAreaDropScaleHolder();
    }
};