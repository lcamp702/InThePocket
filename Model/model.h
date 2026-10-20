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
	
    model.h*/

#ifndef MODEL_H
#define MODEL_H

#include "types.h"
#include "note.h"
#include "meter.h"

#define MAX_NOTES 512

typedef struct Model{
	Note notes[MAX_NOTES];
	Meter meter;
	UINT16 note_count;
	UINT32 game_clock;
	UINT16 head;
} Model;

void model_init(Model * model, const Note* arr, UINT16 notes_amt);
void model_tick(Model* model);
UINT16 model_last_on_screen(const Model* model);
void model_move_notes(Model* model);
void model_advance_head(Model* model);

#endif