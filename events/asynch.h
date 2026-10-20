/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Header file for the asynch.c file. Contains function prototypes for the functions in asynch.c.
	
    asynch.h*/

#ifndef ASYNCH_H
#define ASYNCH_H

#include "../model/model.h"

UINT8 asynch_key_to_pitch(char key);
UINT8 asynch_is_hit(Model *model, UINT8 pitch);
void asynch_apply_hit(Model *model);
void asynch_apply_miss(Model *model);

#endif