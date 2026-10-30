/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Header file for the system.c file. Contains function prototypes for the functions in system.c.
	These functions will eventually be replaced when TOS is thrown out in favour of a custom ROM.

	system/system.h
*/
#include "../types.h"

/* Framebuffer tools */
UINT32 *get_pbase();
UINT32 *buff_256();
void set_screen(UINT32 *base);

void disable_cursor();

/* Clock tools */
UINT32 get_time();

/* Input tools */
UINT8 waiting_input();
UINT8 get_input();