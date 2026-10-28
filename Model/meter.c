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

#define MAX_MULTI 4
#define CONSEC_MULTI 10

/*initializes meter*/
/*------ Function: meter_init ------
PURPOSE: Initializes Meter struct

INPUT: Meter*, a pointer to a Meter struct

OUTPUT: N/A
*/
void meter_init(Meter *meter)
{
	meter->enjoyment = ENJOYMENT_START;
	meter->score = 0;
	meter->multiplier = 1;
	meter->multi_progress = 0;
}

/* update enjoyment meter NB POSITIVE DELTA MAX 2 AND WILL NEVER OVERFLOW METER SINCE 100+2=102<127*/
/*------ Function: meter_update_enjoyment ------
PURPOSE: Updates the enjoyment meter

INPUT: Meter*, a pointer to a Meter struct and the amount to change it by in the form of a byte sized signed integer

OUTPUT: N/A
*/
void meter_update_enjoyment(Meter *meter, INT8 delta)
{
	if (meter->enjoyment >= 0)
		/* Stop updating enjoyment once it hits/passes 0 */
		meter->enjoyment += delta;

	if (meter->enjoyment > ENJOYMENT_MAX)
		/* Clip enjoyment to max value */
		meter->enjoyment = ENJOYMENT_MAX;
}

/*------ Function: meter_update_score ------
PURPOSE: Add's score to meter

INPUT: Meter*, a pointer to a Meter struct, and points to be added in the form of a 32-bit unsigned Integer

OUTPUT: N/A
*/
void meter_update_score(Meter *meter, UINT32 points)
{
	meter->score += points * meter->multiplier;
}

/*------ Function: meter_increase_multiplier ------
PURPOSE: Increases the multiplier progress and the multiplier, if applicable

INPUT: pointer to Meter

OUTPUT: N/A
*/
void meter_increase_multiplier(Meter *meter)
{
	if (meter->multiplier < MAX_MULTI)
		/* Don't track progress if multiplier can't increase */
		if (++meter->multi_progress >= CONSEC_MULTI) /* Increase multi progress */
		{											 /* 10 consecutive notes, increase multiplier */
			meter->multiplier++;
			/* Reset multi increase progress */
			meter->multi_progress = 0;
		}
}

/* multiplier goes to 1 */
/*------ Function: meter_reset_multiplier ------
PURPOSE: Resets Multiplier to 1, resets multiplier progress to 0

INPUT: pointer to Meter

OUTPUT: N/A
*/
void meter_reset_multiplier(Meter *meter)
{
	/* Multi minimum is 1x (Gotta get points somehow) */
	meter->multiplier = 1;
	/* Reset progress (consecutive streak broken) */
	meter->multi_progress = 0;
}