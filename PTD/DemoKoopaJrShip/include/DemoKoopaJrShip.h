#pragma once

#include "syati.h"
#include "LiveActor.h"

class KoopaJr;

class DemoKoopaJrShip : public LiveActor {
public:
    /// @brief Creates a new `DemoKoopaJrShip`.
    /// @param pName A pointer to the null-terminated name of the object.
    DemoKoopaJrShip(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void exeAppear();
    void exeTalk();
    void exeFlyAway();
    void initAnimID(const JMapInfoIter& rIter);
    void createKoopaJrObj(const JMapInfoIter& rIter);
    bool tryDemoEnd();
    void startEntryAnim();
    void startLeaveAnim();

    KoopaJr* mKoopaJrObj;
    s32 mAnimCameraIndex;
};

namespace NrvDemoKoopaJrShip {
    NERVE_DECL_EXE(DemoKoopaJrShipNrvAppear, DemoKoopaJrShip, Appear);
    NERVE_DECL_EXE(DemoKoopaJrShipNrvTalk, DemoKoopaJrShip, Talk);
    NERVE_DECL_EXE(DemoKoopaJrShipNrvFlyAway, DemoKoopaJrShip, FlyAway);
};