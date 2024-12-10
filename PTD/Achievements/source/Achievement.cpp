#include "Achievement.hpp"


void Achievements::LoadFromDisk() {
    void* data = pt::loadArcAndFile("/SystemData/Achievements.arc", "/Achievements.bcsv", 0);
    JMapInfo table = JMapInfo();
    table.attach(data);
    mLength = MR::getCsvDataElementNum(&table);
    mAchievements = new Achievement*[mLength];
    for (int i = 0; i < mLength; i++) {
        Achievement* ach = new Achievement();
        MR::getCsvDataStr(&ach->mName, &table, "Name", i);
        MR::getCsvDataS32((s32*)&ach->mType, &table, "Type", i);
        MR::getCsvDataS32(&ach->mRequiredNum, &table, "RequiredNum", i);
        MR::getCsvDataS32(&ach->mSceneNum, &table, "SceneNum", i);
        MR::getCsvDataStr(&ach->mStageName, &table, "StageName", i);
        mAchievements[i] = ach;
    }
}

void Achievements::WriteToNand() {
    if (mLength == 0 && mAchievements == 0)
        LoadFromDisk();
    s32 code = NANDCreate("Achievements.bin", NAND_PERM_READ_WRITE, 0);
    if (code == 0 || code == -6) {
        NANDFileInfo info;
        code = NANDOpen("Achievements.bin", &info, NAND_MODE_RW);
        if (code == 0) {
            s32 size = 4 + (5 * mLength);
            u8* buffer = new (0x20) u8[size];
            memcpy(buffer, &mLength, 4);
            int index = 4;
            for (int i = 0; i < mLength; i++) {
                const Achievement* ach = mAchievements[i];
                memcpy(buffer + index, &i, 4);
                buffer[index + 4] = ach->mAcquired;
                index += 5;
            }
            code = NANDWrite(&info, buffer, size);
            delete [] buffer;
        }
        NANDClose(&info);
    }
}