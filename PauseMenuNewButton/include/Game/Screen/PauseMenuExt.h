#include "Game/Screen/PauseMenu.h"

class PauseMenuExt : public PauseMenu {
public: 
    PauseMenuExt();
    ButtonPaneController* mButtonNew;
    TVec2f mButtonNewFollowPos;
    s32 mDisplayMode;
    bool mIsUsedNewButton;
};