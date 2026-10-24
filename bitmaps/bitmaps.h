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

/*
=============== SCORE METER ===============
A saxophone in the top right that is filled
according to the enjoyment meter level.
*/
#define METER_HEIGHT 64

extern const UINT32 emp_left[METER_HEIGHT];
extern const UINT32 emp_right[METER_HEIGHT];
extern const UINT32 full_left[METER_HEIGHT];
extern const UINT32 full_right[METER_HEIGHT];

/* 
===============  NOTE HEADS  ===============
Each note head is a 16px tall shape. 
The widest form is 32px wide,
narrower shapes are center aligned.
*/
#define NOTE_HEAD_HEIGHT 16	/* Determines line spacing and target size. */

extern const UINT32 note_head_whole[NOTE_HEAD_HEIGHT];
extern const UINT32 note_head_half[NOTE_HEAD_HEIGHT];
extern const UINT32 note_head_black[NOTE_HEAD_HEIGHT];

/*
===============   TARGETS   ===============
Targets are the same height as note heads
so they fit between the lines on the staff.
*/
extern const UINT16 target_off_bmp[NOTE_HEAD_HEIGHT]; 
extern const UINT16 target_on_bmp[NOTE_HEAD_HEIGHT];

/*
===============  Backgrounds  ===============
640px wide bitmaps to plot in their entirety.
*/
#define BG_HEIGHT 272

extern const UINT32 bg[BG_HEIGHT];

#endif
