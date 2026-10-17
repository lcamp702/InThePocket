/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the asynch.c file. Contains function definitions for the functions in asynch.h.
    asynch.c*/

#include "asynch.h"
#include "cond.h"

#define HIT_REWARD_ENJOYMENT 2
#define HIT_REWARD_SCORE 10

/*------ Function: asynch_key_to_pitch ------
PURPOSE: Converts char value from input ket to corresponding int value.

INPUT: Char value of keyboard press

OUTPUT: UINT8 of corresponding key press to the "pitch" of the note.
*/
UINT8 asynch_key_to_pitch(char key){
	
	UINT8 pitch=4; /*sets pitch default be a value functions as a "no press" value.*/
	
	if(key=='a' || key=='A'){
		pitch=KEY_A;
	}
	else if(key=='s'|| key=='S'){
		pitch=KEY_S;
	}
	else if(key=='d'|| key=='D'){
		pitch=KEY_D;
	}
	else if(key=='f'|| key=='F'){
		pitch=KEY_F;
	}
	
	return pitch;
}

/*------ Function: asynch_is_hit ------
PURPOSE: Checks if note has been hit

INPUT: pitch value (UINT8) and model address

OUTPUT: N/A
*/
UINT8 asynch_is_hit(const Model *model, UINT8 pitch){
	
	UINT8 is_hit;

	if(model->head<model->note_count && model->notes[model->head].pitch==pitch && model->game_clock>=(model->notes[model->head].time+TRAVEL_TICKS)){ /*checks to see if note is in the timing window for hitting*/
		is_hit=1;
	}
	else{
		is_hit=0;
	}

	return is_hit;
}

/*------ Function: asynch_apply_hit ------
PURPOSE: Handles hit event.

INPUT: Model address

OUTPUT: N/A
*/
void asynch_apply_hit(Model *model){

	meter_update_enjoyment(&model->meter, HIT_REWARD_ENJOYMENT); /*adds to enjoyment meter*/
	meter_update_score(&model->meter, HIT_REWARD_SCORE); /*updates score*/
	meter_double_multiplier(&model->meter); /*double multiplier*/
	model_advance_head(model); /*advance head*/

}

/*------ Function: asynch_apply_miss ------
PURPOSE: Handles miss event.

INPUT: Model address.

OUTPUT: N/A
*/
void asynch_apply_miss(Model *model){ 

		meter_update_enjoyment(&model->meter, -MISS_PENALTY); /*applies miss penalty*/	
		meter_reset_multiplier(&model->meter); /*reset the score multiplier to 1*/
		model_advance_head(model); /*advance head*/

	}

