#pragma once

#define TRACK_DAMAGE_WAIT "\x83\x5F\x83\x81\x81\x5B\x83\x57\x83\x45\x83\x47\x83\x43\x83\x67"
#define TOXIC_RESTORE_EXIT 0.015f // ~= 1 / 60

#include "syati.h"

namespace ToxicController {
    void main();

    inline f32 getToxic();
    inline void setToxic(f32);
    inline void incToxic(f32);
    inline void decToxic(f32);

    void decToxicCoin();
    void decToxicBubble();
};

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