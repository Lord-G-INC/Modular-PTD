#pragma once

#include "syati.h"
#include "Game/System/WPadHolder.h"
#include "Game/Player/SupportTico.h"

class NoSupportTicoArea : public AreaObj {
public:
    NoSupportTicoArea(const char *pName);
    virtual void init(const JMapInfoIter &);
    virtual void movement();
};