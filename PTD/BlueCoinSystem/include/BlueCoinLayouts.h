#pragma once

#include "syati.h"
#include "Game/Screen/CountUpPaneRumbler.h"
#include "Game/Screen/SysInfoWindow.h"
#include "Game/Screen/PauseMenu.h"

const wchar_t counterPictureFonts[] = {
0x000E, 0x0003, 0x0055, 0x0002, 0x005A, // Blue Coin Icon
0x000E, 0x0003, 0x0010, 0x0002, 0x0010, // X Icon
0x000E, 0x0006, 0x0001, 0x0008, 0x0000, // Counter
0x0000, 0x0000, 0x0000
};

const wchar_t counterPictureFonts1[] = {
0x000E, 0x0003, 0x0055, 0x0002, 0x005A, // Blue Coin Icon
0x000E, 0x0003, 0x0010, 0x0002, 0x0010, // X Icon
0x000E, 0x0006, 0x0001, 0x0008, 0x0001, // Counter
0x0000, 0x0000, 0x0000
};

const wchar_t counterPictureFonts2[] = {
0x000E, 0x0003, 0x0055, 0x0002, 0x005A, // Blue Coin Icon
0x000E, 0x0003, 0x0010, 0x0002, 0x0010 // X Icon
};

class BlueCoinCounter : public LayoutActor {
public:
        BlueCoinCounter(const char* pName);
        
        virtual ~BlueCoinCounter();
        virtual void init(const JMapInfoIter& rIter);
        virtual void appear();
        virtual void control();
        void forceAppear();
        void updateCounter();
        void disappear();
        void setCounter();
        void exeHide();
        void exeAppear();
        void exeWait();
        void exeDisappear();
        void exeShowTextBox();

        CounterLayoutAppearer* mAppearer;
        CountUpPaneRumbler* mPaneRumbler;
        SysInfoWindow* mSysInfoWindow;
        s32 mBlueCoinCount;
        s32 mBlueCoinDisplayNum;
        s32 _3C;
};

namespace NrvBlueCoinCounter {
	NERVE(NrvHide);
        NERVE(NrvAppear);
        NERVE(NrvWait);
        NERVE(NrvDisappear);
        NERVE(NrvShowTextBox);
};

void PauseMenuIDListControls(PauseMenu* pPauseMenu);
