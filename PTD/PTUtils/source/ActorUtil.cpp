#include "ActorUtil.h"

/*
* Authors: Aurum
*/
namespace pt {
	void moveAndTurnToPlayer(LiveActor *pActor, const MR::ActorMoveParam &rParam) {
		MR::moveAndTurnToPlayer(pActor, rParam._0, rParam._4, rParam._8, rParam._C);
	}

	void moveAndTurnToDirection(LiveActor *pActor, const TVec3f &rDir, const MR::ActorMoveParam &rParam) {
		MR::moveAndTurnToDirection(pActor, rDir, rParam._0, rParam._4, rParam._8, rParam._C);
	}

	void turnToDirectionUpFront(LiveActor *pActor, TVec3f rUp, TVec3f rFront) {
		TMtx34f mtx;
		MR::makeMtxUpFront((TPos3f*)&mtx, rUp, rFront);
		((TRot3f*)&mtx)->getEulerXYZ(pActor->mRotation);
		pActor->mRotation *= 57.295776f;
	}

	void turnToDirectionGravityFront(LiveActor *pActor, TVec3f rFront) {
		turnToDirectionUpFront(pActor, -pActor->mGravity, rFront);
	}

	void initRailToNearestAndRepositionWithGravity(LiveActor *pActor) {
		RailRider *pRider = pActor->mRailRider;
		pRider->setCoord(pRider->calcNearestPos(pActor->mTranslation));
		getTransRotateFromRailWithGravity(pActor);
	}

	void getTransRotateFromRailWithGravity(LiveActor *pActor) {
		RailRider *pRider = pActor->mRailRider;
		pActor->mTranslation.set(pRider->mCurrentPos);
		turnToDirectionUpFront(pActor, -pActor->mGravity, pRider->mCurrentDir);
	}

	AnimScaleController* createSamboAnimScaleController(AnimScaleParam *pAnimScaleParam) {
		pAnimScaleParam->_10 = 4.0f;
		pAnimScaleParam->_14 = 1.0f;
		pAnimScaleParam->_18 = 0.06f;
		pAnimScaleParam->_1C = 2.0f;
		pAnimScaleParam->_20 = 0.06f;
		pAnimScaleParam->_24 = 0.4f;
		pAnimScaleParam->_28 = 0.8f;

		return new AnimScaleController(pAnimScaleParam);
	}
	
	//Loads an arc and a selected file into memory.
	void* loadArcAndFile(const char *pArc, const char *pFile) {
		OSReport("Loading file %s from %s\n", pFile, pArc);
		JKRArchive* arc = MR::mountArchive(pArc, MR::getStationedHeapGDDR3(), false);
		void* file = arc->getResource(pFile);
	
		if (arc && file) {
			OSReport("(PTD Archive Loader) Archive %s and file %s both exist!\n", pArc, pFile);
			return file;
		}
		else
			OSReport("(PTD Archive Loader) %s %s isn't exist!\n", pArc, pFile);
	
		return 0;
	}

	void initShadowVolumeBox(LiveActor* pActor, const TVec3f& rPos) {
		pActor->initShadowControllerList(1);
		MR::addShadowVolumeBox(pActor, "ボリューム影(ボックス)", rPos, (MtxPtr)pActor->getBaseMtx());
	}

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
}
