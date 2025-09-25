#pragma once

#include "syati.h";
#include "ClipAreaDropScale.h";

class ClipAreaDropScaleHolder : public DeriveActorGroup<ClipAreaDropScale> {
public:
    ClipAreaDropScaleHolder();
};


namespace MR {
    NameObj* createClipAreaDropScaleHolder();
    ClipAreaDropScale* getDeadClipAreaDropScale();
    bool appearClipAreaDropScale(const TVec3f &, f32);
};

namespace ClipAreaDropScaleHolderUtil {
    NameObj* createClipAreaDropScaleHolder();
};
