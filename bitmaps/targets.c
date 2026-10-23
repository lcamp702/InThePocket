/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Definitions of the target sprite bitmaps used by the renderer. Each array holds one
	entry per pixel row; the *_HEIGHT constants in bitmaps.h give the row counts.

    bitmaps/targets.c
*/

#include "bitmaps.h"

/* target not pressed: hollow circle */
const UINT16 target_off_bmp[NOTE_HEAD_HEIGHT] = {
    0x07E0, 0x1818, 0x2004, 0x4002,
    0x4002, 0x8001, 0x8001, 0x8001,
    0x8001, 0x8001, 0x8001, 0x4002,
    0x4002, 0x2004, 0x1818, 0x07E0
};

/* target pressed: filled circle */
const UINT16 target_on_bmp[NOTE_HEAD_HEIGHT] = {
    0x07E0, 0x1FF8, 0x3FFC, 0x7FFE,
    0x7FFE, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0x7FFE,
    0x7FFE, 0x3FFC, 0x1FF8, 0x07E0
};