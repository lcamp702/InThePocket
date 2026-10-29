/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the synch.c file. Contains function definitions for the functions in synch.h.
	synch.c
*/

#include "synch.h"
/*Synch clock should advance clock, move notes, and move head past moved notes. Parameters is the model. Assumes the model has been initialized.*/

/*------ Function: synch_clock_tick ------
PURPOSE: Handler function to call model_move_notes, updates model clock. Designed to handle and call all "timed" base events.

INPUT: Pointer to Model,
	The current time from the system clock (UINT32)

OUTPUT: N/A
*/
void synch_clock_tick(Model *model, UINT32 time_now)
{
	model_time_sync(model, time_now); /* Update model clock */
}