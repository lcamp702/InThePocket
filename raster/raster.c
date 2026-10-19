/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026
	raster.c

	Status:
	  Not Tested:
		plot_line
		plot_rectangle
		plot_square
		plot_triangle
		plot_8bit_bitmap
		plot_16bit_bitmap
		plot_32bit_bitmap
	  Visual Tests:
		clear_screen
		clear_region
		plot_pixel
		plot_horizontal_line
		plot_vertical_line
		plot_character
		plot_string

	Credits:

	[1] D. Salvato, “Assembly tricks for fast bounds checking,” @dansalvato, https://dansalva.to/assembly-tricks-for-fast-bounds-checking/ (Accessed Oct. 13, 2026).
	[2] StingRay, "Plotter / Dot Flag," English Amiga Board, https://eab.abime.net/showthread.php?t=50595 (Accessed Oct. 13 2026)

	Purpose:
	A raster engine for our game, In the Pocket on the Atari ST.

*/

#include <linea.h>
#include "raster.h"

#define COLS 640
#define BYTE_COLS 80
#define ROWS 400

#define FONT_HEIGHT 16

/*----- Function: plot_character -----

 PURPOSE: Plots a single character, as a bitmap from a font table, to the screen.

 INPUT: Address(UINT8*): to the start of the screen
		Position(row,col): the coordinates of the top left pixel of the character
		ch(char): the character to be written to the screen

 OUTPUT: None
*/
void plot_character(UINT8 *base, int row, int col, char ch)
{
	char *font;			/* Base of font table */
	register int l = 0; /* Loop counter for printing rows */
	int offset;			/* Column remainder after finding byte*/
	int max_row;
	int max_col;

	/* Fast bound checking [1] */
	max_row = ROWS - FONT_HEIGHT;
	max_col = COLS - 8;
	if ((UINT16)row > max_row)
		return;
	if ((UINT16)col > max_col)
		return;

	linea0();				 /* Initialize line-a variables */
	font = (char *)V_FNT_AD; /* Get start address of font table */

	base += row * BYTE_COLS; /* Move base pointer to the starting row. */
	base += col >> 3;		 /* Move base pointer to the starting byte. */
	offset = col & 7;		 /* Column offset amount for the letter bitmap */

	while (l < FONT_HEIGHT)
	{
		UINT8 letter = *(font + ch + (l << 8)); /* Current bitmap row of the letter being printed */

		*(base) |= letter >> offset; /* Shift if needed and mask */

		if (offset != 0) /* If the bitmap was shifted, part will be in the next byte. */
			*(base + 1) |= letter << (8 - offset);

		base += BYTE_COLS; /* Next row */
		l++;			   /* Increment row count */
	}
}

/*----- Function: plot_string -----

 PURPOSE: Plots a string, as a sequence of bitmaps from a font table, to the screen.

 INPUT: Address(UINT8*): to the start of the screen
		Position(row,col): the coordinates of the top left pixel of the string
		ch(c-string): the string to be written to the screen

 OUTPUT: None
*/
void plot_string(UINT8 *base, int row, int col, char *ch)
{
	/* Bounds are checked in plot_character*/
	while (*ch != '\0')
	{ /* Loop over string until null terminator. */
		plot_character(base, row, col, *ch);

		/* Increment by 8 columns (1 byte in memory)*/
		col += 8;
		/* Get next character */
		ch++;

		if (col > (COLS - 8)) /* Check for the end of the row */
		{
			row += FONT_HEIGHT;
			col = 0;
		}
	}
}