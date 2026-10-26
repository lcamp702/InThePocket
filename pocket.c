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

int main() {
    UINT32 timeThen, timeNow, timeElapsed;
    char quit = 0; /* Quit = false */
    /* Initialize model */
    /* Render model*/
    timeThen = get_time();
    while (!quit) {
        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        /* If input is pending */
        if (Cconis())
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