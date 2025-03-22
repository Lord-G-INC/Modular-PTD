#include "ObjectPullArea.h"

ObjectPullArea::ObjectPullArea (const char *pName) : AreaObj(pName) {
	mNoSpinning = false;
	mCheckTypes[0] = ATYPE_COIN;
}

ObjectPullArea::~ObjectPullArea () {

}

void ObjectPullArea::init (const JMapInfoIter &rIter) {
	AreaObj::init(rIter);
	MR::connectToSceneAreaObj(this);
	MR::getJMapInfoArg0NoInit(rIter, &mNoSpinning);
	MR::getJMapInfoArg1NoInit(rIter, &mCheckTypes[0]);
	MR::getJMapInfoArg2NoInit(rIter, &mCheckTypes[1]);
	MR::getJMapInfoArg3NoInit(rIter, &mCheckTypes[2]);
	MR::getJMapInfoArg4NoInit(rIter, &mCheckTypes[3]);
	MR::getJMapInfoArg5NoInit(rIter, &mCheckTypes[4]);
	MR::getJMapInfoArg6NoInit(rIter, &mCheckTypes[5]);
	MR::getJMapInfoArg7NoInit(rIter, &mCheckTypes[6]);
}

void ObjectPullArea::movement () {
	if (isInVolume(*MR::getPlayerPos())) {
		ObjectPullConfig::gIsEnabled = true;
		ObjectPullConfig::gSpinRequired = !mNoSpinning;
		for (s32 i = 0; i < 7; i++) 
			ObjectPullConfig::gCheckTypes[i] = mCheckTypes[i];
	} else 
		ObjectPullConfig::gIsEnabled = false;
}

const char *ObjectPullArea::getManagerName() const {
    return "ObjectPullArea";
}

// ------------------

bool ObjectPullConfig::isPullItem (MarioActor *pMarioActor, u32 sensorType) {
	if (!gIsEnabled)
		return false;
	if (!((MarioActor *)pMarioActor->_1080)->isActionOk("ÉJÉÅéùÇø")) 
		return false;
	for (s32 i = 0; i < 7; i++) {
		if (gCheckTypes[i] == sensorType) 
			return true;
	}
	return false;
}

LiveActor *newCoinPull (HitSensor *pSensor) {
	MarioActor *pMarioActor = MarioAccess::getPlayerActor();
	u32 sensorType = pSensor->mSensorType;
	if (ObjectPullConfig::isPullItem(pMarioActor, sensorType)) {
		TVec3f v0 = pSensor->mPosition - pMarioActor->mTranslation;
		// If the radius is too small, it will cause the duplicate coin bug
		if (PSVECMag(v0) < 120.0f) {
			//((MarioActor *)pMarioActor->_1080)->tryGetItem(pSensor);
			for (s32 i = 0; i < 128; i++) {
				if (pMarioActor->_7C0[i] == pSensor) {
					pMarioActor->_7C0[i] = 0;
					break;
				}
			}
		} else {
			// For coins
			MR::onCalcShadowAll(pSensor->mActor);
			MR::tryEmitEffect(pSensor->mActor, "CoinSpinBlur");

			pMarioActor->tryCoinPullOne(pSensor);
			for (s32 i = 0; i < 128; i++) {
				if (!pMarioActor->_7C0[i]) {
					pMarioActor->_7C0[i] = pSensor;
					break;
				}
			}
		}
	}
	return MR::getSensorHost(pSensor);
}

kmCall(0x803BA270, newCoinPull);

bool checkForSpin () {
	MarioActor *pMarioActor = MarioAccess::getPlayerActor();
	if (!ObjectPullConfig::gSpinRequired) 
		return true;
	return pMarioActor->_704 && pMarioActor->_E3D < 14;
}

kmCall(0x803CBEE4, checkForSpin);
kmWrite32(0x803CBEE8, 0x2C030000); // cmpwi r3, 0
kmWrite32(0x803CBEF0, 0x60000000); // nop
kmWrite32(0x803CBEF4, 0x60000000); // nop
kmWrite32(0x803CBEF8, 0x60000000); // nop