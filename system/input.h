/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 3
	Due: Nov 13, 2026

	Header file for the input.c file. Contains function prototypes for the functions in input.c.
	These functions will eventually be replaced when TOS is thrown out in favour of a custom ROM.

	system/input.h
*/

#include "../types.h"

/* ==== Function: has_input ====

PURPOSE: Check wether an input is waiting in the buffer.

INPUT: N/A

OUTPUT: (bool) Is there an input waiting in the buffer?
*/
bool has_input();

/* ==== Function: get_input ====

PURPOSE: Read an input from the input buffer.

INPUT: N/A

OUTPUT: (UINT8) ASCII input code from buffer.
*/
UINT8 get_input();