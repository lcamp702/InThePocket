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

#define TRAVEL_TICKS 568  /* On screen ticks before note needs to be hit */
#define HIT_WINDOW 32	 /* Total window for button presses */

/*
==== Note ====
	UINT32 time (Time when note should appear on screen)
	UINT8 pitch	(Track number for note to run along)
	UINT8 duration (Icon number to be be drawn)
	Note *neighbour (Pointer to the next note, if it is played at the same time. Otherwise 0.)
*/
typedef struct Note
{
	UINT32 time; /* Time when note should be hit */
	UINT8 pitch;
	UINT8 duration;
	struct Note *neighbour;
	UINT8 hit;	/* 0 - Not hit; 1 - Already hit*/
} Note;

/* ==== Function: note_init ====
PURPOSE: Initializes note object

INPUT: Note object pointer, UINT32 time (time when note should appear on screen), UINT pitch value (from 1-4)
	neighbour(Note *): Pointer to the next note, if it is played at the same time. Otherwise 0.
OUTPUT: N/A
*/
void note_init(Note *note, UINT32 time, UINT8 pitch, UINT8 duration, Note *neighbour);

/* ==== Function: note_is_hit ====
PURPOSE: Check wether the game time and key input are correct to hit the note passed

INPUT: Note object pointer, UINT32 game_clock (time of hit), UINT pitch value (from 1-4)

OUTPUT: N/A
*/
bool note_is_hit(Note *note, UINT32 game_clock, UINT32 last_interval, UINT8 pitch);

#endif