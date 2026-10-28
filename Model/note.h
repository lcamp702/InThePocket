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

#define KEY_A 0
#define KEY_S 1
#define KEY_D 2
#define KEY_F 3
#define NOTE_START_X 640   /* right edge, ST high-res; adjust to res */
#define NOTE_SPEED 1   /* pixels per tick: open spec value */

#define TRAVEL_TICKS 600 /* On screen ticks before note needs to be hit */
#define HIT_WINDOW 16 /* Total window for button presses */

typedef struct Note {
	UINT32 time; /*time of note beginning to appear on screen*/
	INT16 x; /*x position of note*/
	UINT8 pitch;
	UINT8 duration;
} Note;

void note_init(Note *note, UINT32 time, UINT8 pitch, UINT8 duration);	/* sets x = NOTE_START_X */
void note_move(Note *note, int speed);	/* x -= speed */

#endif