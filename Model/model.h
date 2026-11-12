/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Header file for the model.c file. Contains function prototypes for the functions in model.c.

	model/model.h
*/

#ifndef MODEL_H
#define MODEL_H

#include "../types.h"
#include "note.h"
#include "meter.h"

#define MAX_NOTES 512
#define TARGET_COUNT 4

/*
==== Model ====
Stores general game information and objects

== Objects ==
Note notes[MAX_NOTES];	(Song array of note objects)
UINT8 targets[TARGET_COUNT];  (Array of target states)
Meter meter;	(Object of scores and enjoyment meter)
UINT8 key_press_buff[TARGET_COUNT]; (array of keyboard inputs)
UINT8 quit_request_buff; (quit request inputs)

UINT16 note_count; (Total number of notes in the song)
UINT16 head;	(First note in on the screen)

UINT8 new_bg;	(Flag for changing backgrounds)

== Clock (70Hz) ==
UINT32 game_clock;	(Ticks since game start)
UINT32 start_time;	(Clock ticks at game start)
UINT32 last_interval; (Ticks passed since last model update)
*/
typedef struct Model
{
	Note notes[MAX_NOTES];	/*Song array of note objects*/
	UINT8 targets[TARGET_COUNT];	/*Array of target states*/
	UINT8 key_press_buff[TARGET_COUNT]; /*buffers a keyboard input until the next clock tick*/
	UINT8 quit_request_buff; /*buffers a quit key input until the next clock tick*/
	Meter meter;	/*Object of scores and enjoyment meter*/

	UINT16 note_count;	/*(Total number of notes in the song*/
	UINT16 head;	/*First note in on the screen*/

	UINT8 new_bg;	/*Flag for changing backgrounds*/

	/* ==== Clock ====*/
	UINT32 game_clock;	/*Ticks since game start*/
	UINT32 start_time;	/*Clock ticks at game start*/
	UINT32 last_interval;	/*Ticks passed since last model update*/
} Model;

/* ==== Function: model_init ====
PURPOSE: Initializes Model.  Assumes that notes are ordered by spawn time. Songs over 512 notes are truncated.

INPUT: Takes Model, the song array, and the amount of notes as parameters.

OUTPUT: N/A
*/
void model_init(Model *model, const Note *arr, UINT16 notes_amt, UINT32 time_now);

/* ==== Function: model_next_disp_note ====
PURPOSE: Calculates the index of the last note on screen. This gives the bounds for the array which is functional. Assumes the array is non empty.

INPUT: pointer to Model

OUTPUT: Returns index of the last note which is on screen (Unsigned Integer, length word).
*/
UINT16 model_next_disp_note(const Model *model);

/* ==== Function: model_advance_head ====
PURPOSE: Advances the model head.

INPUT: pointer to Model

OUTPUT: N/A
*/
void model_advance_head(Model *model);

/* ==== Function: model_clear_key_buff ====
PURPOSE: clears keyboard input buffer.

INPUT: pointer to Model

OUTPUT: N/A
*/

void model_clear_key_buff(Model *model);

#endif