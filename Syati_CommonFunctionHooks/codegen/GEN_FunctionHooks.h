#pragma once

{{IncludeListOnGameSceneStart}}
{{IncludeListAnySceneDestroyed}}
{{IncludeListGameSystemFrameLoop}}

typedef void (*HookFuncDef)();

const HookFuncDef cModuleGameSceneStartFuncTable[] = {
    NULL,
{{GameSceneStartFuncList}}
};
const s32 cModuleGameSceneStartFuncTableCount = sizeof(cModuleGameSceneStartFuncTable) / sizeof(HookFuncDef);


const HookFuncDef cModuleAnySceneDestroyedFuncTable[] = {
    NULL,
{{AnySceneDestroyedFuncList}}
};
const s32 cModuleAnySceneDestroyedFuncTableCount = sizeof(cModuleAnySceneDestroyedFuncTable) / sizeof(HookFuncDef);


const HookFuncDef cModuleGameSystemFrameLoopTable[] = {
    NULL,
{{GameSystemFrameLoopList}}
};
const s32 cModuleGameSystemFrameLoopTableCount = sizeof(cModuleGameSystemFrameLoopTable) / sizeof(HookFuncDef);