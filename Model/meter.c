/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the meter.c file. Contains function definitions for the functions in meter.h.
    meter.c*/

#include "meter.h"

/*initializes meter*/
void meter_init(Meter *meter){
	
}

/* update enjoyment meter*/
void meter_update_enjoyment(Meter *meter, INT8 delta){
	meter->enjoyment+=delta;
}

/* Add's score to meter */
void meter_update_score(Meter *meter, UINT32 points){
	UINT32 delta;
	delta=points;
	delta*=meter->multiplier;
	meter->score+=delta;
}      

/* Increases the multiplier */
void meter_increase_multiplier(Meter *meter){
	meter->multiplier=2;
};              

/* multiplier goes to 1 */
void meter_reset_multiplier(Meter *meter){
	meter->multiplier=1;
}              