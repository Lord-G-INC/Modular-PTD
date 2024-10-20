#include "Game/System/StationedArchiveLoader.h"

static StationedFileInfoEntry cNewStationedFileEntries[] = {
    {0, 0, 0},
    {0, 6, "/ObjectData/BlueCoin.arc"},
    {0, 6, "/ObjectData/BlueCoinClear.arc"},
    {0, 8, "/LayoutData/BlueCoinCounterInStage.arc"},
    {0, 9, 0}
};

const size_t cNumNewStationedFileEntries = sizeof(cNewStationedFileEntries)/12;