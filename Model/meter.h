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
	
    meter.h*/

#ifndef METER_H
#define METER_H

#include "types.h"

typedef struct Meter{
	INT8 enjoyment; /*the "health bar" of the game. Falling to 0 */
	UINT32 score;
	INT8 multiplier;
} Meter; 

#endif