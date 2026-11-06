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

/* Check for waiting inputs in the input buffer */
UINT8 has_input()
{
	return Cconis();
}

/* Read the last input from the input buffer */
UINT8 get_input()
{
	return Cnecin();
}