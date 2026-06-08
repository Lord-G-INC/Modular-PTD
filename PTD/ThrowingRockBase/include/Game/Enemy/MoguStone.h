#pragma once

#include "Game/LiveActor/ModelObj.h"

class MoguStone : public ModelObj {
public:
	MoguStone(const char *pName, const char *pModelName);

	virtual void init(const JMapInfoIter &rIter);
	virtual void appear();
	virtual void kill();
	virtual void calcAndSetBaseMtx();
	virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
	virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);
	virtual void doBehavior();
	virtual void startBreakSound();
	virtual void startThrowLevelSound();

	void exeThrow();
	void exeFall();
	void emit(bool calcGravity, const TVec3f &rTranslation, const TVec3f &rFrontVec, f32 speed);
	bool isTaken();

	TQuat4f mRotateQuat;
	TQuat4f mUpQuat;
	TVec3f mFrontVec;
	f32 mSpeed;
	bool mCalcGravity;
};

namespace NrvMoguStone {
	NERVE(NrvTaken);
	NERVE(NrvThrow);
	NERVE(NrvFall);
};


class ParabolicPath;

class ThrowingIce : public MoguStone {
public:
	ThrowingIce(const char* pName, const char* pModel, s32 attackMsg);

	virtual void init(const JMapInfoIter&);
	virtual void attackSensor(HitSensor*, HitSensor*);
	virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
	virtual void doBehavior();
	virtual void startBreakSound();
	virtual void startThrowLevelSound();

	void emitArc(const TVec3f&, const TVec3f&, f32, const TVec3f&);

	ParabolicPath* mPath;
	s32 mAttackMsg;
};