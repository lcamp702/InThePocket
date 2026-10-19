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

/*----- Function: plot_rectangle -----

 PURPOSE: Plots a rectangle on the screen given by the top left pixel, and the length and width of the rectangle.

 INPUT: Address(UINT32*): to the start of the screen
		Position(row,col): the coordinates of the top left pixel of the rectangle
		Length: the length (number of rows) in pixels of the rectangle
		Width: the width (number of columns) in pixels of the rectangle

 OUTPUT: None
*/
void plot_rectangle(UINT32 *base, int row, int col, UINT16 length, UINT16 width)
{
	plot_horizontal_line(base, row, col, width);
	plot_vertical_line(base, row, col, length);
	plot_horizontal_line(base, row, col + width, width);
	plot_vertical_line(base, row + length, col, length);
}

/*----- Function: plot_square -----

 PURPOSE: Plots a square on the screen given by the top left pixel, and the length of the sides of the square.

 INPUT: Address(UINT32*): to the start of the screen
		Position(row,col): the coordinates of the top left pixel of the square
		Side: the length of each side, in pixels, of the square

 OUTPUT: None
*/
void plot_square(UINT32 *base, int row, int col, UINT16 side)
{
	plot_horizontal_line(base, row, col, side);		   /*draw top side in square*/
	plot_vertical_line(base, row, col, side);		   /*draw left side of square*/
	plot_horizontal_line(base, row, col + side, side); /*draw bottom side of square*/
	plot_vertical_line(base, row + side, col, side);   /*draw right side of square*/
}

/*----- Function: plot_triangle -----

 PURPOSE: Plots a triangle on the screen given by the coordinate of the 90° angle, the length of the base, the length of the height, and the direction of the triangle.

 INPUT: Address(UINT32*): to the start of the screen
		Position(row,col): the coordinates of the pixel of the 90° angle of the triangle
		Base: the length (number of columns) of the base in pixels of the triangle (must be positive)
		Height: the length (number of rows) of the height in pixels of the triangle (must be positive)
		Direction: Describes where the coordinate is relative to the rest of the triangle
			  0 - Coordinate is the top left point of the triangle
			  1 - Coordinate is the top right point of the triangle
			  2 - Coordinate is the bottom left point of the triangle
			  3 - Coordinate is the bottom right point of the triangle


 OUTPUT: None
*/
#define	TRIANGLE_ROW_DIR 1
#define TRIANGLE_COL_DIR 2
void plot_triangle(UINT32 *base, int row, int col, UINT16 triangle_base, UINT16 height, UINT8 direction)
{
	/*
		0 (00) => dr > 0, dc > 0
		1 (01)=> dr > 0, dc < 0
		2 (10)=> dr < 0, dc > 0
		3 (11)=> dr < 0, dc < 0
		Left bit is dr (0 => base>0, 1 => dr<0)
		Right bit is dc (0 => dc>0, 1 => dc<0)
	*/

	int end_row;
	int end_col;

	if (direction & TRIANGLE_ROW_DIR)
		height = -height;				/* Triangle going down */

	if (direction & TRIANGLE_COL_DIR)
		triangle_base = -triangle_base;	/* Triangle going left */

	/* Draw lines from corner */
	plot_horizontal_line(base, row, col, triangle_base);
	plot_vertical_line(base, row, col, height);

	/* Draw hypotenuse */
	end_row = row + height;
	end_col = col + triangle_base;
	plot_line(base, row, col, end_row, end_col);
}

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