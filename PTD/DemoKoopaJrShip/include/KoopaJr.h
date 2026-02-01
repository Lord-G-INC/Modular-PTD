#pragma once

#include "syati.h"
#include "NPCActor.h"

class KoopaJr : public NPCActor {
public:
    KoopaJr(const char*);

    virtual ~KoopaJr();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void setStateShipBattleAppear();
    void setStateShipBattlePowerUp();
    void setStateShipBattleEscape();
    void setStateShipBattleShipDamage();
    void endShipBattleTalk();
    void startShipBattleTalk();
    void exeWait();
    void exeReaction();
    void exeReactionEnd();
    void exeShipBattleAppear();
    void exeShipBattlePowerUp();
    void exeShipBattleEscape();
    void exeShipBattleShipDamage();
    void exeShipBattleDemoTalkStart();
    void exeShipBattleDemoTalkWait();

    u8 _15C;
    u8 _15D;
};

namespace NrvKoopaJr {
    NERVE_DECL_EXE(HostTypeWait, KoopaJr, Wait);
    NERVE_DECL_EXE(HostTypeReaction, KoopaJr, Reaction);
    NERVE_DECL_EXE(HostTypeReactionEnd, KoopaJr, ReactionEnd);
    NERVE_DECL_EXE(HostTypeShipBattleAppear, KoopaJr, ShipBattleAppear);
    NERVE_DECL_EXE(HostTypeShipBattlePowerUp, KoopaJr, ShipBattlePowerUp);
    NERVE_DECL_EXE(HostTypeShipBattleEscape, KoopaJr, ShipBattleEscape);
    NERVE_DECL_EXE(HostTypeShipBattleShipDamage, KoopaJr, ShipBattleShipDamage);
    NERVE_DECL_EXE(HostTypeShipBattleDemoTalkStart, KoopaJr, ShipBattleDemoTalkStart);
    NERVE_DECL_EXE(HostTypeShipBattleDemoTalkWait, KoopaJr, ShipBattleDemoTalkWait);
};  // namespace NrvKoopaJr
