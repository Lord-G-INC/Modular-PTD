#include "Game/Enemy/SamboHead.h"
#include "BlueCoin.h"
//#include "pt/MapObj/RedCoinSystem/RedCoin.h"

class SamboHeadExt : public SamboHead {
public:
    SamboHeadExt(const char* pName);
    BlueCoin* mBlueCoin;
    s32 mBlueCoinArg;
    LiveActor* mRedCoin;
    s32 mRedCoinGroup;
};