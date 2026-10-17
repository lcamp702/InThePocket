/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the cond.c file. Contains function definitions for the functions in cond.h.
    cond.c*/

#include "cond.h"


/*------ Function: cond_check_misses ------
PURPOSE: Checks to see if index points at a missed note, and if so updates the meter and repeats until head points at an "alive" note 

INPUT: Model addres

OUTPUT: N/A
*/
void cond_check_misses(Model *model){
	while(model->head<model->note_count && model->notes[model->head].time+TRAVEL_TICKS+HIT_WINDOW<model->game_clock && model->meter.enjoyment > 0 ){ /*checks that the head is pointed at a note that has passed the hit window*/
		meter_update_enjoyment(&model->meter, -MISS_PENALTY); /*applies miss penalty*/
		meter_reset_multiplier(&model->meter); /*sends score multiplier back to 1*/
		model_advance_head(model); /*advances index*/
	}
}

/*------ Function: cond_is_game_over ------
PURPOSE: Checks if game is over (enjoyment meter has gone to 0)

INPUT: Model address

OUTPUT: N/A
*/
UINT8 cond_is_game_over(const Model *model){
	UINT8 game_over; /*true/false value to be returned*/

	if(model->meter.enjoyment<=0) { /*checks if enjoyment meter is below or equal to 0*/
		game_over = 1; /*sets game over to true*/
	}
	else{ /*game over meter is greater than 0 (player hasn't lost yet)*/
		game_over=0; /*sets game over to false*/
	}
	
	return game_over;
}

/*------ Function: cond_is_song_over ------
PURPOSE: Checks if song is over

INPUT: Model address

OUTPUT: N/A
*/
UINT8 cond_is_song_over(const Model *model){
	UINT8 song_over; /*true/false value to be returned*/
	if(model->head>=model->note_count){ /*checks if head is out of bounds of note count*/
		song_over=1; /*if it is sets return value to 1*/
	}
	else{
		song_over=0; /*else it sets return value to 0*/
	}

	return song_over; /*return song over*/
}