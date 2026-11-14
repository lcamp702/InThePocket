/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the note.c file. Contains function definitions for the functions in note.h.

	model/note.c
*/

#include "note.h"

/* ==== Function: note_init ====
PURPOSE: Initializes note object

INPUT: Note object pointer, UINT32 time (time when note should appear on screen), UINT pitch value (from 1-4)
	neighbour(Note *): Pointer to the next note, if it is played at the same time. Otherwise 0.
OUTPUT: N/A
*/
void note_init(Note *note, UINT32 time, UINT8 pitch, UINT8 duration, Note *neighbour)
{
	note->time = time;
	note->pitch = pitch;
	note->duration = duration;
	note->neighbour = neighbour;
	note->hit = 0;
}

/* ==== Function: note_is_hit ====
PURPOSE: Check wether the game time and key input are correct to hit the note passed

INPUT: Note object pointer, UINT32 game_clock (time of hit), UINT pitch value (from 1-4)

OUTPUT: (bool) Note is hit?
*/
bool note_is_hit(Note *note, UINT32 game_clock, UINT32 last_interval, UINT8 pitch)
{
	/* Check if note can be hit based on time */
	UINT32 hit_time = (note->time + TRAVEL_TICKS);

	UINT8 can_hit = game_clock >= hit_time; /* Time is greater than start hit time */
	can_hit &= (game_clock - last_interval) < (hit_time + HIT_WINDOW); /* Time is less than end hit time */

	if (can_hit && pitch == note->pitch) 
	{ /* Right key, right time, not already hit. Now this note is hit */
		note->hit = 1;
		return true;
	}
	else if ((UINT32)(note->neighbour)) /* If neighbour pointer is not 0, check neighbour. */
		return note_is_hit(note->neighbour, game_clock, last_interval, pitch);
	else 
		return false;
}