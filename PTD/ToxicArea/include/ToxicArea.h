#pragma once
#include "Game/AreaObj.h"

class ToxicArea : public AreaObj {
public:
    ToxicArea(const char *);

    virtual void init(const JMapInfoIter &rIter);
    virtual void movement();
    virtual bool isInVolume(const TVec3f &mTranslation) const;
    virtual const char* getManagerName() const;

    f32 mDeathToxicLevel;
    bool mIsMeterActive;
};