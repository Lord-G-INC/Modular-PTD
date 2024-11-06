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
    f32 mBallColor;
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