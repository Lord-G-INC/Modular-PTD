#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Util/PlayerUtil.h"
#include "syati.h"
#include "Game/AreaObj/AreaObj.h"

namespace pt {
    class NumberedSwitchArea : public AreaObj {
    public:
        NumberedSwitchArea(const char *pName);
        virtual void init(const JMapInfoIter &rIter);
        virtual void movement();
        virtual const char* getManagerName() const;
    
        s32 mTime;
        bool mIsSwActive;
    
        #define mSwNum mObjArg0
    };  
}