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

typedef struct Model{
	Note notes[MAX_NOTES];
	UINT8 targets[TARGET_COUNT];
	Meter meter;
	UINT16 note_count;
	UINT32 game_clock;
	UINT32 start_time;
	UINT16 head;
} Model;

void model_init(Model * model, const Note* arr, UINT16 notes_amt, UINT32 time_now);
void model_time_sync(Model* model, UINT32 time_now);
UINT16 model_last_on_screen(const Model* model);
void model_move_notes(Model* model);
void model_advance_head(Model* model);

#endif