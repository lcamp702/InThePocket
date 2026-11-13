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

	asynch.h
*/

#ifndef ASYNCH_H
#define ASYNCH_H

#include "../model/model.h"

/* ==== Function: asynch_key_to_pitch ====
PURPOSE: Converts char value from input key to corresponding int value.

INPUT: Char value of keyboard press

OUTPUT: UINT8 of corresponding key press to the "pitch" of the note.
*/
UINT8 asynch_key_to_pitch(char key);

/* ==== Function: asynch_key_hit ====
PURPOSE: checks to see if input buffer must be set and then sets it

INPUT: Key pressed (char) and pointer to model

OUTPUT: N/A	
*/
void asynch_key_hit(Model *model, char key);

#endif