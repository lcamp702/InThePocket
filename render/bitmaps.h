/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Declarations of the sprite bitmaps used by the renderer. Bitmaps are defined in
	bitmaps.c, not here, so no space is reserved by including this header.

    bitmaps.h*/

#ifndef BITMAPS_H
#define BITMAPS_H

#include "../types.h"

#define NOTE8_HEIGHT  14
#define NOTE16_HEIGHT 28
#define NOTE16_HEAD_ROW 24   /* row of note16_bmp at the centre of the notehead */

extern const UINT8  note8_bmp[NOTE8_HEIGHT];
extern const UINT16 note16_bmp[NOTE16_HEIGHT];

#endif
