#pragma once
#include "syati.h"
#include "Game/AreaObj/AreaObjContainer.h"



static ManagerEntry cModuleCreateAreaObjMgrTable[] = {
    { "ChildSwitchArea", 64, AreaObjContainer::createManager<AreaObjMgr> },

};

const s32 cModuleCreateAreaObjMgrTableCount = sizeof(cModuleCreateAreaObjMgrTable) / sizeof(ManagerEntry);
