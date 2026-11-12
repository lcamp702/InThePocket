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

	render/render.h
*/

#ifndef RENDER_H
#define RENDER_H

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400

#include "../types.h"
#include "../model/model.h"

/* ==== Function: render ====
PURPOSE: Master render function. Calls the other render functions, and renders the game world onto the screen.

INPUT: base(UINT32 *): Pointer to frame buffer,
	   model(Model *): Pointer to Model

OUTPUT: N/A
*/
void render(UINT32 *base, Model *model);

/* ==== Function: render_staff ====
PURPOSE: Renders one four line staff.

INPUT: base(UINT32 *): Pointer to frame buffer

OUTPUT: N/A
*/
void render_staff(UINT32 *base);

/* ==== Function: clean_staff ====

PURPOSE: Clear all the pixels between the lines on the staff and slightly above/below.

INPUT: base(UINT32 *): Pointer to frame buffer

OUTPUT: N/A
*/
void clean_staff(UINT32 *base);

/* ==== Function: render_note ====
PURPOSE: Renders a single note to screen. It's place in the staff is determined by its pitch.

INPUT: base(UINT32 *): Pointer to frame buffer
	   note(Note *): Pointer to note


OUTPUT: N/A
*/
void render_note(UINT32 *base, Note *note, UINT32 time);

/* ==== Function: render_targets ====
PURPOSE: Renders one target.

INPUT: base(UINT32 *): Pointer to frame buffer
	   targets(UINT8 *): Pointer to targets struct

OUTPUT: N/A
*/
void render_targets(UINT32 *base, UINT8 *targets);

/* ==== Function: render_enjoyment ====
PURPOSE: Renders meter. Fills it up based on enjoyment.

INPUT: base(UINT32 *): Pointer to frame buffer
	   meter(Meter *): pointer to meter

OUTPUT: N/A
*/
void render_enjoyment(UINT32 *base, INT8 enjoyment);

/* ==== Function: render_score ====
PURPOSE: Renders the score.

INPUT: base(UINT32 *): Pointer to frame buffer
	   position(row,col): Integer row and column pixel position
	   score(UINT32): Score value (to be printed)

OUTPUT: N/A
*/
void render_score(UINT32 *base, int row, int col, UINT32 score);

/* ==== Function: render_multiplier ====
PURPOSE: Renders the multiplier number (X1) and multiplier progress.

INPUT: base(UINT32 *): Pointer to frame buffer
	   position(row,col): Integer row and column pixel position
	   multi(UINT8): Multiplier value (to be printed)
	   progress(UINT8): Number of current consecutive hits

OUTPUT: N/A
*/
void render_multiplier(UINT32 *base, int row, int col, UINT8 multi, UINT8 progress);

/* ==== Function: render_end_screen ====
PURPOSE: Renders an end screen with large text, a smaller subline, and the score of the game.

INPUT: base(UINT32 *): Pointer to frame buffer
	   t1(char *): Pointer to large text string
	   t1_len(UINT16): Length of large text string
	   t2(char *): Pointer to subtext string
	   t2_len(UINT16): Length of subtext string
	   score(UINT32): Score to render at the bottom of the screen
OUTPUT: N/A
*/
void render_end_screen(UINT32 *base, char *t1, UINT16 t1_len, char *t2, UINT16 t2_len, UINT32 score);

/* ==== Function: render_int_to_string ====
PURPOSE: Converts an int value to a string and writes it to the given char array

INPUT: str(char *): Pointer to string of length places + 1
		value(int): Integer to write to the string
	   places(int): Number of decimal digits to print

OUTPUT: Places actually printed
*/
int render_int_to_string(char *str, UINT32 value, int places);

#endif