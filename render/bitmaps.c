/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Definitions of the sprite bitmaps used by the renderer. Each array holds one
	entry per pixel row; the *_HEIGHT constants in bitmaps.h give the row counts.

    bitmaps.c*/

#include "bitmaps.h"

const UINT8 note8_bmp[NOTE8_HEIGHT] = {
    0x08, 0x08, 0x0E, 0x0B, 0x09, 0x09, 0x08,
    0x08, 0x08, 0x78, 0xF8, 0xF8, 0xF8, 0x70
};

const UINT16 note16_bmp[NOTE16_HEIGHT] = {
    0x0040, 0x00C0, 0x00C0, 0x00E0,
    0x00E0, 0x00F0, 0x00F8, 0x00CE,
    0x00C6, 0x00C3, 0x00C3, 0x00C3,
    0x00C3, 0x00C3, 0x00C2, 0x00C2,
    0x00C4, 0x00C4, 0x00C0, 0x00C0,
    0x00C0, 0x00C0, 0x0FC0, 0x3FC0,
    0x7FC0, 0xFF00, 0xFE00, 0x7C00
};

/* target not pressed: hollow circle */
const UINT16 target_off_bmp[TARGET_HEIGHT] = {
    0x07E0, 0x1818, 0x2004, 0x4002,
    0x4002, 0x8001, 0x8001, 0x8001,
    0x8001, 0x8001, 0x8001, 0x4002,
    0x4002, 0x2004, 0x1818, 0x07E0
};

/* target pressed: filled circle */
const UINT16 target_on_bmp[TARGET_HEIGHT] = {
    0x07E0, 0x1FF8, 0x3FFC, 0x7FFE,
    0x7FFE, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0x7FFE,
    0x7FFE, 0x3FFC, 0x1FF8, 0x07E0
};


/* 
===============  NOTE HEADS  ===============
Each note head is a 16px tall shape. 
The widest form is 32px wide,
narrower shapes are center aligned.
*/

const UINT32 note_head_whole[NOTE_HEAD_HEIGHT] = {
    0X001FF800,
    0X01FFFF80,
    0X0FE0FFF0,
    0X1F801FF8,
    0X3F0007FC,
    0X7F0003FE,
    0XFF0001FF,
    0XFF0001FF,
    0XFF8000FF,
    0XFF8000FF,
    0X7FC000FE,
    0X3FF000FC,
    0X1FF801F8,
    0X0FFF07F0,
    0X01FFFF80,
    0X001FF800
};

const UINT32 note_head_half[NOTE_HEAD_HEIGHT] = {
    0X0001FC00,
    0X000FFF80,
    0X003FFFC0,
    0X007FC1C0,
    0X00FE00E0,
    0X01F800E0,
    0X03F001E0,
    0X03C001E0,
    0X078003C0,
    0X07800FC0,
    0X07001F80,
    0X07007F00,
    0X0383FE00,
    0X03FFFC00,
    0X01FFF000,
    0X003F8000
};

const UINT32 note_head_black[NOTE_HEAD_HEIGHT] = {
    0X0001FC00,
    0X000FFF80,
    0X003FFFC0,
    0X007FFFC0,
    0X00FFFFE0,
    0X01FFFFE0,
    0X03FFFFE0,
    0X03FFFFE0,
    0X07FFFFC0,
    0X07FFFFC0,
    0X07FFFF80,
    0X07FFFF00,
    0X03FFFE00,
    0X03FFFC00,
    0X01FFF000,
    0X003F8000
};