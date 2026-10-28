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
    {3703, 3, 0},
    {3978, 2, 0},
    {4254, 1, 0},
    {4407, 2, 0},
    {4680, 1, 0},
    {4915, 3, 0},
    {5047, 2, 0},
    {5323, 2, 0},
    {5558, 3, 0},
    {6172, 1, 0},
    {6447, 2, 0},
    {6579, 3, 0},
    {6814, 1, 0},
    {7090, 2, 0},
    {7365, 3, 0},
    {7641, 1, 0},
    {7916, 2, 0},
    {8192, 3, 0},
    {8467, 1, 0},
    {8742, 2, 0},
    {8895, 3, 2},
    {8924, 3, 0},
    {9077, 3, 2},
    {9106, 3, 0},
    {9381, 1, 0},
    {9657, 2, 2},
    {9670, 2, 2},
    {9699, 2, 0},
    {9934, 3, 0},
    {10209, 1, 0},
    {10485, 2, 0},
    {10617, 3, 0},
    {10852, 1, 0},
    {11127, 3, 0},
    {11403, 1, 0},
    {11678, 2, 0},
    {11954, 3, 0},
    {12229, 1, 0},
    {12361, 2, 0},
    {12596, 3, 2}};

int main()
{
    static Model model;
    UINT32 *buff_1, *buff_2;
    UINT32 time_then, time_now, time_elapsed;
    UINT8 quit, buffer;

    quit = 0;
    buffer = 0;
    buff_1 = get_pbase();
    buff_2 = malloc_256((SCREEN_WIDTH * SCREEN_HEIGHT) >> 3);

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
            quit = cond_game_over(&model);
            /* Render frame */
            if (buffer == 0)
            {
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
    free_buff(buff_2);

    return 0;
}