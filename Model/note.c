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
    note.c*/

#include "note.h"

/* sets x = NOTE_START_X */
void note_init(Note *note, UINT8 pitch, UINT32 time) {
	note->pitch=pitch;
	note->time=time;
	note->x=NOTE_START_X;
}

/* Moves note according to note speed */
void note_move(Note *note){
	note->x-=NOTE_SPEED;
}                            