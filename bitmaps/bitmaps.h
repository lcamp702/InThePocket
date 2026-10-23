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

    bitmaps/bitmaps.h
*/

#ifndef BITMAPS_H
#define BITMAPS_H

#include "../types.h"

#define METER_HEIGHT 64
#define NOTE_HEAD_HEIGHT 16	/* Determines line spacing and target size. */

/*
===============   TARGETS   ===============
Targets are the same height as note heads
so they fit between the lines on the staff.
*/
extern const UINT16 target_off_bmp[NOTE_HEAD_HEIGHT]; 
extern const UINT16 target_on_bmp[NOTE_HEAD_HEIGHT];

/*
=============== SCORE METER ===============
A saxophone in the top right that is filled
according to the enjoyment meter level.
*/
extern const UINT32 meter_left[METER_HEIGHT];
extern const UINT32 meter_right[METER_HEIGHT];

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
