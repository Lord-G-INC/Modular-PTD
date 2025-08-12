#include "Game/MapObj/TimeAttackClockExt.h"
#include "CometTimerObj.h"

TimeAttackClockExt::TimeAttackClockExt(const char* pName) : TimeAttackClock(pName) {
    mCometTimerObjPtr = 0;
}

TimeAttackClockExt::~TimeAttackClockExt() {
}

TimeAttackClockExt* createTimeAttackClockExt(const char* pName) {
    return new TimeAttackClockExt(pName);
}

kmCall(0x8033FFC4, createTimeAttackClockExt); // bl createTimeAttackClockExt
kmWrite32(0x8033FFC8, 0x48000014); // b 0x14

void TimeAttackClockInit(TimeAttackClockExt* pActor, const JMapInfoIter& rIter, const char* pStr, bool b) {
    MR::processInitFunction(pActor, rIter, pStr, b);
    MR::getJMapInfoArg0NoInit(rIter, (s32*)&pActor->mAddTime);
}

kmCall(0x8032326C, TimeAttackClockInit);

void TimeAttackClockAddTimer(TimeAttackClockExt* pClock, const char* pStr) {
    MR::startActionSound(pClock, pStr, -1, -1, -1);

    if (pClock->mCometTimerObjPtr)
        pClock->mCometTimerObjPtr->addTime(pClock->mAddTime);
    else
        MR::addCometTimer(pClock->mAddTime);
}

kmCall(0x803234B0, TimeAttackClockAddTimer); // bl TimeAttackClockAddTimer
kmWrite32(0x803234B4, 0x48000014); // b 0x14