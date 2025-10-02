#pragma once

#include "syati.h"

#include "ModuleData_SpinDriverPathDrawInit_Ext.h"

/*
* Originally by Aurum
* Further Extended by Evanbowl and Lord Giganticus
* Remastered for Syati Modules by Super Hackio
*/

namespace MR {
	NameObj* createSuperSpinDriverGreen(const char* pName) {
		return new SuperSpinDriver(pName, 1);
	}
}

namespace {
	void init_SuperSpinDriver_Ext(SuperSpinDriver* pActor, const JMapInfoIter& rIter, const TPos3f* pPos, BaseMatrixFollowValidater* pValidator) {
		MR::addBaseMatrixFollowTarget(pActor, rIter, pPos, pValidator); // Vanilla code line. Was easier to get rIter this way

		// The new system uses ShapeModelNo as an ObjArg, since every other ObjArg is used already.
		s32 ShapeModelNo = -1;
		MR::getJMapInfoShapeIdWithInit(rIter, &ShapeModelNo);

		if (ShapeModelNo < 0) { // Use the vanilla colouring system (including Green Launch Stars as well)
			pActor->initColor();

			if (pActor->mColor == 1) {
				MR::startBtpAndSetFrameAndStop(pActor, "SuperSpinDriver", pActor->mColor);
				MR::startBrk(pActor, "Green");
				{ //SpinDriverPathDrawer::setColorGreen
					pActor->mSpinDriverPathDrawer->mColor = 0;
				}
			}
			return;
		}

		// Time to use the new colouring system!
		// ShapeModelNo determines the Colour to use
		if (MR::isExistBtp(pActor, "CustomColors"))
			MR::startBtpAndSetFrameAndStop(pActor, "CustomColors", ShapeModelNo);

		char buffer[8]; // Impossible to use all 8 bytes, as ShapeModelNo is a short value in the BCSV Data
		sprintf(buffer, "%d", ShapeModelNo);
		MR::startBrkIfExist(pActor, buffer);

		pActor->mColor = pActor->mSpinDriverPathDrawer->mColor = (ShapeModelNo | 0xF0000000); //Surely, nothing can go wrong here... :clueless:
	}
	kmWrite32(0x8031E294, 0x60000000); // nop
	kmCall(0x8031E29C, init_SuperSpinDriver_Ext);




	// Disables Empty launch stars from being created (and thus wasting memory), as they cannot be used in Vanilla.
	// GLE's empty launch star system overrules this automatically.
	// However, that system won't support the custom colors of this newer module, so the function just under this fixes that.
	kmWrite32(0x8031E2A4, 0x60000000); // nop
	void initEmptyModel_SuperSpinDriver_Ext(LiveActor* pActor, const char* pAnimName) // Defaults to SuperSpinDriverEmpty
	{
		register SuperSpinDriver* pSpinDriver;
		__asm {
			mr pSpinDriver, r30
		}
		if (pSpinDriver->mColor & 0xF0000000)
			pAnimName = "CustomColorsEmpty";
		MR::startBtpAndSetFrameAndStop(pActor, pAnimName, static_cast<f32>(pSpinDriver->mColor & 0x0FFFFFFF));
	}
	kmCall(0x8031E688, initEmptyModel_SuperSpinDriver_Ext);
	kmWrite32(0x8031E6B4, 0x60000000); // nop




	// Custom path colours!
	void setSpinDriverPathColorCustom()
	{
		register SpinDriverPathDrawer* pDrawer;
		__asm {
			mr pDrawer, r27 // yoink
		}

		s32 idx = pDrawer->mColor & 0x0FFFFFFF;
		JUTTexture* tex;
		SpinDriverPathDrawInit_Ext* pDrawInit_Ext = (SpinDriverPathDrawInit_Ext*)MR::getSceneObjHolder()->getObj(SCENE_OBJ_SPIN_DRIVER_PATH_DRAW_INIT);
		tex = pDrawInit_Ext->mTextures[idx];
		if (idx < 0 || idx >= pDrawInit_Ext->mTextureCount)
			tex = pDrawInit_Ext->mTexBlue; // Failsafe

		tex->load(GX_TEXMAP0);
	}
	kmCall(0x8030EF20, setSpinDriverPathColorCustom);






	// We need to overwrite the CTor of SpinDriverPathDrawInit
	// Easy enough. Just replace the vanilla call with a call to a custom creation function
	// It's only a little jank...
	extern "C"
	{
		void __kAutoMap_8030E1D0(SpinDriverPathDrawInit*); // SpinDriverPathDrawInit::SpinDriverPathDrawInit((void)) Should be region free
	}

	SpinDriverPathDrawInit_Ext* createSpinDriverPathDrawInit_Ext(SpinDriverPathDrawInit_Ext* self) {
		__kAutoMap_8030E1D0(self);

		ResourceHolder* pHolder = MR::createAndAddResourceHolder("SpinDriverPath.arc");
		s32 counter = -1;
		char Buffer[16];
		do
		{
			sprintf(Buffer, "%d.bti", ++counter);
		} while (pHolder->mFileInfoTable->isExistRes(Buffer) && counter <= 2000); // If you have more than 2000 colours, you need to rethink your mod

		self->mTextureCount = counter;
		self->mTextures = NULL;
		if (counter <= 0 || counter >= 2000)
			return self;

		self->mTextures = new JUTTexture*[counter];
		for (s32 i = 0; i < counter; i++) {
			sprintf(Buffer, "%d.bti", i);
			self->mTextures[i] = new JUTTexture(MR::loadTexFromArc("SpinDriverPath.arc", Buffer, 0), 0);
		}
		return self;
	}
	kmCall(0x8045A2AC, createSpinDriverPathDrawInit_Ext);



	// Dummy Display model colours
	// Now supports the vanilla colours as well as the custom ones.
	void CustomSpinDriverDisplayModelColor(DummyDisplayModel* pDisplayModel) {

		if (pDisplayModel->mColorFrame & 0xF0000000)
		{
			// Custom Colour
			s32 col = pDisplayModel->mColorFrame & 0x0FFFFFFF;
			if (MR::isExistBtp(pDisplayModel, "CustomColors"))
				MR::startBtpAndSetFrameAndStop(pDisplayModel, "CustomColors", col);
			char buffer[8];
			sprintf(buffer, "%d", col);
			MR::startBrkIfExist(pDisplayModel, buffer);
		}
		else
		{
			MR::startBtpAndSetFrameAndStop(pDisplayModel, "SuperSpinDriver", pDisplayModel->mColorFrame);
			if (pDisplayModel->mColorFrame == 0)
				MR::startBrk(pDisplayModel, "Yellow");
			else if (pDisplayModel->mColorFrame == 1)
				MR::startBrk(pDisplayModel, "Green");
			else if (pDisplayModel->mColorFrame == 2)
				MR::startBrk(pDisplayModel, "Pink");
		}

	}

	kmCall(0x802957B8, CustomSpinDriverDisplayModelColor);
}