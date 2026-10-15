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
    synch.c*/

#include "synch.h"
#include "cond.h"
/*Synch clock should advance clock, move notes, and move head past moved notes. Parameters is the model. Assumes the model has been initialized.*/
void synch_clock_tick(Model* model){
	model_move_notes(model);
	model_tick(model);
	cond_check_misses(model);
}