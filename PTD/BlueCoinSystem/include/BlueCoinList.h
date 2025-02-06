#include "syati.h"
#include "BlueCoinUtil.h"
#include "Game/Screen/BackButton.h"

struct ListEntry {
    s32 coinNum;
    s32 rangeMin;
    s32 rangeMax;
    char pStageName[48];
    bool isBlankSlot;
};

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

    BackButtonCancelB* mBackButton;
    JMapInfo* mRangeTable;
    ListEntry* mListEntries[7];
    s32 mCurrentPage;
    s32 mCursorPosition;
    s32 mBlueCoinPageCount;
};

namespace NrvBlueCoinList {
    NERVE(NrvInit);
    NERVE(NrvAppear);
    NERVE(NrvWait);
    NERVE(NrvClose);
};