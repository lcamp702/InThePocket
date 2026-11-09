/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 3
	Due: Nov 9, 2026

	The main game module for In the Pocket.
	This module will orchestrate the main game loop using submodule functions.
	pocket.c
*/

#include "types.h"
#include "model/model.h"
#include "render/render.h"
#include "system/system.h"
#include "system/input.h"
#include "events/events.h"
#include "song/song.h"

static UINT32 *buffers[2];
static UINT8 buffer = 0;

void double_buffer_render(Model *model)
{
	buffer = (buffer + 1) & 1;	/* Get number of next buffer (1 or 0) */
	render(buffers[buffer], model); /* Render */
	set_screen(buffers[buffer]); /* Queue for display */
}

int main()
{
	static Model model;
	UINT32 time_then, time_now;
	UINT8 quit;

	/* Stop OS cursor blink */
	disable_cursor();
	/* Initialize buffers */
	buffers[0] = get_pbase(); /* Get main buffer address */
	buffers[1] = buff_256(); /* Get second buffer address */
	/* Initialize model */
	time_then = get_time();
	model_init(&model, song, SONG_NOTES, time_then);
	/* Render model (first frame) */
	double_buffer_render(&model);
	time_then = get_time();
	/* Set quit = false */
	quit = 0;
	while (!quit)
	{ /* Repeat until quit */
		time_now = get_time();
		if (has_input())
			/* If input is pending, process async event */
			asynch_key_hit(&model, get_input());
			
		if ((time_now - time_then) > 0)
		{ 	/* If clock has ticked, process sync events */
			synch_clock_tick(&model, time_now);
			/* Process cond events */
			cond_check_misses(&model);
			quit = cond_game_over(&model);
			/* Render model (next frame) */
			double_buffer_render(&model);
			/* Save time after drawing last frame */
			time_then = get_time();
		}
	}

	/* Render one more frame for game over */
	render(buffers[0], &model);
	/* Return to main buffer on exit */
	set_screen(buffers[0]);

	return 0;
}