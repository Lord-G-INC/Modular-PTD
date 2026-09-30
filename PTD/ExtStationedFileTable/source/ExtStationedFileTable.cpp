#include "ModuleData_ExtStationedFileTable.h"
#include "syati.h"

#if defined TWN || defined KOR
    #define REGIONOFF 0x90
#else
    #define REGIONOFF 0
#endif

const char* createAndAddNewStationed() {
    MR::StationedFileInfo* pEntry;
    asm("mr %0, r30" : "=r" (pEntry));
    
    if (pEntry->mFilePath == 0 && pEntry->mLoadType != 9) {
        pEntry = &cNewStationedFileEntries[0];

        if (pEntry->mLoadType == 9)
            return 0;
    }

    return pEntry->mFilePath;
}

kmWrite32(0x804CDF40 + REGIONOFF, PPC_CMPWI(3, 0));
kmCall(0x804CDF44 + REGIONOFF, createAndAddNewStationed);


const char* loadNewResources() {
    MR::StationedFileInfo* pEntry;
    asm("mr %0, r31" : "=r" (pEntry));
    
    if (pEntry->mFilePath == 0 && pEntry->mLoadType != 9) {
        pEntry = &cNewStationedFileEntries[0];

        if (pEntry->mLoadType == 9)
            return 0;
    }

    return pEntry->mFilePath;
}

kmWrite32(0x804CDE68 + REGIONOFF, PPC_CMPWI(3, 0));
kmCall(0x804CDE6C + REGIONOFF, loadNewResources);

kmWrite32(0x804CDD14 + REGIONOFF, PPC_CMPWI(3, 0));
kmCall(0x804CDD18 + REGIONOFF, loadNewResources);
