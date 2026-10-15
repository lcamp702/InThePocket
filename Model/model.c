/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the model.c file. Contains function definitions for the functions in model.h.
    model.c*/

#include "model.h"

void model_init(Model * model, const Note* arr, UINT16 notes_amt){
	
	UINT32 i;
	
	model->note_count = (notes_amt > MAX_NOTES) ? MAX_NOTES : notes_amt;
	
	for(i=0;i<model->note_count; i++){
		note_init(&model->notes[i], arr[i].pitch, arr[i].time);
	}

	model->head=model->game_clock=0;
	meter_init(&model->meter);
}

void model_tick(Model* model){
	
	model->game_clock++;

}

UINT16 model_notes_on_screen(const Model* model){
	
	UINT16 last_note=model->head;
	
	while(last_note<model->note_count && model->notes[last_note].time <= model->game_clock){
		last_note++;
	}
	
	return last_note;
}
void model_move_notes(Model* model){
	UINT16 last_note, i;
	
	last_note = model_notes_on_screen(model);
	
	for(i=model->head;i<last_note && i<model->note_count; i++){
		note_move(&model->notes[i]);
	}

}
void model_advance_head(Model* model){
	if(model->head<model->note_count){
		model->head++;
	}
}