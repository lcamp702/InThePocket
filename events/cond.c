/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the cond.c file. Contains function definitions for the functions in cond.h.
	cond.c
*/

#include "cond.h"

#define SONG_PLAYING model->head < model->note_count /* The head is not at the end of the song */
#define ENJOYMENT model->meter.enjoyment > 0
#define NOTE_PASSED model->game_clock > (model->notes[model->head].time + TRAVEL_TICKS + HIT_WINDOW) /* Game clock passed hit time */

/*------ Function: cond_check_misses ------
PURPOSE: Checks to see if index points at a missed note, and if so updates the meter and repeats until head points at an "alive" note

INPUT: pointer to Model

OUTPUT: N/A
*/
void cond_check_misses(Model *model)
{
	while (SONG_PLAYING && NOTE_PASSED && ENJOYMENT)
	{																	/*checks that the head is pointed at a note that has passed the hit window*/
		meter_update_enjoyment(&model->meter, -MISS_PENALTY_ENJOYMENT); /*applies miss penalty*/
		meter_reset_multiplier(&model->meter);							/*sends score multiplier back to 1*/
		model_advance_head(model);										/*advances index*/
	}
}

/*------ Function: cond_game_over ------
PURPOSE: Checks if game is over (enjoyment meter has gone to 0)

INPUT: pointer to Model

OUTPUT: N/A
*/
UINT8 cond_game_over(const Model *model)
{
	int no_enjoyment = model->meter.enjoyment <= 0;
	int song_ended = model->head >= model->note_count;
	return (UINT8)no_enjoyment || song_ended;
}
