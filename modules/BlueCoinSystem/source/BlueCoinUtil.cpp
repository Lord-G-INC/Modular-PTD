
#include "BlueCoin.h"
#include "BlueCoinLayouts.h"
#include "BlueCoinUtil.h"
#include "Game/Screen/GameSceneLayoutHolder.h"
#include "Game/Screen/CounterLayoutControllerExt.h"
#include "Game/NPC/TalkMessageCtrl.h"

#define BINSIZE 867

#define FLAGS_LOCATION 765
#define SPENT_LOCATION 861
#define TEXTBOX_LOCATION 864

void* initializeBlueCoinArrayAndLoadArchive() {
    BlueCoinUtil::initBlueCoinArray();
    return pt::loadArcAndFile("SystemData/BlueCoinIDRangeTable.arc", "/BlueCoinIDRangeTable.bcsv");
}

BlueCoinData* gBlueCoinData;
void* gBlueCoinIDRangeTable = initializeBlueCoinArrayAndLoadArchive();
    
namespace BlueCoinUtil {
    void loadBlueCoinData() {
        OSReport("(BlueCoinUtil) Attempting BlueCoinData.bin read.\n");
        NANDFileInfo info;
        s32 code = NANDOpen("BlueCoinData.bin", &info, 3);
        
        if (code == -12) {
            OSReport("(BlueCoinUtil) BlueCoinData.bin not found. A new one will be created on game save.\n");
        }

        if (code == 0) {
            u8* buffer = new(0x20) u8[BINSIZE];
            code = NANDRead(&info, buffer, BINSIZE);

            u32 size;
            NANDGetLength(&info, &size);

            if (size != BINSIZE) {
                char* errstr = new char[180];
                snprintf(errstr, 180, "Blue Coin Read Error\nExpected size of %d\nGot size %d\nNANDRead code: %d\nDelete or hex edit BlueCoinData.bin and try again.\n\nData locations:\nFlags: %d\nSpent: %d\nTextbox: %d", BINSIZE, size, code, FLAGS_LOCATION, SPENT_LOCATION, TEXTBOX_LOCATION);
		        u32 fg = 0xFFFFFFFF;
                u32 bg = 0;
                OSFatal(&fg, &bg, errstr);
            }

            s32 flagidx = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 255; j++) {
                    gBlueCoinData->collectionData[i][j] = *buffer++;
                }
            }

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 32; j++) {
                    gBlueCoinData->flags[i][j] = (bool)buffer[flagidx];
                    flagidx++;
                }
            }

            for (s32 i = 0; i < 3; i++) {
                gBlueCoinData->spentData[i] = buffer[(SPENT_LOCATION-765)+i];
                gBlueCoinData->hasSeenTextBox[i] = (bool)buffer[(TEXTBOX_LOCATION-765)+i];


                if (gBlueCoinData->spentData[i] > getTotalBlueCoinNum(i, false)) {
                    u8 total = getTotalBlueCoinNum(i, false);
                    u8 spent = gBlueCoinData->spentData[i];

                    OSReport("(BlueCoinUtil) Discrepancy found on save file %d, c: %d, s: %d, c-s: %d\n", i+1, total, spent, total-spent);

                    while (total-gBlueCoinData->spentData[i] < 0) {
                        gBlueCoinData->spentData[i]--;
                    }

                }
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
                s32 idx = 0;
                s32 flagidx = 0;

                for (int i = 0; i < 3; i++) {
                    buffer[SPENT_LOCATION+i] = gBlueCoinData->spentData[i];
                    buffer[TEXTBOX_LOCATION+i] = (bool)gBlueCoinData->hasSeenTextBox[i];

                    for (int j = 0; j < 255; j++) {
                        buffer[idx++] = (bool)gBlueCoinData->collectionData[i][j];
                    }

                    for (int j = 0; j < 32; j++) {
                        buffer[FLAGS_LOCATION+flagidx] = gBlueCoinData->flags[i][j];
                        flagidx++;
                    }
                }
                
                code = NANDWrite(&info, buffer, BINSIZE);

                if (code != BINSIZE) {
                    char* errstr = new char[180];
                    snprintf(errstr, 180, "Blue Coin Write Error\nExpected size of %d\nNANDRead code: %d\n\nData locations:\nFlags: %d\nSpent: %d\nTextbox: %d", BINSIZE, code, FLAGS_LOCATION, SPENT_LOCATION, TEXTBOX_LOCATION);
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
                if (j == 8 || j == 17 || j == 26) // hoping to solve this with math
                    flagstr[i][j] = 0x20;
                else {
                    flagstr[i][j] = gBlueCoinData->flags[i][flagidx] ? 0x31 : 0x30;
                    flagidx++;
                }
            }

            for (s32 j = 0; j < 255; j++) {
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
            memset(gBlueCoinData->collectionData[i], 0, 255);
            gBlueCoinData->hasSeenTextBox[i] = 0;
            gBlueCoinData->spentData[i] = 0;
            gBlueCoinData->hasSeenTextBox[i] = 0;

            for (int j = 0; j < 32; j++) {
                gBlueCoinData->flags[i][j] = 0;
            }
        }
        
        OSReport("(BlueCoinUtil) Blue Coin array cleared.\n");
    }

    void initBlueCoinArray() {
        OSReport("(BlueCoinUtil) Initializing Blue Coin array.\n");
        gBlueCoinData = new BlueCoinData;
        gBlueCoinData->collectionData = new bool*[3];
        for (int i = 0; i < 3; i++) {
            gBlueCoinData->collectionData[i] = new bool[255];
            memset(gBlueCoinData->collectionData[i], 0, 255);
            gBlueCoinData->hasSeenTextBox[i] = 0;
            gBlueCoinData->spentData[i] = 0;
        }
        OSReport("(BlueCoinUtil) Blue Coin array initialization complete.\n");
    }

    s32 getCurrentFileNum() {
        return GameDataFunction::getSaveDataHandleSequence()->mCurrentFileNum-1;
    }

    void setBlueCoinGotCurrentFile(u8 id) {
        gBlueCoinData->collectionData[getCurrentFileNum()][id] = true;
        OSReport("(BlueCoinUtil) Blue Coin ID #%d collected on file %d.\n", id, getCurrentFileNum());
    }

    bool isBlueCoinGot(u8 file, u8 id) {
        return gBlueCoinData->collectionData[file][id];
    }
    
    bool isBlueCoinGotCurrentFile(u8 id) {
        return gBlueCoinData->collectionData[getCurrentFileNum()][id];
    }

    bool hasSeenBlueCoinTextBoxCurrentFile() {
        return gBlueCoinData->hasSeenTextBox[getCurrentFileNum()];
    }  

    void setSeenBlueCoinTextBoxCurrentFile() {
        gBlueCoinData->hasSeenTextBox[getCurrentFileNum()] = true;
        OSReport("(BlueCoinUtil) gBlueCoinData->hasSeenTextBox on the current file set to true.\n");
    }

    void setOnBlueCoinFlagCurrentFile(u8 flag) {
        gBlueCoinData->flags[getCurrentFileNum()][flag] = true;
        OSReport("(BlueCoinUtil) Flag %d set on file %d\n", flag, getCurrentFileNum());
    }

    bool isOnBlueCoinFlagCurrentFile(u8 flag) {
        return gBlueCoinData->flags[getCurrentFileNum()][flag];
    }

    void resetAllBlueCoin(u8 file) {
        file--;
        memset(gBlueCoinData->collectionData[file], 0, 255);
        gBlueCoinData->spentData[file] = 0;
        gBlueCoinData->hasSeenTextBox[file] = 0;

        for (int i = 0; i < 32; i++) {
            gBlueCoinData->flags[file][i] = 0;
        }

        saveBlueCoinData();
        OSReport("(BlueCoinUtil) Blue Coin data for file %d reset.\n", file);
    }

    bool isBlueCoinTextBoxAppeared() {
        BlueCoinCounter* pCounter = ((CounterLayoutControllerExt*)MR::getGameSceneLayoutHolder()->mCounterLayoutController)->mBlueCoinCounter;

        if (!pCounter || hasSeenBlueCoinTextBoxCurrentFile())
            return false;
        else
            return pCounter->isNerve(&NrvBlueCoinCounter::NrvShowTextBox::sInstance);
    }

    void startCounterCountUp() {
        BlueCoinCounter* pCounter = ((CounterLayoutControllerExt*)MR::getGameSceneLayoutHolder()->mCounterLayoutController)->mBlueCoinCounter;
        
        if (pCounter)
            pCounter->startCountUp();
    }

    void spendBlueCoinCurrentFile(u8 numcoin) {
        numcoin == 0 ? 30 : numcoin;
        
        if (getTotalBlueCoinNumCurrentFile(true) >= numcoin) {
            gBlueCoinData->spentData[getCurrentFileNum()] += numcoin;
            OSReport("(BlueCoinUtil) Transaction of %d Blue Coins accepted\n", numcoin);
        }
        else
            OSReport("(BlueCoinUtil) Transaction of %d Blue Coins not accepted. Too few coins.\n", numcoin);
    }

    s32 getSpentBlueCoinNum(u8 file) {
        return gBlueCoinData->spentData[file];
    }

    s32 getSpentBlueCoinNumCurrentFile() {
        return gBlueCoinData->spentData[getCurrentFileNum()];
    }

    s32 getTotalBlueCoinNum(u8 file, bool ignoreSpent) {
        s32 total = 0;
        for (s32 i = 0; i < 255; i++) {
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

            OSReport("(BlueCoinIDRangeTable) Stage name: \"%s\", Range: %d through %d, Line: %d, Collected Coins Only: %d, Total: %d\n", pStageName, rangeMin, rangeMax, targetLine, collectedCoinsOnly, count);
            return count;
        }
        
        OSReport("(BlueCoinIDRangeTable) Stage name \"%s\" not found in table. -1 returned!\n", pStageName);
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


    void getBlueCoinPaneNameFromTable(LayoutActor* pLayout, const char* pStageName) {
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

        s32 texID = 0;  
        MR::getCsvDataS32(&texID, &table, "TexID", targetLine);
        char paneName[10];
        snprintf(paneName, 10, "ShaCoin%02d", texID);
        OSReport("(BlueCoinIDRangeTable) Pane \"ShaCoin%02d\" selected.\n", texID);
        MR::showPaneRecursive(pLayout, (const char*)paneName);
    }

    LiveActor* createBlueCoinForSpawning(LiveActor* pSourceActor, s32 id) {
        BlueCoin* coin = new BlueCoin("BlueCoinS");
        MR::addToCoinHolder(pSourceActor, coin);
        coin->mID = id;
        coin->initWithoutIter();
        MR::hideModel(coin);
        MR::invalidateHitSensors(coin);
        pSourceActor->mActionKeeper->mItemGenerator = 0;
        OSReport("(BlueCoinUtil) Created Blue Coin with ID %d for the actor \"%s\".\n", id, pSourceActor->mName);
        return coin;
    }

    void appearBlueCoin(LiveActor* pSourceActor, LiveActor* pBlueCoin) {
        TVec3f coinVelocity = TVec3f(0.0f, 25.0f, 0.0f);
        coinVelocity.scale(coinVelocity.y, -pSourceActor->mGravity);

        MR::startSystemSE("SE_SY_PURPLE_COIN_APPEAR", -1, -1);

        ((BlueCoin*)pBlueCoin)->appearMove(pSourceActor->mTranslation, coinVelocity, 0x7FFFFFFF, 60);
    }
}

// Blue coin binary management

// Delete all blue coins in a save file.
void resetAllBlueCoinOnDeleteFile(SaveDataHandleSequence* pSeq, UserFile* pFile, int fileID) {
    pSeq->restoreUserFileConfigData(pFile, fileID); // Restore original call
    BlueCoinUtil::resetAllBlueCoin(fileID);
}

kmCall(0x804D9BF8, resetAllBlueCoinOnDeleteFile);

// Save gBlueCoinData->collectionData to file.

void saveBlueCoinDataOnGameSave(const char* pName) {
    MR::startSystemSE(pName, -1, -1);
    BlueCoinUtil::saveBlueCoinData();
}

kmCall(0x804DAFD0, saveBlueCoinDataOnGameSave);

// Read blue coin binary on title screen load.
void onTitleScreenLoad(LiveActor* pActor) {
    pActor->initHitSensor(1); // Restore original call

    BlueCoinUtil::clearBlueCoinData();
    BlueCoinUtil::loadBlueCoinData();
}

kmCall(0x8024F358, onTitleScreenLoad);