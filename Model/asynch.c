/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the asynch.c file. Contains function definitions for the functions in asynch.h.
    asynch.c*/

#include "asynch.h"
#include "cond.h"

UINT8 asynch_key_to_pitch(char key){
	
	UINT8 pitch=4;
	
	if(key=='a' || key=='A'){
		pitch=KEY_A;
	}
	else if(key=='s'|| key=='S'){
		pitch=KEY_S;
	}
	else if(key=='d'|| key=='D'){
		pitch=KEY_D;
	}
	else if(key=='f'|| key=='F'){
		pitch=KEY_F;
	}
	
	return pitch;
}
UINT8 asynch_is_hit(const Model *model, UINT8 pitch){
	
	UINT8 is_hit;

	if(model->head<model->note_count && model->notes[model->head].pitch==pitch && model->game_clock>=(model->notes[model->head].time+TRAVEL_TICKS)){
		is_hit=1;
	}
	else{
		is_hit=0;
	}

	return is_hit;
}
