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

/* ==== Function: synch_clock_tick ====
PURPOSE: Updates model clock based on the realtime clock value.

INPUT: Pointer to Model,
	The current time from the system clock (UINT32)

OUTPUT: N/A
*/
void synch_clock_tick(Model *model, UINT32 time_now)
{
	/* Sync game clock based on real time */
	UINT32 time = time_now - model->start_time;
	model->last_interval = time - model->game_clock;
	model->game_clock = time;

	model_clock_tick();
}
