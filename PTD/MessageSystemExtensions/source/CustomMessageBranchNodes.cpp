#include "ModuleData_CustomMessageBranchNodes.h"


bool NewMessageFlow::isPowerStarGotCurrentStage(u16 id) {
	return MR::hasPowerStarInCurrentStage(id);
};

void TalkBranchFlowExtensions(bool b, TalkMessageCtrl* pCtrl) {
	u32* pBranchNode = (u32*)pCtrl->mTalkNodeCtrl->getCurrentNodeBranch();
	
	u16 branchType = *((u16*)pBranchNode+3);
	if (branchType > 16) {
		u16 branchParam = *((u16*)pBranchNode+4);
		u16 targetIndex = branchType;

		for (u32 i = 0; i < cBranchFlowExtSize; i++) {
			BranchFlowExt* pEntry = &cBranchFlowExt[i];
            
			if (pEntry->mIndex == targetIndex) {
				b = (*pEntry->mFuncPtr)(branchParam);
				break;
			}
		}
	}

	pCtrl->mTalkNodeCtrl->forwardCurrentBranchNode(b);
}

kmWrite32(0x8037B134, 0x7CC33378); // mr r3, r6
kmWrite32(0x8037B138, 0x7F84E378); // mr r4, r28
kmCall(0x8037B13C, TalkBranchFlowExtensions); // bl TalkBranchFlowExtensions
kmWrite32(0x8037B140, 0x4BFFFE9C); // b -0x164