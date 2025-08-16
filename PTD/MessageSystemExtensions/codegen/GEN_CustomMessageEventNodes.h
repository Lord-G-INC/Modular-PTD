#pragma once

#include "syati.h"
#include "CustomMessageFlowNodes.h"
{{Includes}}

static EventFlowExt cEventFlowExt[] = {
    { 12, NewEventFlow::warpPlayer },
{{EventFuncDef}}
};

static size_t cEventFlowExtSize = sizeof(cEventFlowExt)/sizeof(cEventFlowExt[0]);