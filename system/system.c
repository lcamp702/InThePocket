/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 3
	Due: Nov 9, 2026

	TOS System display and timer calls. These functions will eventually be replaced when TOS is
	thrown out in favour of a custom ROM.

	system/system.c
*/

#include <osbind.h>
#include <stdio.h> /* For disabling cursor, hopefully there is a better way. */
#include "system.h"

static UINT32 buffer[8064]; /* Extra 64 longs for 256 byte alignment */

/* Return the memory address of the framebuffer */
UINT32 *get_pbase()
{
	return (UINT32 *)Physbase();
}

UINT32 *buff_256()
{
	return (UINT32 *)(((UINT32)buffer + 255) & ~255); /* 256 byte align */

}

void set_screen(UINT32 *base)
{
	Setscreen((void *)-1, (void *)base, (INT16)-1);
}

void disable_cursor()
{
	printf("\033f");
	fflush(stdout);
}

/* Get the current time from the system VBlank timer */
UINT32 get_time()
{
    UINT32 time;
	UINT32 *timer = (UINT32 *)0x462; /* Address of timer value */
	long exit = Super(0); /* Enter priviledged mode. */
	time = *timer;			/* Read timer value*/
	Super(exit);			/* Exit priviledged mode */
	return time;
}