/*
* A switch that turns on its SW_A if the player morphs.
* 
* Obj_arg0 (s32): Morphs to check for
* 	-> -1: Normal
*		0: Bee
*		1: Boo
*		   (when obj_arg2 is true: if Boo Mario vanishes)
*		2: Cloud
*		3: Spin Drill
*		   (when obj_arg2 is true: if Mario is currently using the Drill)
*		4: Red Star
*		   (when obj_arg2 is true: if Mario is flying)
*		   NOTE: Only works when using the MorphFooAndIce module!
*		5: Fire
*		6: Ice
*		   NOTE: Only works when using the MorphFooAndIce module!
*		7: Invincible
*		8: Rock
*		   (when obj_arg2 is true: if Rock Mario is rolling)
*		9: Hopper (Spring)
*		10: Yoshi
*		11: Blimp Fruit Yoshi
*		12: Bulb Berry Yoshi
*		13: Dash Pepper Yoshi
*
* Obj_arg1 (bool): Deactivate?
*	-> Should the SW_A be deactivated after the morph is complete?
*
* Obj_arg2 (bool): Perform action?
*	-> Check obj_arg0 to see what this changes.
*/
#include "MorphSwitchCtrl.h"
#include "Game/Player/MarioActor.h"
#include "Game/Player/MarioState.h"

MorphSwitchCtrl::MorphSwitchCtrl(const char* pName) : NameObj(pName) {
	mMorph = -1;
	mDeactivate = false;
}

void MorphSwitchCtrl::init(JMapInfoIter const& rIter) {
	MR::connectToSceneMapObjMovement(this);
	
	MR::getJMapInfoArg0NoInit(rIter, &mMorph); // Bitfield of morphs
	MR::getJMapInfoArg1NoInit(rIter, &mDeactivate); // Deactivate when no longer checked
	MR::getJMapInfoArg2NoInit(rIter, &mAction); // Check for a specific action (Teresa | Drill | Flying | Rock | Yoshi)

	MR::registerDemoSimpleCastAll(this);

	mStageSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

void MorphSwitchCtrl::movement() {
	bool check = 0;

	MarioActor *pMarioActor = MarioAccess::getPlayerActor();

	switch (mMorph) {
		default: 
			check = MR::isPlayerElementModeNormal();
			break;
			
		case 0:
			check = MR::isPlayerElementModeBee();
			break;
		
		case 1:
			if (mAction) {
				check = MR::isPlayerTeresaDisappear();
			}
			else {
				check = MR::isPlayerElementModeTeresa();
			}
			break;
		
		case 2:
			check = MR::isPlayerElementModeCloud();
			break;
		
		case 3: // Drill
			if (mAction) {
				check = MR::isCurrentRushItemDrill();
			}
			else {
				check = MarioAccess::getTakingSensor() && MR::isName(MR::getSensorHost(MarioAccess::getTakingSensor()), "アイテムドリル");
			}
			break;
		
		case 4: // Foo
			if (mAction) {
				check = MR::isPlayerFlying();
			}
			else {
				check = pMarioActor->mPlayerMode == 7;
			}
			break;
		
		case 5:
			check = MR::isPlayerElementModeFire();
			break;
		
		case 6:
			check = MR::isPlayerElementModeIce();
			break;
		
		case 7:
			check = MR::isPlayerElementModeInvincible();
			break;
		
		case 8:
			if (mAction) {
				check = MR::isCurrentRushRockMario();
			}
			else {
				check = MR::isPlayerElementModeRock();
			}
			break;
		
		case 9:
			check = MR::isPlayerElementModeHopper();
			break;
		
		case 10:
			check = MR::isPlayerElementModeYoshi();
			break;
		
		case 11: // Blimp
			check = MarioAccess::getPlayerActor()->mMario->isStatusActive(0x27);
			break;
		
		case 12:
			check = MR::isPlayerElementModeYoshiLight();
			break;
		
		case 13:
			check = MR::isPlayerElementModeYoshiDash();
		
	}

	// Set switch when condition is met
	if (check) {
		mStageSwitchCtrl->onSwitchA();
	}
	if (!check && mDeactivate) {
		mStageSwitchCtrl->offSwitchA();
	}
}