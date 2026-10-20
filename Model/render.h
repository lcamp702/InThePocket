/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Header file for the render.c file. Contains function prototypes for the functions in render.c.
	
    render.h*/

#ifndef RENDER_H
#define RENDER_H

#include "types.h"
#include "Model.h"

void render(Model *model, UINT32 *base);
void render_note(Note *note, UINT32 *base);
void render_meter(Meter *meter, UINT32 *base);
void render_staff(UINT32 *base);
void render_targets(UINT32 *base);
void render_score(UINT32 *base, int row, int col, UINT32 score);

#endif