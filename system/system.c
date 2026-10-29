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
#include <stdio.h>
#include "system.h"

/* Return the memory address of the framebuffer */
UINT32 *get_pbase()
{
	return (UINT32 *)Physbase();
}

UINT32 *malloc_256(int bytes)
{
	UINT32 *mem = (UINT32 *)Malloc(bytes+256);
	int i;

	/* 256 bit align */
	mem = (UINT32 *)(((UINT32)mem + 255) & ~255);

	/* Clear memory */
	for (i = 0; i < (bytes / 4); i++)
		/* Clear all longs */
		*(mem + i) = 0;

	return mem;
}

void free_buff(UINT32 *ptr)
{
	Mfree(ptr);
}

void set_screen(UINT32 *base)
{
	Setscreen(-1, base, -1);
	Vsync();
}

void disable_cursor()
{
	printf("\033f");
	fflush(stdout);
}

/* Get the current time from the system VBlank timer */
UINT32 get_time()
{
	UINT32 *timer = (UINT32 *)0x462;
	UINT32 time;
	UINT32 exit = Super(0); /* Enter priviledged mode. */
	time = *timer;			/* Read timer value*/
	Super(exit);			/* Exit priviledged mode */
	return time;
}

/* Check for waiting inputs in the input buffer */
UINT8 waiting_input()
{
	return Cconis();
}

/* Read the last input from the input buffer */
UINT8 get_input()
{
	return Cnecin();
}