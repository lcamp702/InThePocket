/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	TOS System calls. These functions will eventually be replaced when TOS is 
    thrown out in favour of a custom ROM.

    system/system.c
*/

#include <osbind.h>
#include "system.h"

UINT32 get_time() 
{
    UINT32 *timer = (UINT32 *) 0x462;
    UINT32 time;
    UINT32 exit = Super(0);	/* Enter priviledged mode. */
    time = *timer;		/* Read timer value*/
    Super(exit);		/* Exit priviledged mode */
    return time;
}

UINT8 waiting_input() 
{
	return Cconis();
}

UINT8 get_input()
{
	return Cnecin();
}