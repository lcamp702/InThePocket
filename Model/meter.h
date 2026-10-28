/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Header file for the meter.c file. Contains function prototypes for the functions in meter.c.

	model/meter.h
*/

#ifndef METER_H
#define METER_H

#include "../types.h"

#define ENJOYMENT_MAX 64
#define ENJOYMENT_START 32 /* open spec value */
#define MISS_PENALTY_ENJOYMENT 5
#define HIT_REWARD_ENJOYMENT 2
#define HIT_REWARD_SCORE 10

typedef struct Meter
{
	INT8 enjoyment; /*the "health bar" of the game. Falling to 0 causes game to end. */
	UINT32 score;
	INT8 multiplier;
	INT8 multi_progress;
} Meter;

void meter_init(Meter *meter);						   /*initializes meter*/
void meter_update_enjoyment(Meter *meter, INT8 delta); /* Updates change enjoyment */
void meter_update_score(Meter *meter, UINT32 points);  /* Add's score to meter */
void meter_increase_multiplier(Meter *meter);		   /* Increases the multiplier */
void meter_reset_multiplier(Meter *meter);			   /* multiplier goes to 1 */

#endif