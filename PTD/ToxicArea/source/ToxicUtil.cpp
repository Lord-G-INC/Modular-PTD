#include "ToxicUtil.h"

u16 playerToxicLevel = 0;

namespace ToxicUtil {
    u16 getToxic() {
        return playerToxicLevel;
    }

    void setToxic(u16 val) {
        playerToxicLevel = val;
    }

    void incToxic(u16 val) {
        playerToxicLevel += val;
    }
}