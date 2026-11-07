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
	bitmaps.c/bitmaps.s, not here, so no space is reserved by including this header.

	bitmaps/bitmaps.h
*/

#ifndef BITMAPS_H
#define BITMAPS_H

#include "../types.h"

/*
===============  Backgrounds  ===============
640px wide bitmaps to plot in their entirety.
*/
#define BG_HEIGHT 272

extern const UINT32 bg[BG_HEIGHT];

/*
===============  Doug  ===============
Sets of 3 bitmaps to animate the character on screen.
*/
#define DOUG_HEIGHT 145
#define DOUG_COUNT 5

extern const UINT32 doug0[DOUG_HEIGHT * 3];
extern const UINT32 doug1[DOUG_HEIGHT * 3];
extern const UINT32 doug2[DOUG_HEIGHT * 3];
extern const UINT32 doug3[DOUG_HEIGHT * 3];
extern const UINT32 doug4[DOUG_HEIGHT * 3];

/*
=============== ENJOYMENT METER ===============
A saxophone in the top right that is filled
according to the enjoyment meter level.
*/
#define METER_HEIGHT 64

extern const UINT32 sax_empty[METER_HEIGHT];
extern const UINT32 sax_full[METER_HEIGHT];

/*
===============  NOTE HEADS  ===============
Each note head is a 16px tall shape.
The widest form is 32px wide,
narrower shapes are center aligned.
*/
#define NOTE_HEAD_HEIGHT 16 /* Determines line spacing and target size. */

extern const UINT32 note_whole_head[NOTE_HEAD_HEIGHT];
extern const UINT32 note_half_head[NOTE_HEAD_HEIGHT];
extern const UINT32 note_black_head[NOTE_HEAD_HEIGHT];

/*
===============   TARGETS   ===============
Targets are the same height as note heads
so they fit between the lines on the staff.
*/
extern const UINT16 target_off_bmp[NOTE_HEAD_HEIGHT];
extern const UINT16 target_on_bmp[NOTE_HEAD_HEIGHT];

/*
===============  Multiplier Bubbles  ===============
8px circle bitmaps to show multiplier level up progress
*/
#define MULTI_HEIGHT 8

extern const UINT8 multi_on[MULTI_HEIGHT];
extern const UINT8 multi_off[MULTI_HEIGHT];

#endif
