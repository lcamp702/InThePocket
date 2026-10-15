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
	
    note.h*/

#ifndef NOTE_H
#define NOTE_H

#include "types.h"

#define KEY_A 0
#define KEY_S 1
#define KEY_D 2
#define KEY_F 3

typedef struct Note {
	UINT8 pitch;
	UINT32 time; /*time of note beginning to appear on screen*/
	UINT32 x; /*x position of note*/
} Note;

#endif