#include "Game/Enemy/SamboHead.h"
#ifdef BLUECOINSYSTEM
#include "BlueCoin.h"
#endif

class SamboHeadExt : public SamboHead {
public:
    SamboHeadExt(const char* pName);
    BlueCoin* mBlueCoin;
    s32 mBlueCoinArg;
};