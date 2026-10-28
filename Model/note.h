/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Header file for the note.c file. Contains function prototypes for the functions in note.c.

	model/note.h
*/

#ifndef NOTE_H
#define NOTE_H

#include "../types.h"

#define KEY_A 3
#define KEY_S 2
#define KEY_D 1
#define KEY_F 0

#define TRAVEL_TICKS 600 /* On screen ticks before note needs to be hit */
#define HIT_WINDOW 16	 /* Total window for button presses */

typedef struct Note
{
	UINT32 time; /* Time to draw note on screen */
	UINT8 pitch;
	UINT8 duration;
} Note;

void note_init(Note *note, UINT32 time, UINT8 pitch, UINT8 duration); /* sets x = NOTE_START_X */

#endif