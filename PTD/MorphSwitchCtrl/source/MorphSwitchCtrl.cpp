#include "pt/Map/MorphSwitchCtrl.h"
#include "Game/Player/MarioActor.h"
#include "Game/Player/MarioState.h"

extern "C" {
    void isMsgYoshiLoopTongue__2MRFUl(u32 msg);
}

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
			/*if (mAction) {
				check = isMsgYoshiLoopTongue__2MRFUl(u32 msg);
			}
			else {
				check = MR::isPlayerElementModeYoshi();
			}*/
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