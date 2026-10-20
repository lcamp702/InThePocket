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
	
    cond.h*/

#ifndef COND_H
#define COND_H

#include "../model/model.h"

#define TRAVEL_TICKS 140
#define HIT_WINDOW 5

void cond_check_misses(Model *model);
UINT8 cond_is_game_over(const Model *model);
UINT8 cond_is_song_over(const Model *model);

#endif