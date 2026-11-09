/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Test driver for the renderer. A copy of the model test driver (tstmodel.c)
	with the printed output replaced by calls to the master render function.
	After each render the test waits for a keypress, then clears the screen.
	Long tick loops render every RENDER_EVERY ticks instead of every tick.

	render/tstrend.c
*/

#include "render.h"
#include "../events/events.h"
#include "../raster/raster.h"
#include <osbind.h>
#include <stdio.h>

static const Note test_song[] = {
	{0, 0, 0, 0},
	{6, 0, 1, 1},
	{12, 0, 2, 2},
	{18, 0, 3, 3}};

#define TEST_SONG_LEN (sizeof(test_song) / sizeof(test_song[0]))
#define RENDER_EVERY 10 /* ticks between renders in long loops */

void show(const char *title);
void show_frame(Model *model, UINT32 *base);
void tick_and_show(Model *model, UINT32 *base, UINT16 ticks, UINT16 step);
void skip_ticks(Model *model, UINT16 ticks);
void apply_hits_and_show(Model *model, UINT32 *base);
void test_init(Model *model, UINT32 *base);
void test_clock_tick(Model *model, UINT32 *base);
void test_advance_head(Model *model, UINT32 *base);
void test_game_over(Model *model, UINT32 *base, INT8 enjoyment, const char *title);
void test_hit(Model *model, UINT32 *base, char key);
void test_apply_hit(Model *model, UINT32 *base);
void test_apply_miss(Model *model, UINT32 *base);

int main(void)
{
	UINT32 *base = (UINT32 *)Physbase();
	static Model test_model;

	test_init(&test_model, base);
	test_clock_tick(&test_model, base);
	test_advance_head(&test_model, base);
	test_game_over(&test_model, base, ENJOYMENT_START, "cond_game_over");
	test_hit(&test_model, base, 'a');
	test_hit(&test_model, base, 's');
	test_hit(&test_model, base, 'd');
	test_hit(&test_model, base, 'f');
	test_apply_hit(&test_model, base);
	test_apply_miss(&test_model, base);

	return 0;
}

/*------ Function: show ------
PURPOSE: Prints the name of the next test and waits for a keypress to start it.

INPUT: title, the test name to print

OUTPUT: N/A
*/
void show(const char *title)
{
	printf("Press any key to proceed with %s test\n", title);
	fflush(stdout);
	Cnecin();
}

/*------ Function: show_frame ------
PURPOSE: Renders the model, waits for a keypress, then clears the screen.

INPUT: pointer to Model to render, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void show_frame(Model *model, UINT32 *base)
{
	model->new_bg = 1;
	model->meter.dirty = 1;
	render(base, model);
	Cnecin();
	clear_screen(base);
}

/*------ Function: tick_and_show ------
PURPOSE: Runs a number of clock ticks, rendering a frame every step ticks.

INPUT: pointer to Model, UINT32 pointer to frame buffer, ticks (total ticks to run), step (ticks between renders, 1 = every tick)

OUTPUT: N/A
*/
void tick_and_show(Model *model, UINT32 *base, UINT16 ticks, UINT16 step)
{
	UINT16 i;

	for (i = 1; i <= ticks; i++)
	{
		synch_clock_tick(model, i);
		if (i % step == 0)
		{
			show_frame(model, base);
		}
	}
}

/*------ Function: skip_ticks ------
PURPOSE: Runs clock ticks without rendering, to move notes near the hit line.

INPUT: pointer to Model, ticks to run

OUTPUT: N/A
*/
void skip_ticks(Model *model, UINT16 ticks)
{
	UINT16 i;

	for (i = 0; i < ticks; i++)
	{
		synch_clock_tick(model, i);
	}
}

/*------ Function: apply_hits_and_show ------
PURPOSE: Through the hit windows, applies a hit whenever any lane is hittable and renders the result, so the meter is seen filling.

INPUT: pointer to Model, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void apply_hits_and_show(Model *model, UINT32 *base)
{
	UINT16 i;
	UINT8 hit;

	for (i = 568; i < 620; i++)
	{
		synch_clock_tick(model, i);
		cond_check_misses(model);
		hit = asynch_is_hit(model, KEY_F, 0);
		if (hit != (UINT8)-1 && model->head < model->note_count)
		{
			asynch_apply_hit(model, hit);
			show_frame(model, base);
		}
	}
}

/*------ Function: test_init ------
PURPOSE: Renders the model straight after model_init (notes not yet on screen).

TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT

1		Check to see if the		Empty Model		Empty Model rendered	as expected
		initialized model						to screen
		will render to
		screen.

INPUT: pointer to Model, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void test_init(Model *model, UINT32 *base)
{
	show("model_init");
	model_init(model, test_song, TEST_SONG_LEN, 1);
	show_frame(model, base);
}

/*------ Function: test_clock_tick ------
PURPOSE: Renders the model after each of 4 clock ticks (notes entering from the right).

TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
2		Checks to see that		Model that 		Some notes should		Notes move on screen edge.
		advancing game clock	ticks 4 times	move on screen edge
		correctly renders
		the changing of the
		game state + clips
		correctly
INPUT: pointer to Model, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void test_clock_tick(Model *model, UINT32 *base)
{
	show("synch_clock_tick");
	tick_and_show(model, base, 4, 1);
}

/*------ Function: test_advance_head ------
PURPOSE: Renders the model after each of 5 head advances; one fewer note is drawn each time, and advancing past the last note must not draw anything extra.

TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
3		Checks that 			Model with		Notes should 			Notes disappear
		advance_head() 			our test song	disappear
		correctly drops note
		from game state

INPUT: pointer to Model, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void test_advance_head(Model *model, UINT32 *base)
{
	UINT8 i;

	show("model_advance_head");
	show_frame(model, base);
	for (i = 0; i < 5; i++)
	{
		model_advance_head(model);
		show_frame(model, base);
	}
}

/*------ Function: test_game_over ------
PURPOSE: Lets every note be missed, rendering each tick, so the meter is seen draining from the given starting enjoyment.

TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
4		Want to check that		Model with		Enjoyment meter		 	Enjoyment meter drains properly.
		enjoyment meter			our song		should drain
		drains correctly
		on missed notes


INPUT: pointer to Model, UINT32 pointer to frame buffer, starting enjoyment, title of the test to print

OUTPUT: N/A
*/
void test_game_over(Model *model, UINT32 *base, INT8 enjoyment, const char *title)
{
	int i;
	show(title);
	model_init(model, test_song, TEST_SONG_LEN, 1);
	model->meter.enjoyment = enjoyment;
	for(i=595;i<620;i++) {
		synch_clock_tick(model, i);
		cond_check_misses(model);
		show_frame(model, base);
	}
}

/*------ Function: test_hit ------
PURPOSE: Renders notes passing through the hit window for one pitch lane, every 5 ticks, with the meter held full.

INPUT: pointer to Model, UINT32 pointer to frame buffer, key being pressed

TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
5		Checking that notes		Model with		Notes get hit and		Notes get hit and
		can be hit in their		our test song	disappear off screen	disappear off screen
		hit windows

OUTPUT: N/A
*/
void test_hit(Model *model, UINT32 *base, char key)
{
	UINT16 i;

	show("asynch_key_hit");
	model_init(model, test_song, TEST_SONG_LEN, 1);
	for (i = 560; i < 620; i++)
	{
		model->meter.enjoyment = ENJOYMENT_MAX;
		synch_clock_tick(model, i);
		cond_check_misses(model);
		asynch_key_hit(model, key);
		if (i % 5 == 0)
		{
			show_frame(model, base);
		}
	}
}

/*------ Function: test_apply_hit ------
PURPOSE: Applies a hit whenever any lane is hittable and renders the result, so the meter is seen filling.

TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
6		Checking to see that	Model with		Notes disappear and		Notes disappear and
		hitting the note and	our test song	meter fills.			meter fills.
		calling the apply_hit
		function correctly
		fills the meter and that
		the rendering of the
		meter being filled is
		handled correctly by the
		render_meter function.
INPUT: pointer to Model, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void test_apply_hit(Model *model, UINT32 *base)
{
	show("asynch_apply_hit");
	model_init(model, test_song, TEST_SONG_LEN, 1);
	skip_ticks(model, 568);
	apply_hits_and_show(model, base);
}

/*------ Function: test_apply_miss ------
PURPOSE: Applies misses while the pitch 3 lane is not hittable, rendering each one so the meter is seen draining, then applies hits as above.

TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
7		Checking that			Model with		Enjoyment meter			Enjoyment meter once reaching 0
		once meter is drained	our song		stays at 0 once			stays at 0 even when notes are being hit.
		it cannot "refill"						it reaches 0, even
		so that one cannot						when notes get hit
		go below the losing
		threshold and then
		go back up over it.
		Say in the case where
		someone had like,
		20 enjoyment, missed on
		lane 0, going to 0 then
		hit on lane 1 and goes
		to 2 enjoyment before
		game checks the game
		over state.

INPUT: pointer to Model, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void test_apply_miss(Model *model, UINT32 *base)
{
	UINT16 i;

	show("asynch_apply_miss");
	model_init(model, test_song, TEST_SONG_LEN, 1);
	for (i = 560; i < 568; i++)
	{
		synch_clock_tick(model, i);
		if (asynch_is_hit(model, KEY_F, 0) == (UINT8)-1)
		{
			asynch_apply_miss(model);
			show_frame(model, base);
		}
	}
	apply_hits_and_show(model, base);
}
