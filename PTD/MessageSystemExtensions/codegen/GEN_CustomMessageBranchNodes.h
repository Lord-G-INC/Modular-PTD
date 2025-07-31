#pragma once

#include "syati.h"
#include "CustomMessageFlowNodes.h"
{{Includes}}

static BranchFlowExt cBranchFlowExt[] = {
    { 17, NewMessageFlow::isPowerStarGotCurrentStage },
{{BranchFuncDef}}
};

static size_t cBranchFlowExtSize = sizeof(cBranchFlowExt)/sizeof(cBranchFlowExt[0]);