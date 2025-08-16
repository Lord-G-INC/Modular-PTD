#include "ModuleData_ExtTextColors.h"

asm void CustomTagProcessorGetNewColor() {
	lis r7, cTextColorTable@ha
	addi r7, r7, cTextColorTable@l
	mr r5, r7
}

kmCall(0x80469F08, CustomTagProcessorGetNewColor);
kmWrite32(0x80469F10, 0x60000000);

kmCall(0x804664F8, CustomTagProcessorGetNewColor);
kmWrite32(0x80466504, 0x60000000);

kmWrite32(0x80469EC4, 0x2C1E0000+cTextColorTableColors); // Last Color (Exclusive)
kmWrite32(0x80469ECC, 0x3BDE0000+(cTextColorTableColors-1)); // Offset? Last Color (Inclusive)