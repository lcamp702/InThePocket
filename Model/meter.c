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
/*------ Function: meter_init ------
PURPOSE: Initializes Meter struct

INPUT: Meter*, an address to a Meter struct

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

INPUT: Meter*, an address to a Meter struct and the amount to change it by in the form of a byte sized signed integer

OUTPUT: N/A
*/
void meter_update_enjoyment(Meter *meter, INT8 delta){
	meter->enjoyment+=delta;
	if(meter->enjoyment>ENJOYMENT_MAX) meter->enjoyment=ENJOYMENT_MAX;
}

/* */
/*------ Function: meter_update_score ------
PURPOSE: Add's score to meter 

INPUT: Meter*, an address to a Meter struct, and points to be added in the form of a 32-bit unsigned Integer

OUTPUT: N/A
*/
void meter_update_score(Meter *meter, UINT32 points){
	UINT32 delta;
	delta=points;
	delta*=meter->multiplier;
	meter->score+=delta;
}      

/* Increases the multiplier */
/*------ Function: meter_double_multiplier ------
PURPOSE: Doubles the mutiplier

INPUT: Meter address

OUTPUT: N/A
*/
void meter_double_multiplier(Meter *meter){
	if(meter->multiplier<16) meter->multiplier*=2; /*doubles score multiplier*/
}              

/* multiplier goes to 1 */
/*------ Function: meter_reset_multiplier ------
PURPOSE: Resets Multiplier to 1

INPUT: Meter Address

OUTPUT: N/A
*/
void meter_reset_multiplier(Meter *meter){
	meter->multiplier=1;
}              