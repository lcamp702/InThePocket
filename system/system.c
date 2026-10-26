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

void get_time() 
{
    long *timer = (long *) 0x462;
    long time;
    long exit = Super(0);	/* Enter priviledged mode. */
    time = *timer;		/* Read timer value*/
    Super(exit);		/* Exit priviledged mode */
    return time;
}