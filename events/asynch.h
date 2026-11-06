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
PURPOSE: Converts char value from input ket to corresponding int value.

INPUT: Char value of keyboard press

OUTPUT: UINT8 of corresponding key press to the "pitch" of the note.
*/
UINT8 asynch_key_to_pitch(char key);

/* ==== Function: asynch_is_hit ====
PURPOSE: Checks if the note at the offset or its neighbour can be hit.

INPUT: Pointer to model
		Pitch of keypress
		Head offset to check

OUTPUT: Is hit? -1: No 0-3: Head offset of note hit
*/
UINT8 asynch_is_hit(Model *model, UINT8 pitch, UINT8 head_offset);

/* ==== Function: asynch_key_hit ====
PURPOSE: Checks if note has been hit

INPUT: Key pressed (char) and pointer to model

OUTPUT: Is hit? (Boolean 1:0)
*/
UINT8 asynch_key_hit(Model *model, char key);

/* ==== Function: asynch_apply_hit ====
PURPOSE: Handles hit event.

INPUT: pointer to Model, offset of note from head

OUTPUT: N/A
*/
void asynch_apply_hit(Model *model, UINT8 head_offset);

/* ==== Function: asynch_apply_miss ====
PURPOSE: Handles miss event.

INPUT: pointer to Model.

OUTPUT: N/A
*/
void asynch_apply_miss(Model *model);

#endif