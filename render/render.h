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

#include "../types.h"
#include "../model/model.h"

void render_level(Model *model, UINT32 *base);

/*------ Function: render ------
PURPOSE: Master render function. Calls the other render functions, and renders the game world onto the screen.

INPUT: model(Model *): Pointer to Model, 
       base(UINT32 *): Pointer to frame buffer
       
OUTPUT: N/A
*/
void render(Model *model, UINT32 *base);

/*------ Function: render_note ------
PURPOSE: Renders a single note to screen. It's place in the staff is determined by its pitch.

INPUT: note(Note *): Pointer to Note
       base(UINT32 *): Pointer to frame buffer

OUTPUT: N/A
*/
void render_note(Note *note, UINT32 *base);

/*------ Function: render_meter ------
PURPOSE: Renders meter. Fills it up based on enjoyment.

INPUT: base(UINT32 *): Pointer to frame buffer
       meter(Meter *): pointer to meter

OUTPUT: N/A
*/
void render_meter(Meter *meter, UINT32 *base);

/*------ Function: render_staff ------
PURPOSE: Renders one four line staff. 

INPUT: base(UINT32 *): Pointer to frame buffer

OUTPUT: N/A
*/
void render_staff(UINT32 *base);

/*------ Function: render_targets ------
PURPOSE: Renders one target. 

INPUT: base(UINT32 *): Pointer to frame buffer
       targets(UINT8 *): Pointer to targets struct

OUTPUT: N/A
*/
void render_targets(UINT32 *base, UINT8 * targets);

/*------ Function: render_score ------
PURPOSE: Renders the score. 

INPUT: base(UINT32 *): Pointer to frame buffer
       position(row,col): Integer row and column pixel position 
       score(UINT32): Score value (to be printed)

OUTPUT: N/A
*/
void render_score(UINT32 *base, int row, int col, UINT32 score);

#endif