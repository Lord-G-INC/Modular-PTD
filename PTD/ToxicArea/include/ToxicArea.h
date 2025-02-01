#pragma once

// ? #define TOXIC_ANIM "\xE3\x83\x80\xE3\x83\xA1\xE3\x83\xBC\xE3\x82\xB8\xE3\x82\xA6\xE3\x82\xA8\xE3\x82\xA4\xE3\x83\x88" -- Used in ToxicArea.cpp, L41
#define TOXIC_RESTORE_EXIT 0.015f // ~= 1 / 60

#include "syati.h"

namespace ToxicController {
    void main();

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