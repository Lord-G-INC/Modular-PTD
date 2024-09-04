#include "Game/Enemy/Teresa.h"
#include "BlueCoin.h"

class TeresaExt : public Teresa {
public:
    TeresaExt(const char* pName);
    BlueCoin* mBlueCoin;
    s32 mBlueCoinArg;
};