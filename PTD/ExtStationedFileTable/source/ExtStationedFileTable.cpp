#include "ModuleData_ExtStationedFileTable.h"

#if defined TWN || defined KOR
    #define REGIONOFF 0x90
#else
    #define REGIONOFF 0
#endif

const char* createAndAddNewStationed() {
    StationedFileInfoEntry* pEntry;
    asm("mr %0, r30" : "=r" (pEntry));
    
    if (pEntry->pPath == 0 && pEntry->l2 != 9) {
        pEntry = &cNewStationedFileEntries[1];
    }

    return pEntry->pPath;
    asm("mr r30, %0" : "=r" (pEntry));
}

kmWrite32(0x804CDF40 + REGIONOFF, 0x2C030000);
kmCall(0x804CDF44 + REGIONOFF, createAndAddNewStationed);


const char* loadNewResources() {
    StationedFileInfoEntry* pEntry;
    asm("mr %0, r31" : "=r" (pEntry));
    
    if (pEntry->pPath == 0 && pEntry->l2 != 9) {
        pEntry = &cNewStationedFileEntries[1];
    }

    return pEntry->pPath;
    asm("mr r31, %0" : "=r" (pEntry));
}

kmWrite32(0x804CDE68 + REGIONOFF, 0x2C030000);
kmCall(0x804CDE6C + REGIONOFF, loadNewResources);

kmWrite32(0x804CDD14 + REGIONOFF, 0x2C030000);
kmCall(0x804CDD18 + REGIONOFF, loadNewResources);
