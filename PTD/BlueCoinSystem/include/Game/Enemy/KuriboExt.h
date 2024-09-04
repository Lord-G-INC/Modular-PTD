#include "Game/Enemy/Kuribo.h"
#include "BlueCoin.h"

class KuriboExt : public Kuribo {
public:
    KuriboExt(const char* pName);
    BlueCoin* mBlueCoin;
    s32 mBlueCoinArg;
};