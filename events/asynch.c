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

INPUT: pitch value (UINT8) and pointer to model

OUTPUT: Is hit? (Boolean 1:0)
*/
UINT8 asynch_is_hit(Model *model, char key){
	UINT8 is_hit;
	int song_playing, pitch_ok, can_hit;
	UINT8 pitch = asynch_key_to_pitch(key);

	model->targets[pitch]=1; /* Light up the target for this pitch. */
	
	song_playing = model->head<model->note_count; /* Check if song is playing  */
	pitch_ok = model->notes[model->head].pitch==pitch; /* Check if the pitch matches the next note */
	can_hit = model->game_clock>=(model->notes[model->head].time+TRAVEL_TICKS); /* Check if note can be hit based on time */

	return (UINT8) (song_playing && pitch_ok && can_hit);
}

/*------ Function: asynch_apply_hit ------
PURPOSE: Handles hit event.

INPUT: pointer to Model

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

INPUT: pointer to Model.

OUTPUT: N/A
*/
void asynch_apply_miss(Model *model){ 

		meter_update_enjoyment(&model->meter, -MISS_PENALTY); /*applies miss penalty*/	
		meter_reset_multiplier(&model->meter); /*reset the score multiplier to 1*/
		model_advance_head(model); /*advance head*/

	}

