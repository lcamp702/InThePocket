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

/* sets x = NOTE_START_X */
/*------ Function: note_init ------
PURPOSE: Initializes note object

INPUT: Note object pointer, UINT pitch value (from 1-4), and UINT32 time object (time when note will begin on screen)

OUTPUT: N/A
*/
void note_init(Note *note, UINT32 time, UINT8 pitch, UINT8 duration)
{
	note->time = time;
	note->pitch = pitch;
	note->duration = duration;
}