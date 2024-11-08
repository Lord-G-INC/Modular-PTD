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

struct StageInstrumentEntry {
    const char* pName;
    const char* pStage;
    u8 _8;
    u8 _9;
    u8 _A;
    u8 _B;
};

StageInstrumentEntry gStageInstrumentTable[] = {
    {"dummy", "dummy",               0x00, 0xFF, 0x00, 0x00},
    {"Game", "BigWaterFallGalaxy",   0x01, 0xFF, 0x00, 0x00},
    {"Game", "TamakoroSliderGalaxy", 0x02, 0xFF, 0x00, 0x00},
    {"Game", "BigTree2Galaxy",       0x03, 0xFF, 0x00, 0x00},
    {"Game", "TropicalResortGalaxy", 0x05, 0xFF, 0x00, 0x00},
    {"Game", "WorldMap01Galaxy",     0x00, 0xFF, 0x00, 0x00},
    {"Game", "WorldMap02Galaxy",     0x00, 0xFF, 0x00, 0x00},
    {"Game", "WorldMap03Galaxy",     0x00, 0xFF, 0x00, 0x00},
    {"Game", "WorldMap04Galaxy",     0x00, 0xFF, 0x00, 0x00},
    {"Game", "WorldMap05Galaxy",     0x00, 0xFF, 0x00, 0x00},
    {"Game", "WorldMap06Galaxy",     0x00, 0xFF, 0x00, 0x00},
    {"Game", "WorldMap07Galaxy",     0x00, 0xFF, 0x00, 0x00},
    {"Game", "WorldMap08Galaxy",     0x00, 0xFF, 0x00, 0x00},
};

const size_t cStageInstrumentTableEntries = sizeof(gStageInstrumentTable)/sizeof(gStageInstrumentTable[0]);

class RedCoin;

namespace RedCoinUtil {
    extern RedCoin* tryLinkToChildRedCoin(LiveActor* pSourceActor, const JMapInfoIter& rIter);
    extern bool tryAppearLinkedRedCoin(LiveActor* pSourceActor, const TVec3f& pPosition);
}

namespace BlueCoinUtil {
    extern bool tryCreateBlueCoinForSpawningActorActionKeeper(LiveActor* pSourceActor, s32 id);
    extern void appearBlueCoinActionKeeper(LiveActor* pSourceActor);
};

//TamakoroBgmParamEntry gTamakoroBgmParamTable[] = {
//    {"BGM_TAMAKORO",   0x6B, 0x1E, 0x1E, 134.0f, 73.700005f, 163.48001f, 198.052f, 0, 0, 2.996985f, 0xE, 0xFFFFFFFF},
//    {"BGM_TAMAKORO_2", 0xCA, 0x1E, 0xF, 84.0f, 80.0f, 92.0f, 112.0f, 0, 0, 4.99685f, 0xE, 0xF}
//};

TamakoroBgmParamEntry gTamakoroBgmParamTable[] = {
    {"BGM_TAMAKORO",   0x6B, 0x1E, 0x1E, 134.0f, 73.700005f, 163.48001f, 198.052f, 0, 0, 2.996985f, 0xE, 0xFFFFFFFF},
    {"BGM_TAMAKORO_2", 0xCA, 0x1E, 0xF, 84.0f, 80.0f, 92.0f, 112.0f, 0, 0, 4.99685f, 0xE, 0xF},
    {"BGM_SMG_TAMAKORO",   0x6B, 0x1E, 0x1E, 134.0f, 73.700005f, 163.48001f, 198.052f, 0, 0, 2.996985f, 0xE, 0xFFFFFFFF},
    {"BGM_SMG_TAMAKORO_2", 0xCA, 0x1E, 0xF, 84.0f, 80.0f, 92.0f, 112.0f, 0, 0, 4.99685f, 0xE, 0xF}
};