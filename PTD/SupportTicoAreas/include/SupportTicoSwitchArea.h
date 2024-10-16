#pragma once

#include "syati.h"
#include "Game/Player/SupportTico.h"

class SupportTicoSwitchArea : public AreaObj {
public:
    SupportTicoSwitchArea(const char *pName);
    virtual void init(const JMapInfoIter &);
    virtual void movement();
};