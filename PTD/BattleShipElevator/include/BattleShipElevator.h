#pragma once

#include "syati.h"

class BattleShipElevator : public MapObjActor {
public:
    BattleShipElevator(const char*);

    virtual ~BattleShipElevator();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeMove();
    void exeEnd();
};


namespace NrvBattleShipElevator {
    NERVE_DECL_EXE(BattleShipElevatorNrvWait, BattleShipElevator, Wait);
    NERVE_DECL_EXE(BattleShipElevatorNrvMove, BattleShipElevator, Move);
    NERVE_DECL_EXE(BattleShipElevatorNrvEnd, BattleShipElevator, End);
};

extern "C" {
	void __kAutoMap_8013B110(LiveActor* pActor, const char*, const char*, const Nerve*, const Nerve*, s32, DemoStartInfo::DemoType, DemoStartInfo::CinemaFrameType, DemoStartInfo::StarPointerType, DemoStartInfo::DeleteEffectType);
}