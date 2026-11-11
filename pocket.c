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

int main()
{
	static Model model;
	UINT32 time_then, time_now;
	UINT8 quit;

	/* Initialize buffers */
	render_init_buffers();
	/* Initialize model */
	time_then = get_time();
	model_init(&model, song, SONG_NOTES, time_then);
	/* Render model (first frame) */
	render(&model);
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
			render(&model);
			/* Save time after drawing last frame */
			time_then = get_time();
		}
	}

	/* Return to main buffer on exit */
	render_exit(&model);

	return 0;
}