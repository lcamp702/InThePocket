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

typedef struct Model
{
	Note notes[MAX_NOTES];
	UINT8 targets[TARGET_COUNT];
	Meter meter;

	UINT16 note_count;
	UINT16 head; /* First note in play */

	UINT8 render_bg;	/* Bool: should render background */

	/* ==== Clock ====*/
	UINT32 game_clock;	/* Current game time */
	UINT32 start_time;	/* Clock time at level start */
	UINT32 last_interval;	/* Clock ticks since last frame. */
} Model;

/*------ Function: model_init ------
PURPOSE: Initializes Model.  Assumes that notes are ordered by spawn time. Songs over 512 notes are truncated.

INPUT: Takes Model, the song array, and the amount of notes as parameters.

OUTPUT: N/A
*/
void model_init(Model * model, const Note* arr, UINT16 notes_amt, UINT32 time_now);

/*------ Function: model_time_sync ------
PURPOSE: Increments the models clock. Note these ticks are 1/70th of a second.

INPUT: pointer to Model

OUTPUT: N/A

NOTE : MOVE NOTES FIRST THEN TICK.
*/
void model_time_sync(Model* model, UINT32 time_now);

/*------ Function: model_next_disp_note ------
PURPOSE: Calculates the index of the last note on screen. This gives the bounds for the array which is functional. Assumes the array is non empty.

INPUT: pointer to Model

OUTPUT: Returns index of the last note which is on screen (Unsigned Integer, length word).

BUG: TECHNICALLY IT IS RETURNING THE FIRST NOTE OUTSIDE THE SCREEN. HOWEVER I HAVE ALREADY WRITTEN MOST OF THE CODEBASE AND CHANGING THE LOGIC SEEMS ARDOUS.
*/
UINT16 model_next_disp_note(const Model* model);

/*------ Function: model_advance_head ------
PURPOSE: Advances the model head.

INPUT: pointer to Model

OUTPUT: N/A
*/
void model_advance_head(Model* model);

#endif