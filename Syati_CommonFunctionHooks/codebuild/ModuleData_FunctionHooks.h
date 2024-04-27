#pragma once





typedef void (*HookFuncDef)();

const HookFuncDef cModuleGameSceneStartFuncTable[] = {
    NULL,

};
const s32 cModuleGameSceneStartFuncTableCount = sizeof(cModuleGameSceneStartFuncTable) / sizeof(HookFuncDef);


const HookFuncDef cModuleAnySceneDestroyedFuncTable[] = {
    NULL,

};
const s32 cModuleAnySceneDestroyedFuncTableCount = sizeof(cModuleAnySceneDestroyedFuncTable) / sizeof(HookFuncDef);


const HookFuncDef cModuleGameSystemFrameLoopTable[] = {
    NULL,

};
const s32 cModuleGameSystemFrameLoopTableCount = sizeof(cModuleGameSystemFrameLoopTable) / sizeof(HookFuncDef);