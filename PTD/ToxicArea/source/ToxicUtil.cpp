#include "ToxicUtil.h"

u16 playerToxicLevel = 0;

namespace ToxicUtil {
    u16 getToxic() {
        return playerToxicLevel;
    }

    void setToxic(u16 val) {
        playerToxicLevel = val;
    }

    void incToxic() {
        playerToxicLevel += 1;
    }

    void decToxic() {
        if (playerToxicLevel <= 70) {
            playerToxicLevel = 0;
            return;
        }

        playerToxicLevel = playerToxicLevel - 70;
    }
}

kmBranch(0x8028D1D4, ToxicUtil::decToxic);