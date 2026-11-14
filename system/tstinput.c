/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Test driver for the input module.
	input/tstinput.c
*/

#include <stdio.h>
#include "input.h"

int main(void)
{
    while(!has_input)
        ;
    printf("Input: %c\n", get_input());
    return 0;
}