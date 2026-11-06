/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the asynch.c file. Contains function definitions for the functions in asynch.h.
	asynch.c
*/

#include "asynch.h"

/* ==== Function: asynch_key_to_pitch ====
PURPOSE: Converts char value from input ket to corresponding int value.

INPUT: Char value of keyboard press

OUTPUT: UINT8 of corresponding key press to the "pitch" of the note.
*/
UINT8 asynch_key_to_pitch(char key)
{
	UINT8 pitch;
	switch (key)
	{
	case 'a':
	case 'A':
		pitch = KEY_A;
		break;

	case 's':
	case 'S':
		pitch = KEY_S;
		break;

	case 'd':
	case 'D':
		pitch = KEY_D;
		break;
	case 'f':
	case 'F':
		pitch = KEY_F;
		break;

	default:
		pitch = -1;
		break;
	}

	return pitch;
}

/* ==== Function: asynch_is_hit ====
PURPOSE: Checks if the note at the offset or its neighbour can be hit.

INPUT: Pointer to model
		Pitch of keypress
		Head offset to check

OUTPUT: Is hit? -1: No 0-3: Head offset of note hit
*/
UINT8 asynch_is_hit(Model *model, UINT8 pitch, UINT8 head_offset)
{
	Note note = model->notes[model->head + head_offset];

	/* Check if note can be hit based on time */
	UINT32 hit_time = (note.time + TRAVEL_TICKS);

	UINT8 can_hit = model->game_clock >= hit_time;
	can_hit &= model->game_clock < (hit_time + HIT_WINDOW);

	if (can_hit && note.pitch == pitch)
		return head_offset;
	else if (model->notes[model->head + head_offset].neighbour)
		return asynch_is_hit(model, pitch, head_offset+1);
	else
		return -1;
}

/* ==== Function: asynch_key_hit ====
PURPOSE: Checks if note has been hit by this key press

INPUT: Key pressed (char) and pointer to model

OUTPUT: Is hit? -1: No 0-3: Head offset of note hit
*/
UINT8 asynch_key_hit(Model *model, char key)
{
	UINT8 pitch;

	if (model->head > model->note_count)	 /* Check if song is playing  */
		return -1;

	pitch = asynch_key_to_pitch(key);

	if (pitch < TARGET_COUNT)
		model->targets[pitch] = 1; /* Enable the target for this pitch. */

	return asynch_is_hit(model, pitch, 0);
}

/* ==== Function: asynch_apply_hit ====
PURPOSE: Handles hit event.

INPUT: pointer to Model

OUTPUT: N/A
*/
void asynch_apply_hit(Model *model, UINT8 head_offset)
{
	meter_update_enjoyment(&model->meter, HIT_REWARD_ENJOYMENT); /*adds to enjoyment meter*/
	meter_update_score(&model->meter, HIT_REWARD_SCORE);		 /*updates score*/
	meter_increase_multiplier(&model->meter);					 /*double multiplier*/
	if (head_offset == 0)
		model_advance_head(model);								/*advance head*/
	else
		model->notes[model->head + head_offset].hit = 1;
}

/* ==== Function: asynch_apply_miss ====
PURPOSE: Handles miss event.

INPUT: pointer to Model.

OUTPUT: N/A
*/
void asynch_apply_miss(Model *model)
{
	meter_update_enjoyment(&model->meter, -MISS_PENALTY_ENJOYMENT); /*applies miss penalty*/
	meter_reset_multiplier(&model->meter);							/*reset the score multiplier to 1*/
}
