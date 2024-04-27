#pragma once

#include "ActorCreation.h"

#include "PowerStarSpawner.h"
#include "RedCoin.h"
#include "RedCoinController.h"
#include "RedCoinSwitch.h"



const CreateActorEntry cModuleCreateNameObjTable[] = {
	//DUMMY - This is never read
	{ "1234567890", NULL },
{ "PowerStarSpawner", createExtActor<pt::PowerStarSpawner> },
{ "RedCoin", createExtActor<RedCoin> },
{ "RedCoinController", createExtActor<RedCoinController> },
{ "RedCoinSwitch", createExtActor<RedCoinSwitch> },

};

const s32 cModuleCreateNameObjTableCount = sizeof(cModuleCreateNameObjTable) / sizeof(CreateActorEntry);