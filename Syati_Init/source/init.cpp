#pragma once

#include "syati.h"

typedef void (*Func)();
extern Func __ctor_loc;
extern Func __ctor_end;

void* gDummyDisplayModelTable;

void* loadArcAndFile(const char *pArc, const char *pFile) {
	OSReport("Loading file %s from %s\n", pFile, pArc);
	JKRArchive* arc = MR::mountArchive(pArc, MR::getStationedHeapNapa(), false);
	void* file = arc->getResource(pFile);

	if (arc && file) {
		OSReport("(PTD Archive Loader) Archive %s and file %s both exist!\n", pArc, pFile);
		return file;
	}
	else
		OSReport("(PTD Archive Loader) %s %s isn't exist!\n", pArc, pFile);

	return 0;
}

namespace {
    // ----------------------------------------------------------------------------------------------------------------
    // Calls all static initializers after GameSystem::initAfterStationedResourceLoaded((void)). This is necessary to initialize static variables and 
    // constants in the BSS segment. Custom Nerve instances will be stored there, for example.

    void init() {
        for (Func* f = &__ctor_loc; f < &__ctor_end; f++) {
            (*f)();
        }

        gDummyDisplayModelTable = loadArcAndFile("/SystemData/DummyDisplayModelTable.arc", "/DummyDisplayModelTable.bcsv");
    }

#if defined(TWN) || defined(KOR)
    kmBranch(0x804B6A64, init);
#else
    kmBranch(0x804B69F4, init);
#endif
}