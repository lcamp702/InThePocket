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

void main_init_buffers(UINT32 *buffers[2]);
void main_render(UINT32 *base, Model *model);

int main()
{
	UINT32 time_then, time_now;
	Model model;
	UINT32 *buffers[2];
	UINT8 buffer = 0;
	UINT8 quit;

	time_then = get_time();
	model_init(&model, song, SONG_NOTES, time_then);

	main_init_buffers(buffers);
	buffer = (buffer + 1) & 1;
	main_render(buffers[buffer], &model);

	time_then = get_time();

	quit = false;
	while (!quit)
	{
		time_now = get_time();

		if (has_input())
			asynch_key_hit(&model, get_input());
			
		if ((time_now - time_then) > 0)
		{
			synch_clock_tick(&model, time_now);

			cond_check_misses(&model);
			quit = cond_game_over(&model);

			buffer = (buffer + 1) & 1;
			main_render(buffers[buffer], &model);

			time_then = get_time();
		}
	}

	main_render(buffers[0], &model);

	return 0;
}

void main_init_buffers(UINT32 *buffers[2])
{
	/* Stop OS cursor blink */
	disable_cursor();
	/* Initialize buffer pointers */
	buffers[0] = get_pbase();
	buffers[1] = buff_256();
}

void main_render(UINT32 *base, Model *model)
{
	render(base, model);
	set_screen(base);
}