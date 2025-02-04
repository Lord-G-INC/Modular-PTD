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

    BackButtonCancelB* mBackButton;
};

namespace NrvBlueCoinList {
    NERVE(NrvInit);
    NERVE(NrvAppear);
    NERVE(NrvWait);
    NERVE(NrvClose);
};