#pragma once

#include "CustomReplaceTagFunctions.h"
{{Includes}}

static ReplaceTagFuncEntry cReplaceTagFuncTable[] = {
    { 5, 0, 0, 0, ReplaceTagProcessor::exeLocalizeGroup},
    { 9, 0, 0, 0, ReplaceTagProcessor::exeRaceTimeGroup},
    { 3, 0, 0, 0, ReplaceTagProcessor::exePictureGroup},
    { 12, 0, 0, 0, NewReplaceTagFunction::getPowerStarNum },
    { 13, 0, 0, 0, NewReplaceTagFunction::getTicoCoinNum },
    { 14, 0, 0, 0, NewReplaceTagFunction::getMissCount },
    { 15, 0, 0, 0, NewReplaceTagFunction::getCoinNum },
    { 16, 0, 0, 0, NewReplaceTagFunction::getStarPieceNum },
    { 17, 0, 0, 0, NewReplaceTagFunction::getPurpleCoinNum },
{{ReplaceTagDef}}
};

static const size_t cReplaceTagFuncTableSize = sizeof(cReplaceTagFuncTable)/sizeof(cReplaceTagFuncTable[0]);