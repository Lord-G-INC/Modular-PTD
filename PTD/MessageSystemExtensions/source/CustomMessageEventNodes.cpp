#include "ModuleData_CustomMessageEventNodes.h"

void NewEventFlow::warpPlayer(u16 param) {
    char buf[20];
    snprintf(buf, 20, "EventFlowPos%03d", param);
    MR::setPlayerPos(buf);
}

bool TalkEventFlowExtensions(TalkMessageCtrl* pCtrl) {
    u32* pNode = (u32*)pCtrl->mTalkNodeCtrl->getCurrentNodeEvent();
    u16 eventType = *((u16*)pNode+2);
    
    if (eventType > 11) {
        u16 eventParam = *((u16*)pNode+5);
        u32 eventIndex = eventType;

        for (u16 i = 0; i < cEventFlowExtSize; i++) {
            EventFlowExt* pEntry = &cEventFlowExt[i];

            if (pEntry->mIndex == eventIndex) {
                (*pEntry->mFuncPtr)(eventParam);
                break;
            }
        }
    }

    return pCtrl->mTalkNodeCtrl->isExistNextNode();
}

kmWrite32(0x8037B388, 0x7FA3EB78); // mr r3, r29
kmCall(0x8037B38C, TalkEventFlowExtensions); // bl TalkEventFlowExtensions