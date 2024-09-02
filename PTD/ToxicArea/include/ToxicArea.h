#pragma once
#include "Game/AreaObj.h"

class ToxicArea : public AreaObj {
public:
    ToxicArea(const char *);
    virtual ~ToxicArea();

    virtual void init(const JMapInfoIter &rIter);
    virtual void movement();
    virtual bool isInVolume(const TVec3f &mTranslation) const;
    virtual const char* getManagerName() const;

    u16 mDeathToxicLevel;
    bool mIsMeterActive;
};