/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Header file for the cond.c file. Contains function prototypes for the functions in cond.c.

	cond.h
*/

#ifndef COND_H
#define COND_H

#include "../model/model.h"

/*------ Function: cond_check_misses ------
PURPOSE: Checks to see if index points at a missed note, and if so updates the meter and repeats until head points at an "alive" note

INPUT: pointer to Model

OUTPUT: N/A
*/
void cond_check_misses(Model *model);

/*------ Function: cond_game_over ------
PURPOSE: Checks if game is over (enjoyment meter has gone to 0)

INPUT: pointer to Model

OUTPUT: N/A
*/
UINT8 cond_game_over(const Model *model);

#endif