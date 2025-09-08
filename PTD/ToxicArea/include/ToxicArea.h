#pragma once

#include "syati.h"

class ToxicArea : public AreaObj {
public:
    ToxicArea(const char *);

    virtual void init(const JMapInfoIter &rIter);
    virtual bool isInVolume(const TVec3f &mTranslation) const;
    virtual const char* getManagerName() const;

    f32 mIncAmount;
    f32 mCoinRestore;
    f32 mBubbleRestore;
};