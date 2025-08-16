#pragma once

#include "syati.h"

static GXColor cTextColorTable[] = {
	{0xFF, 0xFF, 0xFF, 0xFF},
{{PrimaryColorTable}}
{{SecondaryColorTable}}
};

const size_t cTextColorTableTotal = sizeof(cTextColorTable)/sizeof(cTextColorTable[0]);
const size_t cTextColorTableColors = (cTextColorTableTotal-1)/2;