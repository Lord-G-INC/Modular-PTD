#include "NumberedSwitchArea.h"
#include "Game/Util.h"
#include "Game/AreaObj/AreaObj.h"

/*
 * Author: Gabbo
 * Object: NumberedSwitchArea
 * Description: An area that outputs an SW_A signal when an external Switch is activated a specified number of times while the player is inside it.
 *
 * - SW_A (required): Activates when the specified number of activations of the connected Switch is reached.
 * - SW_B (required): Connects this area to the external Switch.
 * 
 *  Obj_arg0: The amount needed to activate SW_A.
*/

namespace pt {
    NumberedSwitchArea::NumberedSwitchArea(const char *pName) : AreaObj(pName) {
        mTime = 0;
        mIsSwActive = false;
    }

    void NumberedSwitchArea::init(const JMapInfoIter &rIter) {
        AreaObj::init(rIter);
        MR::connectToSceneAreaObj(this);
    }

    void NumberedSwitchArea::movement() {
        if (isInVolume(*MR::getPlayerPos())) {
            if (isOnSwitchB() && !mIsSwActive) {
                mTime++;
                mIsSwActive = true;
                OSReport("Le time #%d\n", mTime);
                if (mTime == mSwNum) {
                    onSwitchA();
                    mTime = 0;
                }
            }
            if (!isOnSwitchB()) {
                mIsSwActive = false;
            }
        }
    }

    const char* NumberedSwitchArea::getManagerName() const {
        return "NumberedSwitchArea";
    }    
};