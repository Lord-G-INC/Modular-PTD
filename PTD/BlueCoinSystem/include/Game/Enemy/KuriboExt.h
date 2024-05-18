#include "Game/Enemy/Kuribo.h"
#ifdef BLUECOINSYSTEM
#include "BlueCoin.h"
#endif

class KuriboExt : public Kuribo {
public:
    KuriboExt(const char* pName);
    BlueCoin* mBlueCoin;
    s32 mBlueCoinArg;
};