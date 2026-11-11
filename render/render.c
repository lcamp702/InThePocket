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
#include "../system/system.h"
#include "../bitmaps/bitmaps.h"
#include "../raster/raster.h"

/* ===== Staff ===== */
#define STAFF_BOTTOM 372   /* Bottom pixel row of staff */
#define LINE_COUNT 5	   /* 5 lines, 4 tracks between for notes*/
#define STAFF_LINE_WIDTH 2 /* Thickness of each staff line */

/* ==== Targets ==== */
#define TARGET_COL 64

/* ===== Meter ===== */
#define METER_ROW 8
#define METER_COL 568

/* ===== Score ===== */
#define SCORE_ROW 8
#define SCORE_COL 8
#define SCORE_MAX_LEN 6

/* ===== Notes ===== */
#define STEM_LENGTH (UINT16)((NOTE_HEAD_HEIGHT) * 3)
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

static const UINT32 *note_heads[3] = {
	note_whole_head,
	note_half_head,
	note_black_head};

static UINT32 *buffers[2];
static UINT8 buffer = 0; /* On-screen buffer */

int render_int_to_string(char *str, UINT32 value, int places);
void render_end_screen(UINT32 *base, char *t1, UINT16 t1_len, char *t2, UINT16 t2_len, UINT32 score);

/* ==== Function: render_init_buffers ====
PURPOSE: Initialize the buffers for use with the main rendering function

INPUT: N/A

OUTPUT: N/A
*/
void render_init_buffers()
{
	disable_cursor();
	buffers[0] = get_pbase();
	buffers[1] = buff_256();
}

/* ==== Function: render ====
PURPOSE: Master render function. Calls the other render functions, and renders the game world onto the screen.

INPUT: base(UINT32 *): Pointer to frame buffer,
	   model(Model *): Pointer to Model

OUTPUT: N/A
*/
void render(Model *model)
{
	UINT16 i, next_note;
	UINT32 playback_col;
	
	buffer = (++buffer) & 1;

	/* Render next frame */
	if (model->head >= model->note_count)
	{ /* Game won */
        render_end_screen(buffers[buffer], "You Win", 7, "This cat can jam!", 17, model->meter.score);
        return;
	}

	if (model->meter.enjoyment <= 0)
	{ /* Game over */
		render_end_screen(buffers[buffer], "Game Over", 9, "Doug was booed off stage", 24, model->meter.score);
		return;
	}

	if (model->new_bg)
	{ /* Render background and staff (If needed) */
		clear_screen(buffers[buffer]);
		render_staff(buffers[buffer]);
		plot_20l_bitmap(buffers[buffer], 0, bg, BG_HEIGHT);
		model->new_bg--;
	}

	if (model->meter.dirty)
	{ /* Render stats (Enjoyment, score, multiplier) */
		render_enjoyment(buffers[buffer], model->meter.enjoyment); /* Render enjoyment meter */
		render_score(buffers[buffer], SCORE_ROW, SCORE_COL, model->meter.score); /* Render Score value */
		render_multiplier(buffers[buffer], SCORE_ROW, SCORE_COL, model->meter.multiplier, model->meter.multi_progress);
		model->meter.dirty--;
	}

	clean_staff(buffers[buffer]); /* Clear between staff lines */

	render_targets(buffers[buffer], model->targets); /* Redraw targets */
	
	next_note = model_next_disp_note(model); /* Get end note index */
	playback_col = NOTE_START_X - model->game_clock; /* Note x position offset */

	for (i = model->head; i < next_note; i++)
		if (model->notes[i].hit == 0) /* If note is not hit, draw it */
			render_note(buffers[buffer], &model->notes[i], playback_col);
	
	set_screen(buffers[buffer]); /* Queue last frame for display */
}

/* ==== Function: render_exit ====
PURPOSE: Render to and show buffer 0

INPUT: N/A

OUTPUT: N/A
*/
void render_exit(Model *model)
{
	buffer = 1;
	render(model); /* Render will display to buffer 0 before exiting */
}

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
void render_end_screen(UINT32 *base, char *t1, UINT16 t1_len, char *t2, UINT16 t2_len, UINT32 score)
{
    int row = 168;
    char score_str[SCORE_MAX_LEN + 1];
    int places = render_int_to_string(score_str, score, SCORE_MAX_LEN);
	int score_col = 264 - (8 * places);
    fill_screen(base);
    plot_nxstring(base, row		, 320 - (16 * t1_len), t1, 2);
    plot_nxstring(base, row + 32, 320 - (8 * t2_len), t2, 1);

    plot_nxstring(base, row + 48, score_col, "Score: ", 1);
    plot_nxstring(base, row + 48, score_col + 112, score_str, 1);
}

/* ==== Function: render_staff ====
PURPOSE: Renders one four line staff.

INPUT: base(UINT32 *): Pointer to frame buffer

OUTPUT: N/A
*/
void render_staff(UINT32 *base)
{
	int row = STAFF_BOTTOM - 1;
	int i, w;
	
	for (i = 0; i < LINE_COUNT; i++)
	{
		for (w = 0; w < STAFF_LINE_WIDTH; w++)
		{
			plot_horizontal_line(base, row + w, 0, SCREEN_WIDTH);
		}
		row -= (NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH);
	}
}

/* ==== Function: clean_staff ====

PURPOSE: Clear all the pixels between the lines on the staff and slightly above/below.

INPUT: base(UINT32 *): Pointer to frame buffer

OUTPUT: N/A
*/
void clean_staff(UINT32 *base)
{
	int row = STAFF_BOTTOM - 1;
	int i;

	clear_20l(base, row + STAFF_LINE_WIDTH, 2);

	for (i = 0; i < LINE_COUNT - 1; i++)
	{
		/* Clear between the lines */
		clear_20l(base, row - NOTE_HEAD_HEIGHT, NOTE_HEAD_HEIGHT);
		row -= (NOTE_HEAD_HEIGHT + STAFF_LINE_WIDTH);
	}
	clear_20l(base, row-2, 2);
}

/* ==== Function: render_note ====
PURPOSE: Renders a single note to screen. It's place in the staff is determined by its pitch.

INPUT: base(UINT32 *): Pointer to frame buffer
	   note(Note *): Pointer to note


OUTPUT: N/A
*/
void render_note(UINT32 *base, Note *note, UINT32 playback_col)
{
	int row = pitch_row[note->pitch];
	int col = playback_col + note->time;

	UINT32 *note_head = note_heads[note->duration];

	plot_32bit_bitmap(base, row, col, note_head, NOTE_HEAD_HEIGHT);

	/*
	Note Stems (Vertical lines)
	*/

	row += STEM_ROW;

	if (note->duration > 0 && note->neighbour == 0)
	{
		if (note->pitch > 1) /* Bottom 2 tracks */
			col += STEM_COL_L;
		else /* Top 2 tracks */
		{
			row -= STEM_LENGTH;
			col += STEM_COL_R;
		}

		plot_vertical_line(base, row, col, STEM_LENGTH);
	}
}

/* ==== Function: render_targets ====
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
			plot_16bit_bitmap((UINT16 *)base, row, TARGET_COL, target_on_bmp, NOTE_HEAD_HEIGHT);
			targets[i] = 0; /* Clear after rendering */
		}
		else
			plot_16bit_bitmap((UINT16 *)base, row, TARGET_COL, target_off_bmp, NOTE_HEAD_HEIGHT);
	}
}

/* ==== Function: render_enjoyment ====
PURPOSE: Renders meter. Fills it up based on enjoyment.

INPUT: base(UINT32 *): Pointer to frame buffer
	   meter(Meter *): pointer to meter

OUTPUT: N/A
*/
void render_enjoyment(UINT32 *base, INT8 enjoyment)
{
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
		plot_32bit_bitmap(base, METER_ROW, METER_COL, sax_full, plot_height);
		plot_32bit_bitmap(base, METER_ROW, METER_COL + 32, (sax_full + METER_HEIGHT), plot_height);
		/* Plot empty portion of the meter */
		plot_n32bit_bitmap(base, METER_ROW, METER_COL, sax_empty, plot_height);
		plot_n32bit_bitmap(base, METER_ROW, METER_COL + 32, (sax_empty + METER_HEIGHT), plot_height);
	}

	if (enjoyment > 0)
	{
		/* Plot filled portion of the meter (Always overwrites empty section) */
		plot_n32bit_bitmap(base, METER_ROW + plot_height, METER_COL, sax_full + plot_height, enjoyment);
		plot_n32bit_bitmap(base, METER_ROW + plot_height, METER_COL + 32, sax_full + METER_HEIGHT + plot_height, enjoyment);
	}
}

/* ==== Function: render_score ====
PURPOSE: Renders the score.

INPUT: base(UINT32 *): Pointer to frame buffer
	   position(row,col): Integer row and column pixel position
	   score(UINT32): Score value (to be printed)

OUTPUT: N/A
*/
void render_score(UINT32 *base, int row, int col, UINT32 score)
{
	char s[SCORE_MAX_LEN + 1];
	render_int_to_string(s,score,SCORE_MAX_LEN);
	fill_region(base, row, col, 16, 16 * SCORE_MAX_LEN);
	plot_nxstring(base, row, col, s, 1);
}

/* ==== Function: render_multiplier ====
PURPOSE: Renders the multiplier number (X1) and multiplier progress.

INPUT: base(UINT32 *): Pointer to frame buffer
	   position(row,col): Integer row and column pixel position
	   multi(UINT8): Multiplier value (to be printed)
	   progress(UINT8): Number of current consecutive hits

OUTPUT: N/A
*/
void render_multiplier(UINT32 *base, int row, int col, UINT8 multi, UINT8 progress)
{	/* Print score multiplier */
	char str[3] = {'x', '0', '\0'};
	int i;
	str[1] += multi;

	fill_region(base, row, col + (16 * SCORE_MAX_LEN), 32, 64);
	plot_nxstring(base, row, col + (16 * SCORE_MAX_LEN), str, 2);

	for (i = 0; i < CONSEC_MULTI; i++) {
		if(progress > i)
			plot_n8bit_bitmap((UINT8 *)base, row + 16, col + (i * 9), multi_on, MULTI_HEIGHT);
		else
		{
			/* Plot in black to erase potential filled dot */
			plot_8bit_bitmap((UINT8 *)base, row + 16, col + (i * 9), multi_on, MULTI_HEIGHT);
			plot_n8bit_bitmap((UINT8 *)base, row + 16, col + (i * 9), multi_off, MULTI_HEIGHT);
		}
	}
}

/* ==== Function: render_int_to_string ====
PURPOSE: Converts an int value to a string and writes it to the given char array

INPUT: str(char *): Pointer to string of length places + 1
		value(int): Integer to write to the string
	   places(int): Number of decimal digits to print

OUTPUT: Places actually printed
*/
int render_int_to_string(char *str, UINT32 value, int places)
{
	int i = 0;
	int j;
	int len;

	do
	{
		str[i++] = (char)((value % 10) + '0');
		value /= 10;
	} while (value > 0 && i < places);

	str[i] = '\0';
	len = i-1;

	for (j = i - 1, i = 0; i < j; i++, j--)
	{
		char temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}

	return len;
}