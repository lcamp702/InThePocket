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

#define SONG_NOTES 40

static const Note song[SONG_NOTES] = {
    /* UINT32 time, INT16 x, UINT8 pitch, UINT8 duration */
    {  3703,  0, 3, 0 },
    {  3978,  0, 2, 0 },
    {  4254,  0, 1, 0 },
    {  4407,  0, 2, 0 },
    {  4680,  0, 1, 0 },
    {  4915,  0, 3, 0 },
    {  5047,  0, 2, 0 },
    {  5323,  0, 2, 0 },
    {  5558,  0, 3, 0 },
    {  6172,  0, 1, 0 },
    {  6447,  0, 2, 0 },
    {  6579,  0, 3, 0 },
    {  6814,  0, 1, 0 },
    {  7090,  0, 2, 0 },
    {  7365,  0, 3, 0 },
    {  7641,  0, 1, 0 },
    {  7916,  0, 2, 0 },
    {  8192,  0, 3, 0 },
    {  8467,  0, 1, 0 },
    {  8742,  0, 2, 0 },
    {  8895,  0, 3, 2 },
    {  8924,  0, 3, 0 },
    {  9077,  0, 3, 2 },
    {  9106,  0, 3, 0 },
    {  9381,  0, 1, 0 },
    {  9657,  0, 2, 2 },
    {  9670,  0, 2, 2 },
    {  9699,  0, 2, 0 },
    {  9934,  0, 3, 0 },
    { 10209,  0, 1, 0 },
    { 10485,  0, 2, 0 },
    { 10617,  0, 3, 0 },
    { 10852,  0, 1, 0 },
    { 11127,  0, 3, 0 },
    { 11403,  0, 1, 0 },
    { 11678,  0, 2, 0 },
    { 11954,  0, 3, 0 },
    { 12229,  0, 1, 0 },
    { 12361,  0, 2, 0 },
    { 12596,  0, 3, 2 } 
};

int main() {
    UINT32 time_then, time_now, time_elapsed;
    UINT32 *base;
	static Model model;
    UINT8 quit;

    quit = 0;
    base = get_pbase();
    time_then = get_time();
    /* Initialize model */
    model_init(&model, song, SONG_NOTES, time_now);
    /* Render model */
	render_level(&model, base);
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
            render(&model, base);
            time_then = time_now;
        }
    }
    return 0;
}