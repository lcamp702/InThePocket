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

/*Initializes Model. Takes Model, the song array, and the amount of notes as parameters. Assumes that notes are ordered by spawn time. Songs over 512 notes are truncated.*/
void model_init(Model * model, const Note* arr, UINT16 notes_amt){
	
	UINT32 i;
	
	model->note_count = (notes_amt > MAX_NOTES) ? MAX_NOTES : notes_amt;
	
	for(i=0;i<model->note_count; i++){
		note_init(&model->notes[i], arr[i].pitch, arr[i].time);
	}

	model->head=model->game_clock=0;
	meter_init(&model->meter);
}

/*Increments the models clock. Note these ticks are 1/70th of a second. Parameters are the model.
*NOTE : MOVE NOTES FIRST THEN TICK. 
*/
void model_tick(Model* model){
	
	model->game_clock++;

}
/*Calculates the index of the last note on screen. This gives the bounds for the array which is functional. Assumes the array is non empty. Paramaters are the model itself. Does bounds checking.
*
*/
UINT16 model_last_on_screen(const Model* model){
	
	UINT16 last_note=model->head;
	
	while(last_note<model->note_count && model->notes[last_note].time <= model->game_clock){
		last_note++;
	}
	
	return last_note;
}

/*Moves notes where applicable. Parameters are the model. Does bounds checking. 
*NOTE : MOVE NOTES FIRST THEN TICK. 
*/
void model_move_notes(Model* model){
	UINT16 last_note, i;
	
	last_note = model_last_on_screen(model);
	
	for(i=model->head;i<last_note && i<model->note_count; i++){
		note_move(&model->notes[i]);
	}

}
/*Advances the model head. Parameter is the model itself. Does bounds checking. */
void model_advance_head(Model* model){
	if(model->head<model->note_count){
		model->head++;
	}
}