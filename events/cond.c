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
		if (model->notes[model->head].hit == 0)
		{
			meter_update_enjoyment(&model->meter, -MISS_PENALTY_ENJOYMENT); /*applies miss penalty*/
			meter_reset_multiplier(&model->meter);							/*sends score multiplier back to 1*/
		}
		model_advance_head(model);										/*advances index*/
	}
}

/* ==== Function: cond_game_over ====
PURPOSE: Checks if game is over (enjoyment meter has gone to 0, quit key input flag was set)

INPUT: pointer to Model

OUTPUT: N/A
*/
UINT8 cond_game_over(const Model *model)
{	
	UINT8 quit_flag = model->quit_request_buff;
	bool no_enjoyment = model->meter.enjoyment <= 0;
	bool song_ended = model->head >= model->note_count;
	return (UINT8)no_enjoyment || song_ended || quit_flag;
}

/* ==== Function: cond_is_hit ====
PURPOSE: Checks if the note at the offset or its neighbour can be hit.

INPUT: Pointer to model
		Pitch of keypress
		Head offset to check

OUTPUT: Is hit? -1: No 0-3: Head offset of note hit
*/
UINT8 cond_is_hit(Model *model, UINT8 pitch, UINT8 head_offset)
{
	Note note = model->notes[model->head + head_offset];

	/* Check if note can be hit based on time */
	UINT32 hit_time = (note.time + TRAVEL_TICKS);

	UINT8 can_hit = model->game_clock >= hit_time;
	can_hit &= model->game_clock < (hit_time + HIT_WINDOW);

	if (can_hit && note.pitch == pitch)
		return head_offset;
	else if (model->notes[model->head + head_offset].neighbour)
		return cond_is_hit(model, pitch, head_offset+1);
	else
		return -1;
}

/* ==== Function: cond_apply_hit ====
PURPOSE: Handles hit event.

INPUT: pointer to Model

OUTPUT: N/A
*/
void cond_apply_hit(Model *model, UINT8 head_offset)
{
	meter_update_enjoyment(&model->meter, HIT_REWARD_ENJOYMENT); /*adds to enjoyment meter*/
	meter_update_score(&model->meter, HIT_REWARD_SCORE);		 /*updates score*/
	meter_increase_multiplier(&model->meter);					 /*double multiplier*/
	if (head_offset == 0)
		model_advance_head(model);								/*advance head*/
	else
		model->notes[model->head + head_offset].hit = 1;
}

/* ==== Function: cond_apply_miss ====
PURPOSE: Handles miss event.

INPUT: pointer to Model.

OUTPUT: N/A
*/
void cond_apply_miss(Model *model)
{
	meter_update_enjoyment(&model->meter, -MISS_PENALTY_ENJOYMENT); /*applies miss penalty*/
	meter_reset_multiplier(&model->meter);							/*reset the score multiplier to 1*/
}

/* ==== Function: cond_check_key_buffer ====
PURPOSE: checks key buffer and triggers appropriate events

INPUT: Pointer to game model

OUTPUT: N/A
*/
void cond_check_key_buffer(Model *model){
	UINT8 i, is_hit;

	for(i = 0; i<TARGET_COUNT; i++){
		if(model->key_press_buff[i]){
			model->targets[i]=1;
			if(SONG_PLAYING){
				is_hit=cond_is_hit(model, i, 0); /*sets a flag for if a note has been hit*/
				if(is_hit==(UINT8)(-1)){ /*check if flag is miss value */
					cond_apply_miss(model); 
				}
				else{ /*applies appropriate hit*/
					cond_apply_hit(model, is_hit);
				}
			}
		}
	}
	model_clear_key_buff(model);
}
