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

#define NOTE_HEAD_HEIGHT 16
#define NOTE8_HEIGHT  14
#define NOTE16_HEIGHT 28
#define TARGET_HEIGHT 16
#define NOTE16_HEAD_ROW 27   /* row of note16_bmp at the centre of the notehead */

extern const UINT8  note8_bmp[NOTE8_HEIGHT];
extern const UINT16 note16_bmp[NOTE16_HEIGHT];
extern const UINT16 target_off_bmp[TARGET_HEIGHT];
extern const UINT16 target_on_bmp[TARGET_HEIGHT];

/* 
===============  NOTE HEADS  ===============
Each note head is a 16px tall shape. 
The widest form is 32px wide,
narrower shapes are center aligned.
*/
extern const UINT32 note_head_whole[NOTE_HEAD_HEIGHT];
extern const UINT32 note_head_half[NOTE_HEAD_HEIGHT];
extern const UINT32 note_head_black[NOTE_HEAD_HEIGHT];

#endif
