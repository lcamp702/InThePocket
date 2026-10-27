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
    model/tst_model.c
*/

#include "../events/events.h"
#include <osbind.h>
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
	/*test model_init*/
	
	printf("Press any key to proceed with model_init test\n");
	fflush(stdout);
	Cconin();

	model_init(&test_model, test_song, TEST_SONG_LEN);
	print_model(&test_model);
	/*synch_clock_tick test
	*  
	TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
	1		Synch clock should		Model w/ test	Clock advances, 		As expected
			advance clock, move		song			notes position changes
			notes, and move head					and the head moves
			past moved notes.						passed other notes
	*/
	printf("Press any key to proceed with synch_clock_tick test\n");
	fflush(stdout);
	Cconin();
	for(i = 0; i<4; i++){
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		print_model(&test_model);
	}
	
	/*advance head test
	TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
	2		Need to check that		Model w/		Head advances			Head advances
			advance head works		test song
	*/
	printf("Press any key to proceed with model_advance_head test\n");
	fflush(stdout);
	Cconin();

	print_model(&test_model);
	for(i = 0; i<5; i++){
		model_advance_head(&test_model);
		printf("--ADVANCE HEAD--\n");
		print_model(&test_model);
	}

	model_init(&test_model, test_song, TEST_SONG_LEN);
	
	/*cond_game_over test*/
	printf("Press any key to proceed with cond_game_over test\n");
	fflush(stdout);
	Cconin();

	for(i = 0; i<145; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<12; i++){
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		print_model(&test_model);
		printf("game over = %d \n", cond_game_over(&test_model));
	}


	/*cond_game_over (enjoyment = 40) test
	TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
	3		Checks that cond		Model w/ test	cond_game_over		as expected		
			is_game_over returns	song			returns 1 when
			1 when enjoyment						enjoyment goes below
			goes below 1.							0
	*/
	printf("Press any key to proceed with cond_game_over (enjoyment = 40) test\n");
	fflush(stdout);
	Cconin();

	model_init(&test_model, test_song, TEST_SONG_LEN);
	test_model.meter.enjoyment=40;
	for(i = 0; i<145; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<12; i++){
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		print_model(&test_model);
		printf("game over = %d \n", cond_game_over(&test_model));
	}
	
	/*cond_is_song_over test
	TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
	4		check is_song_over		see above		is_song_over			is_song_over
			returns 1 when							returns 1 when			returns 1 when
			head advances past						head advances past		head advances past
			or to song length						or to song length		or to song length
	*/
	
	printf("Press any key to proceed with cond_is_song_over test\n");
	fflush(stdout);
	Cconin();

	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<145; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<100; i++){
		synch_clock_tick(&test_model);
		test_model.meter.enjoyment=64;
		printf("-- TICK -- \n");
		print_model(&test_model);
		printf("song over = %d \n", cond_game_over(&test_model));
	}

	/*Test that key inputs yield correct pitch values
	TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
	5		check that key value	model w/ test	correct translation		as expected
			results in correct		_song			from char to int value
			number being returned 
			from  
			asynch_key_to_pitch
	*/
	printf("Press any key to proceed with asynch_key_to_pitch test\n");
	fflush(stdout);
	Cconin();

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
	printf("Press any key to proceed with asynch_is_hit (pitch 0) test\n");
	fflush(stdout);
	Cconin();

	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<138; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<30; i++){
		test_model.meter.enjoyment=64;
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		printf("hitting a yields %d \n", asynch_is_hit(&test_model, 0));
		print_note(&test_model.notes[test_model.head]);
	}
	/*asynch_is_hit (pitch 1) test
	TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
	6		checks that the			as above		asynch_is_hit			asynch_is_hit
			corresponding							returns 1				returns 1
			pitch value
			returns 1 from
			asynch_is_hit
	*/
	printf("Press any key to proceed with asynch_is_hit (pitch 1) test\n");
	fflush(stdout);
	Cconin();

	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<138; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<30; i++){
		test_model.meter.enjoyment=64;
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		printf("hitting s yields %d \n", asynch_is_hit(&test_model, 1));
		print_note(&test_model.notes[test_model.head]);
	}
	/*asynch_is_hit (pitch 2) test*/
	printf("Press any key to proceed with asynch_is_hit (pitch 2) test\n");
	fflush(stdout);
	Cconin();

	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<138; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<30; i++){
		test_model.meter.enjoyment=64;
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		printf("hitting d yields %d \n", asynch_is_hit(&test_model, 2));
		print_note(&test_model.notes[test_model.head]);
	}
	/*asynch_is_hit (pitch 3) test*/
	printf("Press any key to proceed with asynch_is_hit (pitch 3) test\n");
	fflush(stdout);
	Cconin();

	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<138; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<30; i++){
		test_model.meter.enjoyment=64;
		synch_clock_tick(&test_model);
		printf("-- TICK -- \n");
		printf("hitting f yields %d \n", asynch_is_hit(&test_model, 3));
		print_note(&test_model.notes[test_model.head]);
	}
	
	/*asynch_apply_hit test
	TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
	7		checks that				as above		it works				it does 
			asynch_applies_hit
			works and increases
			enjoyment
	*/
	printf("Press any key to proceed with asynch_apply_hit test\n");
	fflush(stdout);
	Cconin();

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

	/*asynch_apply_miss test
	
	TEST #	REASON					TEST DATA		EXPECTED RESULT			ACTUAL RESULT
	8		checks that				as above		enjoyment goes to 0		as expected
			asynch apply miss
			drains enjoyment
	*/
	
	printf("Press any key to proceed with asynch_apply_miss test\n");
	fflush(stdout);
	Cconin();

	model_init(&test_model, test_song, TEST_SONG_LEN);
	for(i = 0; i<133; i++){
		synch_clock_tick(&test_model);
	}
	for(i = 0; i<5; i++){
		synch_clock_tick(&test_model);
		if(!asynch_is_hit(&test_model, 3)){
			printf("applying miss, meter now gives ");
			print_meter(&test_model.meter);
			asynch_apply_miss(&test_model);
		}
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