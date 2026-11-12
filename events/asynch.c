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
PURPOSE: Converts char value from input key to corresponding int value.

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

	case '`':
	case '~':
		pitch = -2;
		break;

	default:
		pitch = -1;
		break;
	}

	return pitch;
}

/* ==== Function: asynch_key_hit ====
PURPOSE: checks to see if input buffer must be set and then sets it

INPUT: Key pressed (char) and pointer to model

OUTPUT: N/A	
*/
void asynch_key_hit(Model *model, char key)
{
	UINT8 pitch;

	pitch = asynch_key_to_pitch(key);

	if (pitch == (UINT8)-2)
	{ /* Handle quit key */
		model->quit_request_buff = 1;
		return;
	}

	if (pitch == (UINT8)-1)
		return; /* Guard against bad input */

	if (pitch < TARGET_COUNT)
		model->key_press_buff[pitch] = 1; /* sets the input buffer so that the game state can be updated on the next clock tick. */

}
