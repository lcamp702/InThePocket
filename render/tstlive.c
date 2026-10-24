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

    tstrend.c*/

#include "render.h"
#include "../events/events.h"
#include "../raster/raster.h"
#include <osbind.h>
#include <stdio.h>

static const Note test_song[] = {
	{ 0, 0, 0, 0 },
	{ 6, 0, 1, 1 },
	{ 12, 0, 2, 2 },
	{ 18, 0, 3, 3 },
    { 21, 0, 0, 0 },
	{ 24, 0, 1, 1 },
	{ 36, 0, 2, 2 },
	{ 40, 0, 3, 3 },
    { 43, 0, 0, 0 },
	{ 60, 0, 1, 1 },
	{ 78, 0, 2, 2 },
	{ 90, 0, 3, 3 }
};

#define TEST_SONG_LEN (sizeof(test_song)/sizeof(test_song[0]))
#define RENDER_EVERY 10   /* ticks between renders in long loops */

void show(const char *title);
void show_frame(Model *model, UINT32 *base);
void tick_and_show(Model *model, UINT32 *base, UINT16 ticks, UINT16 step);
void skip_ticks(Model *model, UINT16 ticks);
void apply_hits_and_show(Model *model, UINT32 *base);
void test_init(Model *model, UINT32 *base);
void test_clock_tick(Model *model, UINT32 *base);
void test_advance_head(Model *model, UINT32 *base);
void test_game_over(Model *model, UINT32 *base, INT8 enjoyment, const char *title);
void test_hit(Model *model, UINT32 *base, UINT8 pitch);
void test_apply_hit(Model *model, UINT32 *base);
void test_apply_miss(Model *model, UINT32 *base);

int main(void){
	UINT32 *base = (UINT32 *) Physbase();
	static Model test_model;
	UINT8 pitch;

	test_init(&test_model, base);
	test_clock_tick(&test_model, base);
	test_advance_head(&test_model, base);
	test_game_over(&test_model, base, ENJOYMENT_START, "cond_is_game_over");
	for(pitch = KEY_A; pitch <= KEY_F; pitch++){
		test_hit(&test_model, base, pitch);
	}
	test_apply_hit(&test_model, base);
	test_apply_miss(&test_model, base);

	return 0;
}

/*------ Function: show ------
PURPOSE: Prints the name of the next test and waits for a keypress to start it.

INPUT: title, the test name to print

OUTPUT: N/A
*/
void show(const char *title){
	printf("Press any key to proceed with %s test\n", title);
	fflush(stdout);
	Cnecin();
}

/*------ Function: show_frame ------
PURPOSE: Renders the model, waits for a keypress, then clears the screen.

INPUT: pointer to Model to render, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void show_frame(Model *model, UINT32 *base){
	render(model, base);
	Vsync();
}

/*------ Function: tick_and_show ------
PURPOSE: Runs a number of clock ticks, rendering a frame every step ticks.

INPUT: pointer to Model, UINT32 pointer to frame buffer, ticks (total ticks to run), step (ticks between renders, 1 = every tick)

OUTPUT: N/A
*/
void tick_and_show(Model *model, UINT32 *base, UINT16 ticks, UINT16 step){
	UINT16 i;

	for(i = 1; i <= ticks; i++){
		synch_clock_tick(model);
		if(i % step == 0){
			show_frame(model, base);
		}
	}
}

/*------ Function: skip_ticks ------
PURPOSE: Runs clock ticks without rendering, to move notes near the hit line.

INPUT: pointer to Model, ticks to run

OUTPUT: N/A
*/
void skip_ticks(Model *model, UINT16 ticks){
	UINT16 i;

	for(i = 0; i < ticks; i++){
		synch_clock_tick(model);
	}
}

/*------ Function: apply_hits_and_show ------
PURPOSE: For 30 ticks, applies a hit whenever any lane is hittable and renders the result, so the meter is seen filling.

INPUT: pointer to Model, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void apply_hits_and_show(Model *model, UINT32 *base){
	UINT8 i;

	for(i = 0; i < 30; i++){
		synch_clock_tick(model);
		if(asynch_is_hit(model, 0) || asynch_is_hit(model, 1) ||
		   asynch_is_hit(model, 2) || asynch_is_hit(model, 3)){
			asynch_apply_hit(model);
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
void test_init(Model *model, UINT32 *base){
	show("model_init");
    render_level(model, base);
	model_init(model, test_song, TEST_SONG_LEN);
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
void test_clock_tick(Model *model, UINT32 *base){
	show("synch_clock_tick");
    render_level(model, base);
	tick_and_show(model, base, 70, 1);
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
void test_advance_head(Model *model, UINT32 *base){
	UINT8 i;

	show("model_advance_head");
    render_level(model, base);
	show_frame(model, base);
	for(i = 0; i < 5; i++){
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
void test_game_over(Model *model, UINT32 *base, INT8 enjoyment, const char *title){
	show(title);
    render_level(model, base);
	model_init(model, test_song, TEST_SONG_LEN);
	model->meter.enjoyment = enjoyment;
	skip_ticks(model, 145);
	tick_and_show(model, base, 140, 1);
}


/*------ Function: test_hit ------
PURPOSE: Renders notes passing through the hit window for one pitch lane, every 5 ticks, with the meter held full.

INPUT: pointer to Model, UINT32 pointer to frame buffer, pitch lane being hit

TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
5		Checking that notes		Model with		Notes get hit and		Notes get hit and 
		can be hit in their		our test song	disappear off screen	disappear off screen
		hit windows

OUTPUT: N/A
*/
void test_hit(Model *model, UINT32 *base, UINT8 pitch){
	UINT8 i;

	show("asynch_is_hit");
    render_level(model, base);
	model_init(model, test_song, TEST_SONG_LEN);
	skip_ticks(model, 138);
	for(i = 1; i <= 30; i++){
		model->meter.enjoyment = ENJOYMENT_MAX;
		synch_clock_tick(model);
		asynch_is_hit(model, pitch);
		if(i % 5 == 0){
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
void test_apply_hit(Model *model, UINT32 *base){
	show("asynch_apply_hit");
    render_level(model, base);
	model_init(model, test_song, TEST_SONG_LEN);
	skip_ticks(model, 138);
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
void test_apply_miss(Model *model, UINT32 *base){
	UINT8 i;

	show("asynch_apply_miss");
    render_level(model, base);
	model_init(model, test_song, TEST_SONG_LEN);
	skip_ticks(model, 133);
	for(i = 0; i < 5; i++){
		synch_clock_tick(model);
		if(!asynch_is_hit(model, 3)){
			asynch_apply_miss(model);
			show_frame(model, base);
		}
	}
	apply_hits_and_show(model, base);
}
