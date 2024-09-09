
#pragma once

#include "Game/Map/FileSelectIconID.h"
#include "Game/Map/FileSelectItem.h"
#include "Game/Map/FileSelectModel.h"
#include "Game/Screen/MiiSelect.h"
#include "Game/Screen/MiiSelectIcon.h"
#include "revolution.h"
#include "kamek/hooks.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "Game/LiveActor.h"
#include "Game/Util.h"

namespace pt {
    extern void* loadArcAndFile(const char *pArc, const char *pFile, JKRHeap *pHeap);
}