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

#define TRAVEL_TICKS 592  /* On screen ticks before note needs to be hit */
#define HIT_WINDOW 32	 /* Total window for button presses */

/*
==== Note ====
	UINT32 time (Time when note should appear on screen)
	UINT8 pitch	(Track number for note to run along)
	UINT8 duration (Icon number to be be drawn)
	UINT8 neighbour (Is the next neighbour played at the same time?)
*/
typedef struct Note
{
	UINT32 time; /* Time when note should be hit */
	UINT8 pitch;
	UINT8 duration;
	UINT8 neighbour;
	UINT8 hit;	/* 0 - Not hit; 1 - Already hit*/
} Note;

/* ==== Function: note_init ====
PURPOSE: Initializes note object

INPUT: Note object pointer, UINT32 time (time when note should appear on screen), UINT pitch value (from 1-4)
	neighbour(UINT8) Boolean: Is the next neighbour of this note played at the same time? (For multiple notes at the same time)
OUTPUT: N/A
*/
void note_init(Note *note, UINT32 time, UINT8 pitch, UINT8 duration, UINT8 neighbour);

#endif