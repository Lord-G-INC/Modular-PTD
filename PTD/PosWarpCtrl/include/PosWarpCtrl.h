#pragma once

#include "syati.h"

class PosWarpCtrl : public NameObj {
public:
    PosWarpCtrl(const char *pName);
    virtual void init(const JMapInfoIter &);
    virtual void movement();

    TVec3f mStoredPos;
    bool mWasPosStored;
};