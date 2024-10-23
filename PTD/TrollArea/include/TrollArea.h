#pragma once

#include "syati.h"

class TrollArea : public AreaObj {
public:
    TrollArea(const char *);
    virtual void init(const JMapInfoIter &);
    virtual void movement();
    s32 mTrollMode;
};

void crashGame();
JAISoundID getRandomSoundId();