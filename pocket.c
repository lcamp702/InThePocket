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
#include "events/events.h"
#include "song/song.h"

int main()
{
	static Model model;
	UINT32 *buff_1, *buff_2;
	UINT32 time_then, time_now, time_elapsed;
	UINT8 quit, buffer;

	disable_cursor();

	quit = 0;
	buffer = 0;
	buff_1 = get_pbase();
	buff_2 = buff_256();

	/* Initialize model */
	time_then = get_time();
	model_init(&model, song, SONG_NOTES, time_then);
	/* Render model */
	render(buff_1, &model);
	render(buff_2, &model);
	while (!quit)
	{
		time_now = get_time();
		time_elapsed = time_now - time_then;
		/* If input is pending */
		if (waiting_input())
		{
			/* Process async events */
			if (asynch_is_hit(&model, get_input()))
				asynch_apply_hit(&model);
			else
				asynch_apply_miss(&model);
		}
		/* If clock has ticked*/
		if (time_elapsed > 0)
		{
			/* Process sync events */
			synch_clock_tick(&model, time_now);
			/* Process cond events */
			cond_check_misses(&model);
			model.meter.enjoyment = 48; /* DEBUG ONLY: Nofail mode */
			quit = cond_game_over(&model);
			/* Render frame */
			if (buffer == 0)
			{ /* Currently showing main buffer, render on secondary */
				render(buff_2, &model);
				set_screen(buff_2);
				buffer = 1;
			}
			else
			{
				render(buff_1, &model);
				set_screen(buff_1);
				buffer = 0;
			}
			/* Update game module clock */
			time_then = time_now;
		}
	}

	set_screen(buff_1);

	return 0;
}