#include "RedCoinUtil.h"
#include "RedCoin.h"
#include "Game/LiveActor/ExtActorActionKeeper.h"

namespace RedCoinUtil {
    LiveActor* getSpecificActorFromGroup(LiveActor* pActor, const char* pName) {
		LiveActor* pObj = 0;
    	LiveActorGroup* group = MR::getGroupFromArray(pActor);
		
    	for (s32 i = 0; i < group->mNumObjs; i++) {
    	    if (MR::isEqualString(group->getActor(i)->mName, pName)) {
    	        pObj = group->getActor(i);
    	        break;
    	    }
    	}
		return pObj;
    }

	bool linkToChildRedCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter) {
		RedCoin* pCoin = (RedCoin*)NameObjFactory::initChildObj(rIter, 0);
        pCoin->requestHide();
        ((ExtActorActionKeeper*)pSourceActor->mActionKeeper)->mRedCoin = pCoin;
		pSourceActor->mActionKeeper->mItemGenerator = 0;

		if (pCoin)
       		return true;

		return false;
	}

    void appearLinkedRedCoin(LiveActor* pSourceActor, const TVec3f& rPosition) {
		RedCoin* pCoin = ((ExtActorActionKeeper*)pSourceActor->mActionKeeper)->mRedCoin;

		if (pCoin) {
			MR::setPosition(pCoin, rPosition);
			pCoin->appearAndMove();
		}
	}
}