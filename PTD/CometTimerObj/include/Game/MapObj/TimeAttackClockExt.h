#include "Game/MapObj/TimeAttackClock.h"

class CometTimerObj;

class TimeAttackClockExt : public TimeAttackClock {
public:
    TimeAttackClockExt(const char* pName);
    virtual ~TimeAttackClockExt();
    CometTimerObj* mCometTimerObjPtr;
};