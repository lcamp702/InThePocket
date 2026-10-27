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

#include "model/model.h"
#include "render/render.h"
#include "system/system.h"
#include "events/events.h"

#define SONG_NOTES 50
static const Note song[SONG_NOTES] = {
    /* UINT32 time, INT16 x, UINT8 pitch, UINT8 duration */
    {}
};

int main() {
    UINT32 time_then, time_now, time_elapsed;
    UINT32 *base = (UINT32 *) Physbase();
	static Model model;
    /* Quit = false */
    UINT8 quit = 0;
    time_then = get_time();
    /* Initialize model */
    model_init(&model, song, SONG_NOTES, time_now);
    /* Render model */
	show_frame(model, base);
    while (!quit) {
        time_now = get_time();
        time_elapsed = time_now - time_then;
        /* If input is pending */
        if (waiting_input())
        {
            /* Process async events */
            if(asynch_is_hit(&model, get_input()))
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
            quit = cond_game_over(&model);
            /* Render frame */
            
            time_then = time_now;
        }
    }

    return 0;
}