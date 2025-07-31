#pragma once

#include "CustomReplaceTagFunctions.h"
{{Includes}}

static ReplaceTagFuncEntry cReplaceTagFuncTable[] = {
    { 5, 0, 0, 0, ReplaceTagProcessor::exeLocalizeGroup},
    { 9, 0, 0, 0, ReplaceTagProcessor::exeRaceTimeGroup},
    { 3, 0, 0, 0, ReplaceTagProcessor::exePictureGroup},
    { 12, 0, 0, 0, NewReplaceTagFunctions::getPowerStarNum },
    { 13, 0, 0, 0, NewReplaceTagFunctions::getTicoCoinNum },
    { 14, 0, 0, 0, NewReplaceTagFunctions::getMissCount },
{{ReplaceTagDef}}
};

static const size_t cReplaceTagFuncTableSize = sizeof(cReplaceTagFuncTable)/sizeof(cReplaceTagFuncTable[0]);