#include "Game/Screen/PauseMenu.h"

class BlueCoinList;

class PauseMenuExt : public PauseMenu {
public: 
    PauseMenuExt();
    virtual ~PauseMenuExt();
    void exeBlueCoinList();

    // BCS
    BlueCoinList* mBlueCoinList;
    s32 mDisplayMode;
    bool mIsInvalidBack;

};

namespace NrvPauseMenuExt {
    NERVE(NrvPauseMenuExtBlueCoinList);
};