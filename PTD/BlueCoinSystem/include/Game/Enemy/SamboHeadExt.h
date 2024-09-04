#include "Game/Enemy/SamboHead.h"
#include "BlueCoin.h"

class SamboHeadExt : public SamboHead {
public:
    SamboHeadExt(const char* pName);
    BlueCoin* mBlueCoin;
    s32 mBlueCoinArg;
};