#include "ModuleData_ExtTextColors.h"

asm void CustomTagProcessorGetNewColor() {
	lis r7, cTextColorTable@ha
	addi r7, r7, cTextColorTable@l
	mr r5, r7
}

kmCall(0x80469F08, CustomTagProcessorGetNewColor);
kmWrite32(0x80469F10, PPC_NOP);

kmCall(0x804664F8, CustomTagProcessorGetNewColor);
kmWrite32(0x80466504, PPC_NOP);

kmWrite32(0x80469EC4, PPC_CMPWI(30, cTextColorTableColors)); // Last Color (Exclusive)
kmWrite32(0x80469ECC, PPC_ADDI(30, 30, cTextColorTableColors-1)); // Offset? Last Color (Inclusive)