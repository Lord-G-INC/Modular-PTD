#include "Game/Screen/CounterLayoutController.h"

class BlueCoinCounter;
class CounterLayoutControllerExt : public CounterLayoutController {
public:
    CounterLayoutControllerExt();
    s32* mGlobalLayoutHolder;
    BlueCoinCounter* mBlueCoinCounter;
};