#include "syati.h"
#include "BlueCoinUtil.h"
#include "Game/Screen/BackButton.h"



class BlueCoinList : public LayoutActor {
public:
    BlueCoinList(const char* pName);
    virtual ~BlueCoinList();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();

    void exeAppear();
    void exeWait();
    void exeClose();

    void setCursorPosition(s32 slot);
    void populateListEntries();
    void updateTextBoxes();
    void updateBlueCoinTextPane();

    struct ListEntry {
        s32 coinNum;
        s32 rangeMin;
        s32 rangeMax;
        bool isBlankSlot;
        char pStageName[48];
    };
    ListEntry* getEntry(s32 slot);
    bool isEntryBlank(ListEntry* pEntry);
    void setEntryBlank(ListEntry* pEntry);
    void setEntryNotBlank(ListEntry* pEntry);
    void printListDebugInfo();

    ListEntry* mListEntries[7];
    BackButtonCancelB* mBackButton;
    JMapInfo* mRangeTable;
    s32 mTotalCoinsInPage;
    s32 mTotalCollectedCoinsInPage;
    s32 mBlueCoinTotalCount;
    s32 mCurrentPage;
    s32 mCursorPosition;
    s32 mMaxPages;
    s32 mDefaultCursorPosition;
    s32 mDefaultPage;
};

namespace NrvBlueCoinList {
    NERVE(NrvInit);
    NERVE(NrvAppear);
    NERVE(NrvWait);
    NERVE(NrvClose);
};