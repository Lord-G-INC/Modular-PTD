#include "SuperSpinDriverCustomColors.h"
#include "Game/MapObj/SuperSpinDriver.h"
#include "Game/Util/ActorAnimUtil.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JUTTextureHolder.h"

/*
* Authors: Evanbowl and Lord Giganticus
*/

namespace pt {
	namespace {
	/*
	* Green + Custom Launch Stars
	*
	* Unsurprisingly, all the BTP and BRK frames for the green Launch Star is still found inside SuperSpinDriver.arc. Here,
	* we hijack calls to initColor in order to check for the green color first. If the Launch Star's color is set to green,
	* we apply its animation frames. Otherwise, we call initColor to set up the other colors.
	*
	* This code also allows new BTP frames and SpinDriverPath textures to be loaded and used.
	* Support for new BRK frames may be added in the future.
	*/

	const char* gColorsStr[] = {"Red.bti", "Blue.bti", "Rainbow.bti", "Purple.bti", "Black.bti", "White.bti", "Yellow.bti"};

	JUTTextureHolder gSpinDriverPathColors = arrsize(gColorsStr);

	void initSuperSpinDriverCustomColor(SuperSpinDriver *pActor) {
		s32 color = pActor->mColor;

		if (color == 0 || color == 2) {
			pActor->initColor();
		} 
		else {
			MR::startBtpAndSetFrameAndStop(pActor, "SuperSpinDriver", color);
			MR::startBrk(pActor, color == 1 ? "Green" : "Red");

			pActor->mSpinDriverPathDrawer->mColor = color == 1 ? 0 : color;
		}
			
        if (color >= 3)
            gSpinDriverPathColors.SetTexture(color - 3, new JUTTexture(MR::loadTexFromArc("SpinDriverPath.arc", gColorsStr[color - 3], 0), 0));
	}

	kmCall(0x8031E29C, initSuperSpinDriverCustomColor); // redirect initColor in init

	void setSpinDriverPathCustomColor(SpinDriverPathDrawer* pDrawer) {
		if (pDrawer->mColor >= 3)
			gSpinDriverPathColors[pDrawer->mColor - 3]->load(GX_TEXMAP0);

		pDrawer->calcDrawCode(); // Restore original call
	}

	kmCall(0x8030EF28, setSpinDriverPathCustomColor);

	/*
	* The Green Launch Star is coded to load a model from SuperSpinDriverEmpty.arc. This was used for the transparent model
	* in SMG1 to mark its position before all green stars are collected. However, we have no use of this property in SMG2,
	* so we can safely disable this here. This also improves memory usage since this model would be loaded at all times.
	*/
	kmWrite32(0x8031E2A4, 0x60000000); // NOP call to initEmptyModel.
	}
}