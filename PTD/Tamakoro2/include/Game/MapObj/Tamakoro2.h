#include "syati.h"
#include "Game/Ride/Tamakoro.h"

class TamakoroExt : public Tamakoro {
    public:
    TamakoroExt(const char* pName);

    virtual ~TamakoroExt();

    DummyDisplayModel* mDummyDisplayModel;
    s32 mItemType;
    s32 mMusicNum;
    s32 mModelId;
    s32 mBvaFrame;
    f32 mBrkFrame;
    bool mIsBind;
};

class RedCoin;

namespace RedCoinUtil {
    extern RedCoin* tryLinkToChildRedCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter);
    extern bool tryAppearLinkedRedCoin(LiveActor* pSourceActor, const TVec3f& pPosition);
}

namespace BlueCoinUtil {
    extern bool tryCreateBlueCoinForSpawningActorActionKeeper(LiveActor* pSourceActor, s32 id);
    extern void appearBlueCoinActionKeeper(LiveActor* pSourceActor, TVec3f& rPos);
};

//TamakoroBgmParamEntry gTamakoroBgmParamTable[] = {
//    {"BGM_TAMAKORO",   0x6B, 0x1E, 0x1E, 134.0f, 73.700005f, 163.48001f, 198.052f, 0, 0, 2.996985f, 0xE, 0xFFFFFFFF},
//    {"BGM_TAMAKORO_2", 0xCA, 0x1E, 0xF, 84.0f, 80.0f, 92.0f, 112.0f, 0, 0, 4.99685f, 0xE, 0xF}
//};

TamakoroBgmParamEntry gTamakoroBgmParamTable[] = {
    {"BGM_TAMAKORO",   0x6B, 0x1E, 0x1E, 134.0f, 73.700005f, 163.48001f, 198.052f, 0, 0, 2.996985f, 0xE, 0xFFFFFFFF},
    {"BGM_TAMAKORO_2", 0xCA, 0x1E, 0xF, 84.0f, 80.0f, 92.0f, 112.0f, 0, 0, 4.99685f, 0xE, 0xF},
    {"BGM_SMG_TAMAKORO",   0x6B, 0x1E, 0x1E, 134.0f, 73.700005f, 163.48001f, 198.052f, 0, 0, 2.996985f, 0xE, 0xFFFFFFFF},
    {"BGM_SMG_TAMAKORO_2", 0x6B, 0x1E, 0x1E, 134.0f, 73.700005f, 163.48001f, 198.052f, 0, 0, 2.996985f, 0xE, 0xFFFFFFFF},
};