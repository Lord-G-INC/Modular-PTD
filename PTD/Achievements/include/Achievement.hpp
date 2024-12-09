#include "syati.h"

namespace pt {
    extern void* loadArcAndFile(const char *pArc, const char *pFile, JKRHeap *pHeap);
};

class Achievement {
    public:
    enum AchievementType : s32 {
        EnemyKillCount,
        JumpCount,
        TimeAttack,
        StarNumGet,
        SceneDaredevil,
        Custom
    };
    const char* mName;
    AchievementType mType;
    s32 mRequiredNum;
    s32 mSceneNum;
    const char* mStageName;
    static Achievement** LoadFromDisk(int& pLength);
};