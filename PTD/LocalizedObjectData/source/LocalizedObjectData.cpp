#include "syati.h"

bool checkForLocalizedObjectData (char *pDest, u32 length, const char *pFormatter, const char *pArcName) {
    snprintf(pDest, length, "/LocalizeData/%s/ObjectData/%s", MR::getCurrentLanguagePrefix(), pArcName);
    if (!MR::isFileExist(pDest, false)) {
        snprintf(pDest, length, pFormatter, pArcName);
        return MR::isFileExist(pDest, false);
    }
    return true;
}

kmCall(0x80025704, checkForLocalizedObjectData);
kmWrite32(0x80025708, 0x60000000);
kmWrite32(0x8002570C, 0x60000000);
kmWrite32(0x80025710, 0x60000000);