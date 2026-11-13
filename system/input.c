/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 3
	Due: Nov 9, 2026

	System keyboard input calls. These functions will eventually be replaced when TOS is
	thrown out in favour of a custom ROM.

	system/input.c
*/

#include <osbind.h>
#include "input.h"

/* ==== Function: has_input ====

PURPOSE: Check wether an input is waiting in the buffer.

INPUT: N/A

OUTPUT: (bool) Is there an input waiting in the buffer?
*/
bool has_input()
{
	return (bool)Cconis();
}

/* ==== Function: get_input ====

PURPOSE: Read an input from the input buffer.

INPUT: N/A

OUTPUT: (UINT8) ASCII input code from buffer.
*/
UINT8 get_input()
{
	return (UINT8)(Cnecin() & 0x000000FF);
}