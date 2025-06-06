#include "syati.h"

namespace pt {
    extern void* loadArcAndFile(const char *pArc, const char *pFile, JKRHeap *pHeap);
};

class Achievement {
    public:
    enum AchievementType {
        None = -1,
        EnemyKillCount = 0,
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
    bool mAcquired;
    Achievement() {
        mName = 0,
        mType = None,
        mRequiredNum = -1,
        mSceneNum = -1,
        mStageName = 0,
        mAcquired = false;
    }
};

class Achievements {
    int mLength;
    Achievement** mAchievements;
    Achievements() {
        mLength = 0;
        mAchievements = 0;
    }
    ~Achievements() {
        for (int i = 0; i < mLength; i++)
            delete mAchievements[i];
        delete [] mAchievements;
    }
    void LoadFromDisk();
    void WriteToNand();
    // Loads the Acquired info off the NAND. Assumes that the order is the same from when it was last written.
    void LoadFromNand();
};