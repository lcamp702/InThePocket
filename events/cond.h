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

/* ==== Function: cond_check_misses ====
PURPOSE: Checks to see if index points at a missed note, and if so updates the meter and repeats until head points at an "alive" note

INPUT: pointer to Model

OUTPUT: N/A
*/
void cond_check_misses(Model *model);

/* ==== Function: cond_game_over ====
PURPOSE: Checks if game is over (enjoyment meter has gone to 0, quit key input flag was set)

INPUT: pointer to Model

OUTPUT: N/A
*/
UINT8 cond_game_over(const Model *model);

/* ==== Function: cond_is_hit ====
PURPOSE: Checks if the note at the offset or its neighbour can be hit.

INPUT: Pointer to model
		Pitch of keypress
		Head offset to check

OUTPUT: Is hit? -1: No 0-3: Head offset of note hit
*/
UINT8 cond_is_hit(Model *model, UINT8 pitch, UINT8 head_offset);

/* ==== Function: cond_apply_hit ====
PURPOSE: Handles hit event.

INPUT: pointer to Model, offset of note from head

OUTPUT: N/A
*/
void cond_apply_hit(Model *model, UINT8 head_offset);

/* ==== Function: cond_apply_miss ====
PURPOSE: Handles miss event.

INPUT: pointer to Model.

OUTPUT: N/A
*/
void cond_apply_miss(Model *model);

/* ==== Function: cond_check_key_buffer ====
PURPOSE: checks key buffer and triggers appropriate events

INPUT: Pointer to game model

OUTPUT: N/A
*/
void cond_check_key_buffer(Model *model);


#endif