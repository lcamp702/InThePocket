/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the render module. Contains function definitions for the functions in render.h.

	render.c*/

#include "render.h"
#include "../bitmaps/bitmaps.h"
#include "../raster/raster.h"

#define SCREEN_WIDTH		640

/* Staff Parameters */
#define STAFF_BOTTOM		300 /* Bottom pixel row of staff */
#define LINE_COUNT			5 /* 5 lines, 4 tracks between for notes*/
#define STAFF_LINE_WIDTH	2 /* Thickness of each staff line */

#define METER_ROW			16
#define METER_COL			560

#define SCORE_OFFSET		5
#define SCORE_MAX_LEN		20 /*this is almost certainly far too large but we can change it if needed*/

#define STEM_LENGTH			(int)((NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH)*3.5)
#define STEM_COL_R			26	/* Stem column offset */
#define STEM_COL_L			5
#define STEM_ROW			8
#define STEM_WIDTH			2

/*------ Function: render ------
PURPOSE: Master render function. Calls the other render functions, and renders the game world onto the screen.

INPUT: model(Model *): Pointer to Model,
	   base(UINT32 *): Pointer to frame buffer

OUTPUT: N/A
*/
void render(Model *model, UINT32 *base)
{
	int i;
	clear_screen(base);

	/* Title */
	plot_string((UINT8 *)base, 8, 268, "In the Pocket");

	render_staff(base); /*must render the staff first for it to appear in the background*/

	for (i = model->head; i < model_last_on_screen(model); i++)
	{ /*model_last_on_screen returns index of first note off screen. it needs to be renamed probably but it is what it is.*/
		render_note(&model->notes[i], base);
	}

	render_targets(base, model->targets);
	render_meter(&model->meter, base);
}

/*------ Function: render_note ------
PURPOSE: Renders a single note to screen. It's place in the staff is determined by its pitch.

INPUT: base(UINT32 *): Pointer to frame buffer
	   note(Note *): Pointer to note


OUTPUT: N/A
*/
void render_note(Note *note, UINT32 *base)
{
	/*
		TODO:

		Note flags
		Ligatures (Connected flags)
		Connecting stems for multiple notes
	*/

	int row;
	UINT32 *note_head;

	/*
	Note Heads (32bit Bitmaps)
	*/ 

	switch (note->duration)
	{
		case 0:
			note_head = note_head_whole;
			break;
		case 1:
			note_head = note_head_half;
			break;
		default:
			note_head = note_head_black;
			break;
	}

	row = STAFF_BOTTOM + 1 - ((note->pitch + 1) * (NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH));
	plot_32bit_bitmap(base, row, note->x, note_head, NOTE_HEAD_HEIGHT);

	/*
	Note Stems (Vertical lines)
	*/

	if (note->duration != 0) {
		int w;
		if (note->pitch > 1) /* Bottom 2 tracks */
			for (w = 0; w < STEM_WIDTH; w++)
				plot_vertical_line(base, row + STEM_ROW, note->x + STEM_COL_L + w, STEM_LENGTH);
		else
			for (w = 0; w < STEM_WIDTH; w++)
				plot_vertical_line(base, row + STEM_ROW - STEM_LENGTH, note->x + STEM_COL_R - w, STEM_LENGTH);
	}
}

/*------ Function: render_meter ------
PURPOSE: Renders meter. Fills it up based on enjoyment.

INPUT: base(UINT32 *): Pointer to frame buffer
	   meter(Meter *): pointer to meter

OUTPUT: N/A
*/
void render_meter(Meter *meter, UINT32 *base)
{
	int i;

	if (meter->enjoyment > 0)
	{
		plot_32bit_bitmap(base, METER_ROW + METER_HEIGHT - meter->enjoyment, METER_COL, meter_left + METER_HEIGHT - meter->enjoyment, meter->enjoyment);
		plot_32bit_bitmap(base, METER_ROW + METER_HEIGHT - meter->enjoyment, METER_COL + 32, meter_right + METER_HEIGHT - meter->enjoyment, meter->enjoyment);
	}

	render_score(base, METER_ROW + METER_HEIGHT + 8, METER_COL, meter->score);
}

/*------ Function: render_staff ------
PURPOSE: Renders one four line staff.

INPUT: base(UINT32 *): Pointer to frame buffer

OUTPUT: N/A
*/
void render_staff(UINT32 *base)
{
	int row, i, w;

	for (i = 0; i < LINE_COUNT; i++)
	{
		for (w = 0; w < STAFF_LINE_WIDTH; w++) 
		{
			row = STAFF_BOTTOM - 1 + w - i*(NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH);
			plot_horizontal_line(base, row, 0, SCREEN_WIDTH);
		}
	}
}

/*------ Function: render_targets ------
PURPOSE: Renders one target.

INPUT: base(UINT32 *): Pointer to frame buffer
	   targets(UINT8 *): Pointer to targets struct

OUTPUT: N/A
*/
void render_targets(UINT32 *base, UINT8 *targets)
{
	int row, i;
	for (i = 1; i < LINE_COUNT; i++) /* Draw one target for each space */
	{
		row = STAFF_BOTTOM + 1 - (i * (NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH));
		if (targets[i])
		{
			plot_16bit_bitmap((UINT16 *)base, row, 40, target_on_bmp, NOTE_HEAD_HEIGHT);
		}
		else
		{
			plot_16bit_bitmap((UINT16 *)base, row, 40, target_off_bmp, NOTE_HEAD_HEIGHT);
		}
	}
}

/*------ Function: render_score ------
PURPOSE: Renders the score.

INPUT: base(UINT32 *): Pointer to frame buffer
	   position(row,col): Integer row and column pixel position
	   score(UINT32): Score value (to be printed)

OUTPUT: N/A
*/
void render_score(UINT32 *base, int row, int col, UINT32 score)
{
	char s[SCORE_MAX_LEN];
	char temp;
	int i, j;
	i = 0;
	do
	{
		s[i++] = score % 10 + '0';
		score /= 10;
	} while (score > 0);
	s[i] = '\0';
	for (j = i - 1, i = 0; i < j; i++, j--)
	{
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
	}
	plot_string((UINT8 *)base, row + SCORE_OFFSET, col, s);
}