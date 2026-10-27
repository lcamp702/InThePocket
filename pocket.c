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

Note song[] = {
    /* UINT32 time, INT16 x, UINT8 pitch, UINT8 duration */
    {}
}

int main() {
    UINT32 timeThen, timeNow, timeElapsed;
    UINT32 *base = (UINT32 *) Physbase();
	static Model model;
    /* Quit = false */
    char quit = 0;
    timeThen = get_time();
    /* Initialize model */
    model_init(model, test_song, TEST_SONG_LEN);
    /* Render model */
	show_frame(model, base);
    while (!quit) {
        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        /* If input is pending */
        if (waiting_input())
        {
            /* Process async events */

        }

        /* If clock has ticked*/
        if (timeElapsed > 0) 
        {
            /* Process sync events */
            /* Process cond events */
            /* Render frame */
            timeThen = timeNow;
        }
    }

    return 0;
}