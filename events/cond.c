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
#include "../model/model.h"

#define SONG_PLAYING model->head < model->note_count /* The head is not at the end of the song */
#define NOTE_PASSED model->game_clock > (model->notes[model->head].time + TRAVEL_TICKS + HIT_WINDOW) /* Game clock passed hit time */

/* ==== Function: cond_check_misses ====
PURPOSE: Checks to see if index points at a missed note, and if so updates the meter and repeats until head points at an "alive" note

INPUT: pointer to Model

OUTPUT: N/A
*/
void cond_check_misses(Model *model)
{
	while (SONG_PLAYING && NOTE_PASSED)
	{																	/*checks that the head is pointed at a note that has passed the hit window*/
		if (!model->notes[model->head].hit)
			meter_apply_miss(&model->meter);

		model_advance_head(model);										/*advances index*/
	}
}

/* ==== Function: cond_game_over ====
PURPOSE: Checks if game is over (enjoyment meter has gone to 0, quit key input flag was set)

INPUT: pointer to Model

OUTPUT: (bool) Game is over?
*/
bool cond_game_over(const Model *model)
{	
	UINT8 quit_flag = model->quit_request_buff;
	bool no_enjoyment = model->meter.enjoyment <= 0;
	bool song_ended = model->head >= model->note_count;
	return no_enjoyment || song_ended || quit_flag;
}

/* ==== Function: cond_check_key_buffer ====
PURPOSE: checks key buffer and triggers appropriate events

INPUT: Pointer to game model

OUTPUT: N/A
*/
void cond_check_key_buffer(Model *model){
	UINT8 i;

	for(i = 0; i<TARGET_COUNT; i++){
		if(model->key_press_buff[i] && SONG_PLAYING){
			model->targets[i] = true;
			
			if(note_is_hit(&model->notes[model->head], model->game_clock, model->last_interval, i))
			{
				meter_apply_hit(&model->meter);
				
				if(model->notes[model->head].hit)
					model_advance_head(model);
			}
			else
				meter_apply_miss(&model->meter);
		}
	}

	model_clear_key_buff(model);
}

