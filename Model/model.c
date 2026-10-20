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

/*------ Function: model_init ------
PURPOSE: Initializes Model.  Assumes that notes are ordered by spawn time. Songs over 512 notes are truncated.

INPUT: Takes Model, the song array, and the amount of notes as parameters.

OUTPUT: N/A
*/
void model_init(Model * model, const Note* arr, UINT16 notes_amt){
	
	UINT32 i;
	
	model->note_count = (notes_amt > MAX_NOTES) ? MAX_NOTES : notes_amt;
	
	for(i=0;i<model->note_count; i++){
		note_init(&model->notes[i], arr[i].pitch, arr[i].time);
	}

	model->head=model->game_clock=0;
	meter_init(&model->meter);
}

/*------ Function: model_tick ------
PURPOSE: Increments the models clock. Note these ticks are 1/70th of a second. 

INPUT: pointer to Model

OUTPUT: N/A

NOTE : MOVE NOTES FIRST THEN TICK. 
*/
void model_tick(Model* model){
	
	model->game_clock++;

}
/*------ Function: model_last_on_screen ------
PURPOSE: Calculates the index of the last note on screen. This gives the bounds for the array which is functional. Assumes the array is non empty.

INPUT: pointer to Model

OUTPUT: Returns index of the last note which is on screen (Unsigned Integer, length word).

BUG: TECHNICALLY IT IS RETURNING THE FIRST NOTE OUTSIDE THE SCREEN. HOWEVER I HAVE ALREADY WRITTEN MOST OF THE CODEBASE AND CHANGING THE LOGIC SEEMS ARDOUS.
*/
UINT16 model_last_on_screen(const Model* model){
	
	UINT16 last_note=model->head;
	
	while(last_note<model->note_count && model->notes[last_note].time <= model->game_clock){ /*loop runs until time from note is strictly LARGER than game_clock*/
		last_note++;
	}
	
	return last_note;
}



/*------ Function: model_move_notes ------
PURPOSE: Moves notes where applicable

INPUT: pointer to Model

OUTPUT: N/A

NOTE : MOVE NOTES FIRST THEN TICK. 

*/
void model_move_notes(Model* model){
	UINT16 last_note, i;
	
	last_note = model_last_on_screen(model);
	
	for(i=model->head;i<last_note && i<model->note_count; i++){
		note_move(&model->notes[i]);
	}

}

/*------ Function: model_advance_head ------
PURPOSE: Advances the model head. 

INPUT: pointer to Model

OUTPUT: N/A
*/
void model_advance_head(Model* model){
	if(model->head<model->note_count){
		model->head++;
	}
}