#include "Game/Enemy/MoguStone.h"

#include "Game/NameObj/NameObjCategories.h"
#include "Game/Util.h"

/*
* Author: Aurum
* Objects: n/a
* Parameters: n/a
* 
* This is the throwing wrench used by Monty Moles to attack the player. This is also used as a base class for the ice chunks thrown by Baron Brrr.
*/

MoguStone::MoguStone(const char *pName, const char *pModelName) : ModelObj(pName, pModelName, NULL, MR::CategoryList_Auto, MR::CategoryList_Auto, MR::CategoryList_Auto, false) {
	mRotateQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	mUpQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	mFrontVec.set(0.0f, 0.0f, 1.0f);
	mSpeed = 0.0f;
	mCalcGravity = true;
}

void MoguStone::init(const JMapInfoIter &rIter) {
	ModelObj::init(rIter);

	MR::invalidateClipping(this);
	MR::onCalcGravity(this);
	MR::initShadowVolumeSphere(this, 50.0f * mScale.y);

	initNerve(&NrvMoguStone::NrvTaken::sInstance, 0);
	makeActorDead();
}

void MoguStone::appear() {
	ModelObj::appear();
	setNerve(&NrvMoguStone::NrvTaken::sInstance);
}

void MoguStone::kill() {
	ModelObj::kill();
	MR::tryEmitEffect(this, "Break");
}

void MoguStone::calcAndSetBaseMtx() {
	TMtx34f mtxTR;
	((TPos3f*)&mtxTR)->setTrans(mTranslation);
	((TRot3f*)&mtxTR)->setQuat(mRotateQuat);
	MR::setBaseTRMtx(this, *(TPos3f*)&mtxTR);
}

void MoguStone::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
	if (pSender == getSensor("Body") && MR::isSensorPlayer(pReceiver)) {
		if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
			MR::startActionSound(this, "BmIcemerakingStoneBreak", -1, -1, -1);
			kill();
		}
	}
}

bool MoguStone::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (!isNerve(&NrvMoguStone::NrvThrow::sInstance)) {
		return false;
	}

	if (MR::isMsgLockOnStarPieceShoot(msg)) {
		return true;
	}

	if (!MR::isMsgPlayerHitAll(msg) && !MR::isMsgStarPieceAttack(msg)) {
		return false;
	}

	MR::startActionSound(this, "BmIcemerakingStoneBreak", -1, -1, -1);
	kill();
	return true;
}

void MoguStone::doBehavior() {
	if (MR::isFirstStep(this)) {
		mUpQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (mCalcGravity) {
		MR::vecKillElement(mFrontVec, mGravity, &mFrontVec);
		MR::normalizeOrZero(&mFrontVec);
	}

	mVelocity.scale(mSpeed, mFrontVec);
	MR::makeQuatUpFront(&mRotateQuat, -mGravity, mFrontVec);

	if (MR::isGreaterStep(this, 100)) {
		startBreakSound();
		kill();
	}
}

void MoguStone::startBreakSound() {
	MR::startActionSound(this, "EmMoguSpanaBreak", -1, -1, -1);
}

void MoguStone::startThrowLevelSound() {
	MR::startActionSound(this, "EmLvMoguSpanaFly", -1, -1, -1);
}

void MoguStone::exeThrow() {
	startThrowLevelSound();
	doBehavior();

	PSQUATMultiply(mUpQuat, mRotateQuat, mRotateQuat);
	mRotateQuat.normalize();

	if (MR::isBinded(this)) {
		startBreakSound();
		kill();
	}
	else if (MR::isGreaterStep(this, 100)) {
		setNerve(&NrvMoguStone::NrvFall::sInstance);
	}
}

void MoguStone::exeFall() {
	PSQUATMultiply(mUpQuat, mRotateQuat, mRotateQuat);
	mRotateQuat.normalize();

	MR::applyVelocityDampAndGravity(this, 2.0f, 0.8f, 0.98f, 0.98f, 1.0f);

	if (MR::isBinded(this) || MR::isGreaterStep(this, 60)) {
		startBreakSound();
		kill();
	}
}

void MoguStone::emit(bool calcGravity, const TVec3f &rTranslation, const TVec3f &rFrontVec, f32 speed) {
	mSpeed = speed;
	mTranslation.set(rTranslation);
	mVelocity.zero();
	MR::calcGravity(this);

	if (calcGravity) {
		MR::onCalcGravity(this);
		MR::vecKillElement(rFrontVec, mGravity, &mFrontVec);
		mCalcGravity = true;
	}
	else {
		MR::offCalcGravity(this);
		mFrontVec.set(rFrontVec);
		mCalcGravity = false;
	}

	MR::normalizeOrZero(&mFrontVec);

	setNerve(&NrvMoguStone::NrvThrow::sInstance);
}

bool MoguStone::isTaken() {
	return isNerve(&NrvMoguStone::NrvTaken::sInstance);
}

namespace NrvMoguStone {
	void NrvTaken::execute(Spine *pSpine) const {}

	void NrvThrow::execute(Spine *pSpine) const {
		MoguStone *pActor = (MoguStone*)pSpine->mExecutor;
		pActor->exeThrow();
	}

	void NrvFall::execute(Spine *pSpine) const {
		MoguStone *pActor = (MoguStone*)pSpine->mExecutor;
		pActor->exeFall();
	}

	NrvTaken(NrvTaken::sInstance);
	NrvThrow(NrvThrow::sInstance);
	NrvFall(NrvFall::sInstance);
}


namespace {
	const static f32 hAddHeight = 200.0f;
};  // namespace

ThrowingIce::ThrowingIce(const char* pName) : MoguStone(pName, "IceManIce"), mPath(nullptr) {
	mPath = new ParabolicPath();
	mScale.x = 1.3f;
	mScale.y = 1.3f;
	mScale.z = 1.3f;
}

void ThrowingIce::init(const JMapInfoIter& rIter) {
	initBinder(60.0f * mScale.y, 0.0f, 0);
	ModelObj::init(rIter);
	initHitSensor(1);
	MR::addHitSensorEnemy(this, "body", 32, 80.0f * mScale.y, TVec3f(0.0f, 0.0f, 0.0f));
	initNerve(&NrvMoguStone::NrvTaken::sInstance, 0);
	MR::invalidateClipping(this);
	MR::onCalcGravity(this);
	MR::initShadowVolumeSphere(this, 90.0f * mScale.y);
	makeActorDead();
}

void ThrowingIce::emitIce(const TVec3f& arg1, const TVec3f& arg2, f32 arg3, const TVec3f& arg4) {
	mPath->initFromUpVectorAddHeight(arg1, arg2, -arg4, hAddHeight);
	mSpeed = arg3;

	TVec3f v2(arg2);
	v2 -= arg1;
	MR::vecKillElement(v2, mGravity, &mFrontVec);
	MR::normalizeOrZero(&mFrontVec);
	setNerve(&NrvMoguStone::NrvThrow::sInstance);
	MR::emitEffect(this, "Smoke");
}

void ThrowingIce::doBehavior() {
	if (MR::isFirstStep(this)) {
		TVec3f v1;
		PSVECCrossProduct(mFrontVec.toVecPtr(), mGravity.toVecPtr(), v1.toVecPtr());
		mUpQuat.setRotate(v1, 0.25f);
	}

	f32 rate = MR::calcNerveRate(this, 101);
	TVec3f v2;
	mPath->calcPosition(&v2, rate);
	TVec3f v3(v2);
	v3 -= mTranslation;
	mVelocity.set(v3);
}

void ThrowingIce::attackSensor(HitSensor* pSensor1, HitSensor* pSensor2) {
	if (pSensor1 == getSensor("body") && MR::isSensorPlayer(pSensor2) &&
		(MR::isPlayerElementModeIce() ? MR::sendMsgEnemyAttackStrong(pSensor2, pSensor1) : MR::sendMsgEnemyAttackFreeze(pSensor2, pSensor1))) {
		MR::emitEffect(this, "Break");
		MR::startSound(this, "SE_BM_ICEMERAKING_STONE_BREAK", -1, -1);
		MR::deleteEffect(this, "Smoke");
		kill();
	}
}

bool ThrowingIce::receiveMsgPlayerAttack(u32 msg, HitSensor* pSensor1, HitSensor* pSensor2) {
	if (!isNerve(&NrvMoguStone::NrvThrow::sInstance)) {
		return false;
	}

	if (MR::isMsgPlayerSpinAttack(msg)) {
		return false;
	}

	MoguStone::receiveMsgPlayerAttack(msg, pSensor1, pSensor2);
	return false;
}

void ThrowingIce::startBreakSound() {
	MR::startSound(this, "SE_BM_ICEMERAKING_STONE_BREAK", -1, -1);
}

void ThrowingIce::startThrowLevelSound() {
	// make no noise on throw
}