#pragma once

#include "syati.h"

typedef void (*Func)();
extern Func __ctor_loc;
extern Func __ctor_end;

namespace pt {
    extern void* loadArcAndFile(const char *pArc, const char *pFile);
}

namespace BlueCoinUtil {
    extern void initBlueCoinArray();
}

void* gDummyDisplayModelTable;
void* gBlueCoinIDRangeTable;
void* gBoardDataTable;


namespace {
    // ----------------------------------------------------------------------------------------------------------------
    // Calls all static initializers after GameSystem::initAfterStationedResourceLoaded((void)). This is necessary to initialize static variables and 
    // constants in the BSS segment. Custom Nerve instances will be stored there, for example.

    void init() {
        for (Func* f = &__ctor_loc; f < &__ctor_end; f++) {
            (*f)();
        }
        BlueCoinUtil::initBlueCoinArray();

        gDummyDisplayModelTable = pt::loadArcAndFile("/SystemData/DummyDisplayModelTable.arc", "/DummyDisplayModelTable.bcsv");
        gBlueCoinIDRangeTable = pt::loadArcAndFile("SystemData/BlueCoinIDRangeTable.arc", "/BlueCoinIDRangeTable.bcsv");
        gBoardDataTable = pt::loadArcAndFile("SystemData/BlueCoinBoardDataTable.arc", "/BlueCoinBoardDataTable.bcsv");
    }

#if defined(TWN) || defined(KOR)
    kmBranch(0x804B6A64, init);
#else
    kmBranch(0x804B69F4, init);
#endif

void loadPTPictureFont() {
	pt::loadArcAndFile("/SystemData/PictureFont.arc", "/PictureFont.brfnt");
}

kmCall(0x804B8048, loadPTPictureFont);
}