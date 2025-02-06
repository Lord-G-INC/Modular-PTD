#include "BlueCoin.h"
#include "BlueCoinCounter.h"
#include "BlueCoinUtil.h"
#include "Game/Screen/GameSceneLayoutHolder.h"
#include "Game/Screen/CounterLayoutControllerExt.h"
#include "Game/NPC/TalkMessageCtrl.h"
#include "Game/MapObj/FileSelector.h"
#include "Game/LiveActor/ExtActorActionKeeper.h"

#define BINSIZE 873

#define FLAGS_LOCATION 768
#define SPENT_LOCATION 864
#define TEXTBOX_LOCATION 870

void* initializeBlueCoinArrayAndLoadTable() {
    BlueCoinUtil::initBlueCoinArray();
    return pt::loadArcAndFile("/SystemData/BlueCoinIDRangeTable.arc", "/BlueCoinIDRangeTable.bcsv", 0);
}

BlueCoinData* gBlueCoinData;
void* gBlueCoinIDRangeTable = initializeBlueCoinArrayAndLoadTable();
    
namespace BlueCoinUtil {
    void loadBlueCoinData() {
        OSReport("(BlueCoinUtil) Attempting BlueCoinData.bin read.\n");
        NANDFileInfo info;
        s32 code = NANDOpen("BlueCoinData.bin", &info, 3);
        
        if (code == -12) {
            OSReport("(BlueCoinUtil) BlueCoinData.bin not found. A new one will be created on game save.\n");
        }

        if (code == 0) {
            u32 size;
            NANDGetLength(&info, &size);

            if (size != BINSIZE) {
                if (MR::testCorePadButtonB(0)) {
                    saveBlueCoinData();
                }
                else {
                    char errstr[200];
                    snprintf(errstr, 200, "Blue Coin Read Error\nExpected size of %d\nGot size %d\nNANDRead code: %d\nDelete or hex edit BlueCoinData.bin and try again.\n\nHold B when this error appears to reset BlueCoinData.bin.\n", BINSIZE, size, code);
                    u32 fg = 0xFFFFFFFF;
                    u32 bg = 0;
                    OSFatal(&fg, &bg, errstr);
                }
            }

            u8* buffer = new(0x20) u8[BINSIZE];
            code = NANDRead(&info, buffer, BINSIZE);

            for (int i = 0; i < 3; i++) {
                memcpy(gBlueCoinData->collectionData[i], &buffer[256*i], 256);

                memcpy(gBlueCoinData->flags[i], &buffer[FLAGS_LOCATION+(32*i)], 32);

                memcpy(&gBlueCoinData->spentData[i], &buffer[SPENT_LOCATION+(2*i)], 2);

                gBlueCoinData->hasSeenTextBox[i] = buffer[TEXTBOX_LOCATION+i];
            }

            delete [] buffer;
            OSReport("(BlueCoinUtil) BlueCoinData.bin successfully read.\n");
            printBlueCoinSaveFileInfo();
        }
        NANDClose(&info);
    }

    void saveBlueCoinData() {
        s32 code = NANDCreate("BlueCoinData.bin", 0x30, 0);
        if (code == 0 || code == -6) {
            NANDFileInfo info;
            code = NANDOpen("BlueCoinData.bin", &info, 3);
            if (code == 0) {
                NANDSeek(&info, 0, 0);
                u8* buffer = new(0x20) u8[BINSIZE];

                for (int i = 0; i < 3; i++) {
                    memcpy(&buffer[256*i], gBlueCoinData->collectionData[i], 256);

                    memcpy(&buffer[FLAGS_LOCATION+(32*i)], gBlueCoinData->flags[i], 32);

                    memcpy(&buffer[SPENT_LOCATION+(2*i)], &gBlueCoinData->spentData[i], 2);

                    buffer[TEXTBOX_LOCATION+i] = gBlueCoinData->hasSeenTextBox[i];
                }
                
                code = NANDWrite(&info, buffer, BINSIZE);

                if (code != BINSIZE) {
                    char errstr[180];
                    snprintf(errstr, 180, "Blue Coin Write Error\nExpected size of %d\nNANDRead code: %d\n", BINSIZE, code);
		            u32 fg = 0xFFFFFFFF;
                    u32 bg = 0;
                    OSFatal(&fg, &bg, errstr);
                }

                delete [] buffer;
                OSReport("(BlueCoinUtil) BlueCoinData.bin successfully saved.\n");
                printBlueCoinSaveFileInfo();
            }
            NANDClose(&info);
        }
    }

    void printBlueCoinSaveFileInfo() {
        char flagstr[3][36];
        s32 numcoins[3];

        for (s32 i = 0; i < 3; i++) {
            flagstr[i][35] = 0;
            numcoins[i] = 0;
            s32 flagidx = 0;
            
            for (s32 j = 0; j < 35; j++) {
                if (j % 9 == 8)
                    flagstr[i][j] = 0x20;
                else {
                    flagstr[i][j] = gBlueCoinData->flags[i][flagidx] ? 0x31 : 0x30;
                    flagidx++;
                }
            }

            for (s32 j = 0; j < 256; j++) {
                if (gBlueCoinData->collectionData[i][j])
                    numcoins[i]++;
            }
        }

        OSReport("Blue Coin save file info\nc0: %d, c1: %d, c2: %d\nf0: %s\nf1: %s\nf2: %s\ns0: %d, s1: %d, s2: %d\nm0: %s, m1: %s, m2: %s\n", 
        numcoins[0], 
        numcoins[1], 
        numcoins[2],
        flagstr[0],
        flagstr[1],
        flagstr[2],
        gBlueCoinData->spentData[0],
        gBlueCoinData->spentData[1],
        gBlueCoinData->spentData[2],
        gBlueCoinData->hasSeenTextBox[0] ? "Yes" : "No",
        gBlueCoinData->hasSeenTextBox[1] ? "Yes" : "No",
        gBlueCoinData->hasSeenTextBox[2] ? "Yes" : "No"
        );
    }

    void clearBlueCoinData() {
        for (int i = 0; i < 3; i++) {
            memset(gBlueCoinData->collectionData[i], 0, 256);
            memset(gBlueCoinData->flags[i], 0, 32);
            gBlueCoinData->spentData[i] = 0;
            gBlueCoinData->hasSeenTextBox[i] = 0;
        }
        
        OSReport("(BlueCoinUtil) Blue Coin array cleared.\n");
    }

    void initBlueCoinArray() {
        gBlueCoinData = new BlueCoinData;
        gBlueCoinData->collectionData = new bool*[3];
        for (int i = 0; i < 3; i++) {
            gBlueCoinData->collectionData[i] = new bool[256];
            memset(gBlueCoinData->collectionData[i], 0, 256);
            memset(gBlueCoinData->flags[i], 0, 32);
            gBlueCoinData->hasSeenTextBox[i] = 0;
            gBlueCoinData->spentData[i] = 0;
        }
        OSReport("(BlueCoinUtil) Blue Coin array initialized.\n");
    }

    s32 getCurrentFileNum() {
        return GameDataFunction::getSaveDataHandleSequence()->mCurrentFileNum-1;
    }

    void setBlueCoinGotCurrentFile(u16 id) {
        gBlueCoinData->collectionData[getCurrentFileNum()][id] = true;
    }

    bool isBlueCoinGot(u8 file, u16 id) {
        return gBlueCoinData->collectionData[file][id];
    }
    
    bool isBlueCoinGotCurrentFile(u16 id) {
        return gBlueCoinData->collectionData[getCurrentFileNum()][id];
    }

    bool hasSeenBlueCoinTextBoxCurrentFile() {
        return gBlueCoinData->hasSeenTextBox[getCurrentFileNum()];
    }  

    void setSeenBlueCoinTextBoxCurrentFile() {
        gBlueCoinData->hasSeenTextBox[getCurrentFileNum()] = true;
    }

    void setOnBlueCoinFlagCurrentFile(u8 flag) {
        gBlueCoinData->flags[getCurrentFileNum()][flag] = true;
    }

    bool isOnBlueCoinFlagCurrentFile(u8 flag) {
        return gBlueCoinData->flags[getCurrentFileNum()][flag];
    }

    void resetAllBlueCoin(u8 file) {
        file--;
        memset(gBlueCoinData->collectionData[file], 0, 256);
        gBlueCoinData->spentData[file] = 0;
        gBlueCoinData->hasSeenTextBox[file] = 0;

        for (int i = 0; i < 32; i++) {
            gBlueCoinData->flags[file][i] = 0;
        }
    }

    bool isBlueCoinTextBoxAppeared() {
        return MR::isDemoActive("BlueCoinFirstTimeText");
    }

    void setCounter() {
        CounterLayoutControllerExt* pCounterLayoutControllerExt = (CounterLayoutControllerExt*)MR::getGameSceneLayoutHolder()->mCounterLayoutController;
        BlueCoinCounter* pCounter = pCounterLayoutControllerExt->mBlueCoinCounter;
        
        if (pCounter)
            pCounter->setCounter();
    }

    void showTextBox() {
        ((CounterLayoutControllerExt*)MR::getGameSceneLayoutHolder()->mCounterLayoutController)->mBlueCoinCounter->setNerve(&NrvBlueCoinCounter::NrvShowTextBox::sInstance);
    }

    void spendBlueCoinCurrentFile(u8 numcoin) {
        numcoin == 0 ? 30 : numcoin;
        
        if (getTotalBlueCoinNumCurrentFile(true) >= numcoin)
            gBlueCoinData->spentData[getCurrentFileNum()] += numcoin;
    }

    s32 getSpentBlueCoinNum(u8 file) {
        return gBlueCoinData->spentData[file];
    }

    s32 getSpentBlueCoinNumCurrentFile() {
        return gBlueCoinData->spentData[getCurrentFileNum()];
    }

    s32 getTotalBlueCoinNum(u8 file, bool ignoreSpent) {
        s32 total = 0;
        for (s32 i = 0; i < 256; i++) {
            if (gBlueCoinData->collectionData[file][i])
                total++;
        }

        if (ignoreSpent) 
            return total -= getSpentBlueCoinNum(file);
        else
            return total;
    }

    s32 getTotalBlueCoinNumCurrentFile(bool ignoreSpent) {
        return getTotalBlueCoinNum(getCurrentFileNum(), ignoreSpent);
    }

    s32 getBlueCoinRangeData(const char* pStageName, bool collectedCoinsOnly) {
        JMapInfo table = JMapInfo();
        table.attach(gBlueCoinIDRangeTable);

        const char* tableStageName;
        s32 targetLine = -1;

        if (!pStageName)
            pStageName = MR::getCurrentStageName();

        for (s32 i = 0; i < MR::getCsvDataElementNum(&table); i++) {
            MR::getCsvDataStr(&tableStageName, &table, "StageName", i);

            if (MR::isEqualString(pStageName, tableStageName)) {
                targetLine = i;
                break;
            }
        }

        if (targetLine > -1) {
            s32 rangeMin;
            s32 rangeMax;
            s32 count = 0;

            MR::getCsvDataS32(&rangeMin, &table, "BlueCoinRangeMin", targetLine);
            MR::getCsvDataS32(&rangeMax, &table, "BlueCoinRangeMax", targetLine);

            for (u32 i = rangeMin; i < rangeMax + 1; i++) {
                if (collectedCoinsOnly) {
                    if (isBlueCoinGotCurrentFile(i))
                        count++;
                }
                else
                    count++;
            }

            return count;
        }
        
        return -1;
    }
    
    s32 getBlueCoinRange(const char* pStageName, bool minOrMax) {
        JMapInfo table = JMapInfo();
        table.attach(gBlueCoinIDRangeTable);

        const char* tableStageName;
        s32 targetLine = -1;

        if (!pStageName)
            pStageName = MR::getCurrentStageName();

        for (s32 i = 0; i < MR::getCsvDataElementNum(&table); i++) {
            MR::getCsvDataStr(&tableStageName, &table, "StageName", i);

            if (MR::isEqualString(pStageName, tableStageName)) {
                targetLine = i;
                break;
            }
        }

        if (targetLine > -1) {
            s32 val = 0;
            MR::getCsvDataS32(&val, &table, minOrMax ? "BlueCoinRangeMax" : "BlueCoinRangeMin", targetLine);
            return val;
        }
        return -1;
    }

    JMapInfo* getBlueCoinIDRangeTable() {
        JMapInfo* table = new JMapInfo();
        table->attach(gBlueCoinIDRangeTable);
        return table;
    }

    bool tryCreateBlueCoinForSpawningActorActionKeeper(LiveActor* pSourceActor, s32 id) {
        if (id > -1) {
            BlueCoin* coin = new BlueCoin("BlueCoinS");
            MR::addToCoinHolder(pSourceActor, coin);
            coin->mID = id;
            coin->initWithoutIter();
            MR::hideModel(coin);
            MR::invalidateHitSensors(coin);
            ExtActorActionKeeper* pKeeper = (ExtActorActionKeeper*)pSourceActor->mActionKeeper;
            pKeeper->mBlueCoin = coin;
            pKeeper->mItemGenerator = 0;
            return true;
        }
        return false;
    }

    void appearBlueCoinActionKeeper(LiveActor* pSourceActor, TVec3f& rPos) {
        ExtActorActionKeeper* pKeeper = (ExtActorActionKeeper*)pSourceActor->mActionKeeper;
        if (pKeeper->mBlueCoin) {
            TVec3f coinVelocity = TVec3f(0.0f, 25.0f, 0.0f);
            coinVelocity.scale(coinVelocity.y, -pSourceActor->mGravity);

            MR::startSystemSE("SE_SY_PURPLE_COIN_APPEAR", -1, -1);

            pKeeper->mBlueCoin->appearMove(rPos, coinVelocity, 0x7FFFFFFF, 60);
        }
    }

    bool isValidBlueCoinActionKeeper(LiveActor* pSourceActor) {
        ExtActorActionKeeper* pKeeper = (ExtActorActionKeeper*)pSourceActor->mActionKeeper;
        return pKeeper->mBlueCoin;
    }
}

// Blue coin binary management

// Delete all blue coins in a save file.
void resetAllBlueCoinOnDeleteFile(SaveDataHandleSequence* pSeq, UserFile* pFile, int fileID) {
    pSeq->restoreUserFileConfigData(pFile, fileID); // Restore original call
    BlueCoinUtil::resetAllBlueCoin(fileID);
}

kmCall(0x804D9BF8, resetAllBlueCoinOnDeleteFile); // bl resetAllBlueCoinOnDeleteFile

// Save gBlueCoinData->collectionData to file.

void saveBlueCoinDataOnGameSave(const char* pName) {
    MR::startSystemSE(pName, -1, -1);
    BlueCoinUtil::saveBlueCoinData();
}

#if defined TWN || defined KOR
    kmCall(0x804DB060, saveBlueCoinDataOnGameSave); // bl saveBlueCoinDataOnGameSave
#else
    kmCall(0x804DAFD0, saveBlueCoinDataOnGameSave); // bl saveBlueCoinDataOnGameSave
#endif

// Read blue coin binary on title screen load.
void onTitleScreenLoad(FileSelector* pFileSelector) {
    pFileSelector->initHitSensor(1); // Restore original call

    BlueCoinUtil::clearBlueCoinData();
    BlueCoinUtil::loadBlueCoinData();
}

kmCall(0x8024F358, onTitleScreenLoad); // bl saveBlueCoinDataOnGameSave