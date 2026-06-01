#include "BattleShipElevator.h"

BattleShipElevator::BattleShipElevator(const char* pName) : MapObjActor(pName) {}

BattleShipElevator::~BattleShipElevator() {}

void BattleShipElevator::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr, 1);
    info.setupSound(4, nullptr);
    info.setupRailMover();
    info.setupNerve(&NrvBattleShipElevator::BattleShipElevatorNrvWait::sInstance);
    MapObjActor::initialize(rIter, info);
}

void BattleShipElevator::exeWait() {}

void BattleShipElevator::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_B_SHIP_ELEV_START", -1, -1);
        MapObjActor::startMapPartsFunctions();
    }
    MR::startLevelSound(this, "SE_OJ_LV_B_SHIP_ELEV_MOVE", -1, -1, -1);
    if (!MapObjActorUtil::isRailMoverWorking(this)) {
        MR::startSound(this, "SE_OJ_B_SHIP_ELEV_STOP", -1, -1);
        setNerve(&NrvBattleShipElevator::BattleShipElevatorNrvEnd::sInstance);
    }
}

void BattleShipElevator::exeEnd() {}

void BattleShipElevator::control() {
    if (!isNerve(&NrvBattleShipElevator::BattleShipElevatorNrvWait::sInstance)) {
        MapObjActor::control();
    }
}

bool BattleShipElevator::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvBattleShipElevator::BattleShipElevatorNrvWait::sInstance)) {
        return false;
    }

    if (MR::isMsgFloorTouch(msg)) {
        if (MR::isOnPlayer(getSensor("body"))) {
            __kAutoMap_8013B110(this, "エレベーター上昇", nullptr, &NrvBattleShipElevator::BattleShipElevatorNrvMove::sInstance, nullptr, 2, 
                DemoStartInfo::DEMOTYPE_1, 
                DemoStartInfo::CINEMAFRAMETYPE_0, 
                DemoStartInfo::STARPOINTERTYPE_0, 
                DemoStartInfo::DELETEEFFECTYPE_0);
        }
    }

    return false;
}


namespace NrvBattleShipElevator {
    BattleShipElevatorNrvWait(BattleShipElevatorNrvWait::sInstance);
    BattleShipElevatorNrvMove(BattleShipElevatorNrvMove::sInstance);
    BattleShipElevatorNrvEnd(BattleShipElevatorNrvEnd::sInstance);
};