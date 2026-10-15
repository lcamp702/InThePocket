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

#define MISS_PENALTY 20
#define HIT_REWARD 2

void cond_check_misses(Model *model){
	while(model->head<model->note_count && model->notes[model->head].time+TRAVEL_TICKS+HIT_WINDOW<model->game_clock && model->meter.enjoyment > 0 ){
		meter_update_enjoyment(&model->meter, -MISS_PENALTY);
		meter_reset_multiplier(&model->meter);
		model_advance_head(model);
	}
}