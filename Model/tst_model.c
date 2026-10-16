/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Test driver for the game model. Sets up model states, calls the event handlers, and prints every model property after each call.
    tst_model.c*/

#include "events.h"
#include <stdio.h>
static const Note test_song[] = {
	{ 0, 0, 0 },
	{ 1, 0, 1 },
	{ 3, 0, 2 },
	{ 10, 0, 3 }
};

#define TEST_SONG_LEN (sizeof(test_song)/sizeof(test_song[0]))

void print_model(const Model * model);
void print_note(const Note * note);
void print_meter(const Meter * meter);

int main(void){

	UINT8 i;
	static Model test_model;
	
	model_init(&test_model, test_song, TEST_SONG_LEN);
	print_model(&test_model);
	/*synch_clock_tick test
	* Synch clock should advance clock, move notes, and move head past moved notes. 
	*/
	for(i = 0; i<4; i++){
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		print_model(&test_model);
	}
	/*advance head test*/
	for(i = 0; i<5; i++){
		model_advance_head(&test_model);
		printf("--ADVANCE HEAD--\n");
		print_model(&test_model);
	}

	model_init(&test_model, test_song, TEST_SONG_LEN);
	/*cond_is_game_over test*/
	for(i = 0; i<145; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<12; i++){
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		print_model(&test_model);
		printf("game over = %d", cond_is_game_over(&test_model));
	}
	
	model_init(&test_model, test_song, TEST_SONG_LEN);
	test_model.meter.enjoyment=40;
	for(i = 0; i<145; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<12; i++){
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		print_model(&test_model);
		printf("game over = %d", cond_is_game_over(&test_model));
	}
	
	/*cond_is_song_over test*/
	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<145; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<100; i++){
		synch_clock_tick(&test_model);
		test_model.meter.enjoyment=100;
		printf("-- TICK -- \n");
		print_model(&test_model);
		printf("song over = %d", cond_is_song_over(&test_model));
	}

	/*Test that key inputs yield correct pitch values*/
	printf("inputing a yields pitch value %d \n", asynch_key_to_pitch('a'));
	printf("inputing s yields pitch value %d \n", asynch_key_to_pitch('s'));
	printf("inputing d yields pitch value %d \n", asynch_key_to_pitch('d'));
    printf("inputing f yields pitch value %d \n", asynch_key_to_pitch('f'));
	printf("inputing A(caps) yields pitch value %d \n", asynch_key_to_pitch('A'));
	printf("inputing S(caps) yields pitch value %d \n", asynch_key_to_pitch('S'));
	printf("inputing D(caps) yields pitch value %d \n", asynch_key_to_pitch('D'));
    printf("inputing F(caps) yields pitch value %d \n", asynch_key_to_pitch('F'));
	printf("inputing c yields pitch value %d \n", asynch_key_to_pitch('c'));
	/*testing async_is_hit*/
	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<138; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<30; i++){
		test_model.meter.enjoyment=100;
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		printf("hitting a yields %d \n", asynch_is_hit(&test_model, 0));
		print_note(&test_model.notes[test_model.head]);
	}
	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<138; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<30; i++){
		test_model.meter.enjoyment=100;
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		printf("hitting a yields %d \n", asynch_is_hit(&test_model, 1));
		print_note(&test_model.notes[test_model.head]);
	}
	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<138; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<30; i++){
		test_model.meter.enjoyment=100;
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		printf("hitting a yields %d \n", asynch_is_hit(&test_model, 2));
		print_note(&test_model.notes[test_model.head]);
	}
	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<138; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<30; i++){
		test_model.meter.enjoyment=100;
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		printf("hitting a yields %d \n", asynch_is_hit(&test_model, 3));
		print_note(&test_model.notes[test_model.head]);
	}
	
	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<138; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<30; i++){
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		if( asynch_is_hit(&test_model, 0) || asynch_is_hit(&test_model, 1) || asynch_is_hit(&test_model, 2) || asynch_is_hit(&test_model, 3) ){
			asynch_apply_hit(&test_model);
			printf("hit applied, meter now gives ");
			print_meter(&test_model.meter);
		}
	}




	return 0;

}


void print_note(const Note * note){
	printf("time = %lu , x = %d, pitch = %d \n", note->time, note->x, note->pitch);
}

void print_meter(const Meter * meter){
	printf("enjoyment =  %d, multiplier = %d, score = %lu \n", meter->enjoyment, meter->multiplier, meter->score);
}

void print_model(const Model * model){
	
	UINT16 i;

	printf("clock = %lu, note count = %d, head = %d , last on screen = %d \n",
		 model->game_clock,
		 model->note_count,
		 model->head,
		 model_last_on_screen(model)
	);

	print_meter(&model->meter);
	for(i=0; i<model->note_count; i++) {
		printf("note %d : ", i);
		print_note(&model->notes[i]);
	}

}