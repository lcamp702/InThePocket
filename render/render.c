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

	render/render.c
*/

#include "render.h"
#include "../bitmaps/bitmaps.h"
#include "../raster/raster.h"

/* ===== Staff ===== */
#define STAFF_BOTTOM 372   /* Bottom pixel row of staff */
#define LINE_COUNT 5	   /* 5 lines, 4 tracks between for notes*/
#define STAFF_LINE_WIDTH 2 /* Thickness of each staff line */

/* ===== Meter ===== */
#define METER_ROW 8
#define METER_COL 568

/* ===== Score ===== */
#define SCORE_ROW 8
#define SCORE_COL 8
#define SCORE_MAX_LEN 12

/* ===== Notes ===== */
#define STEM_LENGTH (int)((NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH) * 3.5)
#define STEM_COL_R 26 /* Stem column offset */
#define STEM_COL_L 5
#define STEM_ROW 8
#define STEM_WIDTH 2

#define NOTE_START_X 640 /* right edge, ST high-res; adjust to res */

static const int pitch_row[4] = {
	STAFF_BOTTOM + 1 - (1 * (NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH)),
	STAFF_BOTTOM + 1 - (2 * (NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH)),
	STAFF_BOTTOM + 1 - (3 * (NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH)),
	STAFF_BOTTOM + 1 - (4 * (NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH))};

static const UINT32 *const note_heads[3] = {
	note_whole_head,
	note_half_head,
	note_black_head};

void print_debug(UINT8 *base, char *text, UINT32 number)
{
	char s[SCORE_MAX_LEN];
	int i = 0;
	int j;

	fill_region((UINT32 *)base, 24, 8, 8, 200);

	plot_nstring(base, 24, 0, text);

	do
	{
		s[i++] = (char)((number % 10) + '0');
		number /= 10;
	} while (number > 0);

	s[i] = '\0';

	for (j = i - 1, i = 0; i < j; i++, j--)
	{
		char temp = s[i];
		s[i] = s[j];
		s[j] = temp;
	}
	plot_nstring(base, 24, 80, s);
}

/*------ Function: render ------
PURPOSE: Master render function. Calls the other render functions, and renders the game world onto the screen.

INPUT: model(Model *): Pointer to Model,
	   base(UINT32 *): Pointer to frame buffer

OUTPUT: N/A
*/
void render(UINT32 *base, Model *model)
{
	UINT16 i, next_note;
	UINT32 playback_col;

	if (model->render_bg)
	{ /* Plot background once */
		clear_screen(base);
		plot_20l_bitmap(base, 0, bg, BG_HEIGHT);
		model->render_bg--;
	}

	render_staff(base);

	next_note = model_next_disp_note(model);
	playback_col = NOTE_START_X - model->game_clock;
	for (i = model->head; i < next_note; i++)
		render_note(base, &model->notes[i], playback_col);

	render_targets(base, model->targets);

	/* Top stats */
	if (model->meter.dirty)
	{
		render_meter(base, &model->meter);
		render_score(base, SCORE_ROW, SCORE_COL, model->meter.score);
		model->meter.dirty--;
	}
}

/*------ Function: render_note ------
PURPOSE: Renders a single note to screen. It's place in the staff is determined by its pitch.

INPUT: base(UINT32 *): Pointer to frame buffer
	   note(Note *): Pointer to note


OUTPUT: N/A
*/
void render_note(UINT32 *base, Note *note, UINT32 playback_col)
{
	/*
		TODO:

		Note flags
		Ligatures (Connected flags)
		Connecting stems for multiple notes
	*/

	int row = pitch_row[note->pitch];
	int col = playback_col + note->time;

	UINT32 *note_head = note_heads[note->duration];
	plot_32bit_bitmap(base, row, col, note_head, NOTE_HEAD_HEIGHT);

	/*
	Note Stems (Vertical lines)
	*/

	if (note->duration > 0 && note->neighbour == 0)
	{
		int w;
		if (note->pitch > 1) /* Bottom 2 tracks */
				plot_vertical_line(base, row + STEM_ROW, col + STEM_COL_L, STEM_LENGTH);
		else /* Top 2 tracks */
				plot_vertical_line(base, row + STEM_ROW - STEM_LENGTH, col + STEM_COL_R, STEM_LENGTH);
	}
}

/*------ Function: render_meter ------
PURPOSE: Renders meter. Fills it up based on enjoyment.

INPUT: base(UINT32 *): Pointer to frame buffer
	   meter(Meter *): pointer to meter

OUTPUT: N/A
*/
void render_meter(UINT32 *base, Meter *meter)
{
	int i;
	int enjoyment = meter->enjoyment;
	UINT16 plot_height;

	/* Clip enjoyment to meter size */
	/* Replace with scaling if meter height and max value are different. */
	if (enjoyment > METER_HEIGHT)
		enjoyment = METER_HEIGHT;
	if (enjoyment < 0)
		enjoyment = 0;

	plot_height = (UINT16)METER_HEIGHT - enjoyment;

	if (enjoyment < METER_HEIGHT)
	{
		/* Plot meter in black to erase negative version */
		plot_32bit_bitmap(base, METER_ROW, METER_COL, full_left, plot_height);
		plot_32bit_bitmap(base, METER_ROW, METER_COL + 32, full_right, plot_height);
		/* Plot empty portion of the meter */
		plot_n32bit_bitmap(base, METER_ROW, METER_COL, emp_left, plot_height);
		plot_n32bit_bitmap(base, METER_ROW, METER_COL + 32, emp_right, plot_height);
	}

	if (enjoyment > 0)
	{
		/* Plot filled portion of the meter (Always overwrites empty section) */
		plot_n32bit_bitmap(base, METER_ROW + plot_height, METER_COL, full_left + plot_height, enjoyment);
		plot_n32bit_bitmap(base, METER_ROW + plot_height, METER_COL + 32, full_right + plot_height, enjoyment);
	}
}

/*------ Function: render_staff ------
PURPOSE: Renders one four line staff.

INPUT: base(UINT32 *): Pointer to frame buffer

OUTPUT: N/A
*/
void render_staff(UINT32 *base)
{
	int row = STAFF_BOTTOM - 1;
	int i, w;

	clear_20l(base, 283, 108);

	for (i = 0; i < LINE_COUNT; i++)
	{
		for (w = 0; w < STAFF_LINE_WIDTH; w++)
		{
			plot_horizontal_line(base, row + w, 0, SCREEN_WIDTH);
		}
		row -= (NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH);
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
	int i;
	for (i = 0; i < TARGET_COUNT; i++) /* Draw one target for each space (0 to 3) */
	{
		int row = STAFF_BOTTOM + 1 - ((i + 1) * (NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH));
		if (targets[i])
		{
			plot_16bit_bitmap((UINT16 *)base, row, 40, target_on_bmp, NOTE_HEAD_HEIGHT);
			targets[i] = 0; /* Clear after rendering */
		}
		else
			plot_16bit_bitmap((UINT16 *)base, row, 40, target_off_bmp, NOTE_HEAD_HEIGHT);
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
	int i = 0;
	int j;

	do
	{
		s[i++] = (char)((score % 10) + '0');
		score /= 10;
	} while (score > 0);

	s[i] = '\0';

	for (j = i - 1, i = 0; i < j; i++, j--)
	{
		char temp = s[i];
		s[i] = s[j];
		s[j] = temp;
	}

	fill_region(base, row, col, 8, 8 * SCORE_MAX_LEN);
	plot_nstring((UINT8 *)base, row, col, s);
}