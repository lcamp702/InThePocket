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
#define CONSEC_MULTI 10	/* Number of hits in a row to increase multiplier */

typedef struct Meter
{
	INT8 enjoyment; /*the "health bar" of the game. Falling to 0 causes game to end. */
	UINT32 score;
	INT8 multiplier;
	INT8 multi_progress;
	INT8 dirty;	/* A flag for whether or not the enjoyment changed this clock */
	INT8 dirty_sco;	/* A flag for whether or not the score changed this clock */
} Meter;

/* ==== Function: meter_init ====
PURPOSE: Initializes Meter struct

INPUT: Meter*, a pointer to a Meter struct

OUTPUT: N/A
*/
void meter_init(Meter *meter);

/* ==== Function: meter_apply_hit ====
PURPOSE: Update all meters and stats to reflect a hit

INPUT: Meter*, a pointer to a Meter struct to update

OUTPUT: N/A
*/
void meter_apply_hit(Meter *meter);

/* ==== Function: meter_apply_miss ====
PURPOSE: Update all meters and stats to reflect a miss

INPUT: Meter*, a pointer to a Meter struct to update

OUTPUT: N/A
*/
void meter_apply_miss(Meter *meter);

/* ==== Function: meter_update_enjoyment ====
PURPOSE: Updates the enjoyment meter

INPUT: Meter*, a pointer to a Meter struct and the amount to change it by in the 
form of a byte sized signed integer

OUTPUT: N/A

NOTE: Assumes ENJOYMENT_MAX + delta < 127
*/
void meter_update_enjoyment(Meter *meter, INT8 delta);

/* ==== Function: meter_update_score ====
PURPOSE: Adds score to meter

INPUT: Meter*, a pointer to a Meter struct, and the number of points to be added 
in the form of a 32-bit unsigned Integer

OUTPUT: N/A
*/
void meter_update_score(Meter *meter, UINT32 points);

/* ==== Function: meter_increase_multiplier ====
PURPOSE: Increases the multiplier progress and the multiplier, if applicable

INPUT: pointer to Meter

OUTPUT: N/A
*/
void meter_increase_multiplier(Meter *meter);

/* ==== Function: meter_reset_multiplier ====
PURPOSE: Resets Multiplier to 1, resets multiplier progress to 0

INPUT: pointer to Meter

OUTPUT: N/A
*/
void meter_reset_multiplier(Meter *meter);

#endif