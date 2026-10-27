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
	
	model/meter.c
*/

#include "meter.h"

/*initializes meter*/
/*------ Function: meter_init ------
PURPOSE: Initializes Meter struct

INPUT: Meter*, a pointer to a Meter struct

OUTPUT: N/A
*/
void meter_init(Meter *meter){
	meter->enjoyment=ENJOYMENT_START;
	meter->score=0;
	meter->multiplier=1;
}

/* update enjoyment meter NB POSITIVE DELTA MAX 2 AND WILL NEVER OVERFLOW METER SINCE 100+2=102<127*/
/*------ Function: meter_update_enjoyment ------
PURPOSE: Updates the enjoyment meter

INPUT: Meter*, a pointer to a Meter struct and the amount to change it by in the form of a byte sized signed integer

OUTPUT: N/A
*/
void meter_update_enjoyment(Meter *meter, INT8 delta){
	if(meter->enjoyment>0){ /*this check ensures that enjoyment never changes once it goes non-positive */
		meter->enjoyment+=delta; /*increases/decreases the enjoyment meter */
	}	 
	if(meter->enjoyment>ENJOYMENT_MAX) meter->enjoyment=ENJOYMENT_MAX; /*ensures upper bound for enjoyment*/
}

/*------ Function: meter_update_score ------
PURPOSE: Add's score to meter 

INPUT: Meter*, a pointer to a Meter struct, and points to be added in the form of a 32-bit unsigned Integer

OUTPUT: N/A
*/
void meter_update_score(Meter *meter, UINT32 points){
	meter->score += points * meter->multiplier;
}      

/*------ Function: meter_increase_multiplier ------
PURPOSE: Doubles the mutiplier

INPUT: pointer to Meter

OUTPUT: N/A
*/
void meter_increase_multiplier(Meter *meter){
	if(meter->multiplier<4) meter->multiplier++; /*increase score multiplier*/
}              

/* multiplier goes to 1 */
/*------ Function: meter_reset_multiplier ------
PURPOSE: Resets Multiplier to 1

INPUT: pointer to Meter

OUTPUT: N/A
*/
void meter_reset_multiplier(Meter *meter){
	meter->multiplier=1;
}              