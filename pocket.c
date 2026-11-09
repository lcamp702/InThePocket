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
	UINT32 *buff_1, *buff_2;
	UINT32 time_then, time_now, time_elapsed;
	UINT8 quit, buffer;

	/* Stop OS cursor blink */
	disable_cursor();
	/* Initialize buffers */
	buffer = 0; /* Set buffer number (main buffer showing) */
	buff_1 = get_pbase(); /* Get main buffer address */
	buff_2 = buff_256(); /* Get second buffer address */
	/* Initialize model */
	time_then = get_time();
	model_init(&model, song, SONG_NOTES, time_then);
	/* Render model (first frame) */
	render(buff_1, &model);
	render(buff_2, &model);
	/* Set quit = false */
	quit = 0;
	while (!quit)
	{ /* Repeat until quit */
		time_now = get_time();
		time_elapsed = time_now - time_then; /* Time since drawing last frame */
		if (has_input())
			/* If input is pending, process async event */
			asynch_key_hit(&model, get_input());
			
		if (time_elapsed > 0)
		{ 	/* If clock has ticked, process sync events */
			synch_clock_tick(&model, time_now);
			/* Process cond events */
			cond_check_misses(&model);
			quit = cond_game_over(&model);
			/* Render model (next frame) */
			if (buffer == 0)
			{	/* Currently showing main buffer */
				render(buff_2, &model); /* Render to second buffer */
				set_screen(buff_2);		/* Show second buffer */
				buffer = 1;
			}
			else
			{	/* Currently showing second buffer */
				render(buff_1, &model); /* Render to main buffer */
				set_screen(buff_1);		/* Show main buffer */
				buffer = 0;
			}
			/* Update game module clock */
			time_then = get_time(); /* Time after drawing last frame */
		}
	}

	/* Render one more frame for game over */
	render(buff_1, &model);
	/* Return to main buffer on exit */
	set_screen(buff_1);

	return 0;
}