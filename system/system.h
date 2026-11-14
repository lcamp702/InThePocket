/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 3
	Due: Nov 13, 2026

	Header file for the system.c file. Contains function prototypes for the functions in system.c.
	These functions will eventually be replaced when TOS is thrown out in favour of a custom ROM.

	system/system.h
*/

#include "../types.h"

/* ==== Function: get_pbase ====

PURPOSE: Get the physical base address of the current screen buffer

INPUT: N/A

OUTPUT: (UINT32 *) Pointer to the screen buffer base
*/
UINT32 *get_pbase();

/* ==== Function: buff_256 ====

PURPOSE: Get the 256 byte aligned base address of the second framebuffer.

INPUT: N/A

OUTPUT: (UINT32 *) Pointer to the second framebuffer base
*/
UINT32 *buff_256();

/* ==== Function: set_screen ====

PURPOSE: Set a new physical base address for the front frame buffer

INPUT: base (UINT32 *) Pointer to the new front buffer base

OUTPUT: N/A
*/
void set_screen(UINT32 *base);

/* ==== Function: disable_cursor ====

PURPOSE: Prevent the OS cursor flash on the main framebuffer

INPUT: N/A

OUTPUT: N/A
*/
void disable_cursor();

/* ==== Function: get_time ====

PURPOSE: Get the current time value from the VBL clock

INPUT: N/A

OUTPUT: (UINT32) Current time
*/
UINT32 get_time();