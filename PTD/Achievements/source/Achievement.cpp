#include "Achievement.hpp"


Achievement** Achievement::LoadFromDisk(int& pLength) {
    void* data = pt::loadArcAndFile("/SystemData/Achievements.arc", "/Achievements.bcsv", 0);
    JMapInfo table = JMapInfo();
    table.attach(data);
    int length = MR::getCsvDataElementNum(&table);
    pLength = length;
    Achievement** ptr = new Achievement*[length];
    for (int i = 0; i < length; i++) {
        Achievement* ach = new Achievement();
        MR::getCsvDataStr(&ach->mName, &table, "Name", i);
        MR::getCsvDataS32((s32*)&ach->mType, &table, "Type", i);
        MR::getCsvDataS32(&ach->mRequiredNum, &table, "RequiredNum", i);
        MR::getCsvDataS32(&ach->mSceneNum, &table, "SceneNum", i);
        MR::getCsvDataStr(&ach->mStageName, &table, "StageName", i);
        ptr[i] = ach;
    }
    return ptr;
}