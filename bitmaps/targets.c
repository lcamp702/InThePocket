/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Definitions of the target and multiplier progress sprite bitmaps used by the renderer. 
	Each array holds one entry per pixel row; the *_HEIGHT constants in bitmaps.h give 
	the row counts.

	bitmaps/targets.c
*/

#include "bitmaps.h"

/* target not pressed: hollow circle */
const UINT16 target_off_bmp[NOTE_HEAD_HEIGHT] = {
	0x07E0, 0x1818, 0x2004, 0x4002,
	0x4002, 0x8001, 0x8001, 0x8001,
	0x8001, 0x8001, 0x8001, 0x4002,
	0x4002, 0x2004, 0x1818, 0x07E0};

/* target pressed: filled circle */
const UINT16 target_on_bmp[NOTE_HEAD_HEIGHT] = {
	0x07E0, 0x1FF8, 0x3FFC, 0x7FFE,
	0x7FFE, 0xFFFF, 0xFFFF, 0xFFFF,
	0xFFFF, 0xFFFF, 0xFFFF, 0x7FFE,
	0x7FFE, 0x3FFC, 0x1FF8, 0x07E0};

/* Multi progress off */
const UINT8 multi_off[MULTI_HEIGHT] = {
	0x3C, 0x42, 0x81, 0x81,
	0x81, 0x81, 0x42, 0x3C};

/* Multi progress on */
const UINT8 multi_on[MULTI_HEIGHT] = {
	0x3C, 0x7E, 0xFF, 0xFF,
	0xFF, 0xFF, 0x7E, 0x3C};