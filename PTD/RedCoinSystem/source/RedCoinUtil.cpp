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

	RedCoin* tryLinkToChildRedCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter) {
		if (MR::getChildObjNum(rIter) == 1) {
			RedCoin* pCoin = (RedCoin*)NameObjFactory::initChildObj(rIter, 0);

			if (pCoin) {
				pCoin->requestHide();
        		((ExtActorActionKeeper*)pSourceActor->mActionKeeper)->mRedCoin = pCoin;
				pSourceActor->mActionKeeper->mItemGenerator = 0;
       			return pCoin;
			}
		}

		return false;
	}

	bool tryAppearLinkedRedCoin(LiveActor* pSourceActor, const TVec3f& pPosition) {
		RedCoin* pCoin = ((ExtActorActionKeeper*)pSourceActor->mActionKeeper)->mRedCoin;

		if (pCoin) {
			MR::setPosition(pCoin, pPosition);
			pCoin->appearAndMove();

			return true;
		}

		return false;
	}
}