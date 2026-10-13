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
#define WORD_COLS 40
#define LONG_COLS 20
#define ROWS 400
#define LONG_LENGTH 32
#define SCREEN_LONGS 8000

#define FONT_HEIGHT 16

/*----- Function: clear_screen -----

 PURPOSE: Clears the entire screen.

 INPUT: Address(UINT32*): to the start of the screen

 OUTPUT: None

*/
/*
void clear_screen(UINT32 *base)
{
	register int l;

	while (l++ < SCREEN_LONGS)
		*(base++) = 0;
}
*/

/*----- Function: clear_region -----

 PURPOSE: Clear a region of the screen. The section is specified by the coordinates of the top left corner, and the height and width of the region.

 INPUT: Address(UINT32*): to the start of the screen
	   Position(row,col): the coordinates of the top left pixel of the region
	   Length: the length (number of rows) in pixels of the region
	   Width: the width (number of columns) in pixels of the region

 OUTPUT: None

*/
void clear_region(UINT32 *base, int row, int col, UINT16 length, UINT16 width)
{

	int base_offset;
	int start_offset;
	int end_offset;
	int start_bit;
	int end_bit;
	int fills;
	UINT32 start_mask;
	UINT32 end_mask;
	UINT32 *cursor;

	/* Fast bound checking [1] */
	if ((UINT16)row >= ROWS) return;
	if ((UINT16)col >= COLS) return;
	if (length <= 0) return;
	if (width <= 0) return;

	/* Length Clipping */
	base_offset = row;
	base_offset += length;
	if (base_offset > ROWS)
	{
		length = ROWS;
		length -= row;

		base_offset = ROWS;
	}
	base_offset--;	
	/* Base offset points to start of the last row to draw */

	/* Set start offset */
	start_offset = col;
	start_bit = start_offset;

	start_offset >>= 5;

	/* Set end offset*/
	end_offset = col + width - 1;

	/* Width Clipping */
	if (end_offset >= COLS)
	{
		end_offset = COLS;
		end_offset--;
	}
	/* Copy to get remainder */
	end_bit = end_offset;

	end_offset >>= 5;

	/* Get shift amounts for LSR / LSL */
	start_bit &= 31;
	start_bit = -start_bit;
	start_bit += 32;

	end_bit &= 31;
	end_bit += 1;

	/* Create masks */
	if (start_bit == 32)
		start_mask = 0;
	else
		start_mask = 0xFFFFFFFF << start_bit;

	if (end_bit == 32)
		end_mask = 0;
	else
		end_mask = 0xFFFFFFFF >> end_bit;

	/* Move base pointer */
	base_offset *= LONG_COLS;
	base_offset += start_offset;
	base += base_offset;

	if (start_offset == end_offset)
	{
		/* Mask fits in one long */
		start_mask |= end_mask;
		while (length-- > 0) {
			*base &= start_mask;
			base -= LONG_COLS;
		}
	}
	else
	{
		while (length-- > 0) {
			cursor = base;

			/* Mask spans multiple longs */
			*(cursor++) &= start_mask;

			fills = end_offset - start_offset - 1;
			while (--fills >= 0)
			{
				*(cursor++) = 0;
			}

			*cursor &= end_mask;

			base -= LONG_COLS;
		}

	}
}

/*----- Function: plot_pixel -----

 PURPOSE: Plots a single pixel on the screen.

 INPUT: Address(UINT8*): to the start of the screen
		Position(row,col): the location of the pixel to plot

 OUTPUT: None

*/
/*
void plot_pixel(UINT8 *base, int row, int col)
{
	   register UINT8 rem;
	   register UINT8 mask;

	   (Fast bound checking [1])
	   if ((UINT16)row >= ROWS) return;
	   if ((UINT16)col >= COLS) return;

	   base += row * BYTE_COLS;
	   base += col >> 3;

	   rem = col & 7;
	   mask = 1 << (7-rem);

	   *base |= mask;
}
*/

/*----- Function: plot_horizontal_line -----

 PURPOSE: Plot a horizontal line on the screen. The horizontal line is specified by the leftmost pixel of the line and the length of the line.

 INPUT: Address(UINT32*): to the start of the screen
		Position(row,col): the coordinates of the leftmost pixel of the horizontal line
		Length: the length in pixels of the line

 OUTPUT: None
*/
void plot_horizontal_line(UINT32 *base, int row, int col, UINT16 length)
{
	int base_offset;
	int start_offset;
	int end_offset;
	int start_bit;
	int end_bit;
	int fills;
	UINT32 start_mask;
	UINT32 end_mask;

	/* Fast bound checking [1] */
	if ((UINT16)row >= ROWS) return;
	if ((UINT16)col >= COLS) return;
	if (length <= 0) return;

	/* Set start offset */
	start_offset = col;
	start_bit = start_offset;

	start_offset >>= 5;

	/* Set end offset*/
	end_offset = col + length - 1;

	/* Clipping */
	if (end_offset >= COLS)
	{
		end_offset = COLS;
		end_offset -= 1;
	}
	/* Copy to get remainder */
	end_bit = end_offset;

	end_offset >>= 5;

	/* Get shift amounts for LSR / LSL */
	start_bit &= 31;
	end_bit = ~end_bit;
	end_bit &= 0x1F; /* end_bit = 31 - (end_col % 32)*/
					 /* Like [2]. In assembly this is fastest with not and andi */

	/* Create masks */
	start_mask = 0xFFFFFFFF >> start_bit;
	end_mask = 0xFFFFFFFF << end_bit;

	/* Move base pointer */
	base_offset = row;
	base_offset *= LONG_COLS;
	base_offset += start_offset;
	base += base_offset;

	if (start_offset == end_offset)
	{
		/* Line fits in one long */
		start_mask &= end_mask;
		*base |= start_mask;
	}
	else
	{
		/* Line spans multiple longs */
		*(base++) |= start_mask;

		fills = end_offset - start_offset - 1;
		while (--fills >= 0)
		{
			*(base++) = -1;
		}

		*base |= end_mask;
	}
}

/*----- Function: plot_vertical_line -----

 PURPOSE: Plot a vertical line on the screen. The vertical line is specified by the topmost pixel of the line and the length of the line.

 INPUT: Address(UINT32*): to the start of the screen
		Position(row,col): the coordinates of the topmost pixel of the vertical line
		Length: the length in pixels of the line

 OUTPUT: None
*/
/*
void plot_vertical_line(UINT32 *base, int row, int col, UINT16 length)
{
	   int end;
	   ( int lc = LONG_COLS;   In ASM loading this into a register will save clocks for any length >= 1. )
	   int r_offset;
	   int c_offset;
	   int col_bit;
	   UINT32 *cursor;
	   UINT32 mask;
	   int l;

	   ( Bound checking )
	   if (row < 0 || row >= ROWS) return;
	   if (col < 0 || col >= COLS) return;
	   if (length <= 0) return;

	   ( Clipping )
	   l = length;
	   end = row;
	   end += length;
	   if (end > ROWS) {
			  l = ROWS;
			  l -= row;
	   }

	   ( Calculate offsets )
	   r_offset = row;
	   r_offset *= LONG_COLS;

	   c_offset = col;
	   col_bit = c_offset;
	   c_offset >>= 5;

	   ( Optimization through not [2] )
	   col_bit = ~col_bit;
	   col_bit &= 0x1F;

	   ( Move cursor pointer )
	   cursor = base;
	   cursor += r_offset;
	   cursor += c_offset;

	   ( In assembly this will take as long as subtracting to get bit position from the right )
	   mask = 1;
	   mask <<= col_bit;

	   while(--l >= 0) {
			  *(cursor) |= mask;
			  cursor += LONG_COLS;
	   }
}
*/

/*----- Function: plot_line -----
 PURPOSE: Plots a line on the screen between the two given points.

 INPUT: Address(UINT32*): to the start of the screen
		Position(start_row,start_col): the coordinates of the start of the line
		Position(end_row,end_col): the coordinates of the end of the line

 OUTPUT: None
*/
void plot_line(UINT32 *base, int start_row, int start_col, int end_row, int end_col)
{
	if(start_col==end_col && start_row == end_row){
		plot_pixel(base, start_row, start_col); /*if "line" is just one pixel then call plot pixel*/
	}
	else if(start_row==end_row){ /* call plot_horizontal_line if line is on one row*/
		if(end_col>start_col){
			plot_horizontal_line(base, start_row, start_col, end_col-start_col);
		}
		else {
			plot_horizontal_line(base, end_row, start_col, start_col-end_col);
		}
	}
	else if(start_col==end_col){ /*call plot vertical line if the line if start and end col are the same*/
		if(end_row>start_row){
			plot_vertical_line(base, start_row, start_col, end_col-start_col);
		}
		else {
			plot_vertical_line(base, start_row, start_col, start_col-end_col);
		}
	}
	else{
		/*TODO How to plot weird coordinates (like what if it goes 1 unit horizontal and 15 units up? How to plot?*/
	}
}

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
	plot vertical line(base, row, col, length);
	plot_horizontal_line(base, row, col+width, width);
	plot_vertical_line(base, row+length, col, length);
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
	plot_horizontal_line(base, row, col, length); /*draw top side in square*/
	plot vertical line(base, row, col, length); /*draw left side of square*/
	plot_horizontal_line(base, row, col+length, length); /*draw bottom side of square*/
	plot_vertical_line(base, row+length, col, length); /*draw right side of square*/
}

/*----- Function: plot_triangle -----

 PURPOSE: Plots a triangle on the screen given by the coordinate of the 90° angle, the length of the base, the length of the height, and the direction of the triangle.

 INPUT: Address(UINT32*): to the start of the screen
		Position(row,col): the coordinates of the pixel of the 90° angle of the triangle
		Base: the length (number of columns) of the base in pixels of the triangle
		Height: the length (number of rows) of the height in pixels of the triangle
		Direction: Describes where the coordinate is relative to the rest of the triangle
			  0 - Coordinate is the top left point of the triangle
			  1 - Coordinate is the top right point of the triangle
			  2 - Coordinate is the bottom left point of the triangle
			  3 - Coordinate is the bottom right point of the triangle


 OUTPUT: None
*/
void plot_triangle(UINT32 *base, int row, int col, UINT16 triangle_base, UINT16 height, UINT8 direction)
{
}

/*----- Function: plot_8bit_bitmap -----

 PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.

 INPUT: Address(UINT8*): to the start of the screen
		Position(row,col): the coordinates of the top left pixel of the bitmap
		Height: the length (number of rows) of the height in pixels of the bitmap

 OUTPUT: None
*/
void plot_8bit_bitmap(UINT8 *base, int row, int col, const UINT8 *bitmap, UINT16 height)
{
}

/*----- Function: plot_16bit_bitmap -----

 PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.

 INPUT: Address(UINT16*): to the start of the screen
		Position(row,col): the coordinates of the top left pixel of the bitmap
		Height: the length (number of rows) of the height in pixels of the bitmap

 OUTPUT: None
*/
void plot_16bit_bitmap(UINT16 *base, int row, int col, const UINT16 *bitmap, UINT16 height)
{
}

/*----- Function: plot_32bit_bitmap -----

 PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.

 INPUT: Address(UINT32*): to the start of the screen
		Position(row,col): the coordinates of the top left pixel of the bitmap
		Height: the length (number of rows) of the height in pixels of the bitmap

 OUTPUT: None
*/
void plot_32bit_bitmap(UINT32 *base, int row, int col, const UINT32 *bitmap, UINT16 height)
{
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
	if ((UINT16)row >= max_row) return;
	if ((UINT16)col >= max_col) return;

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
			*(base + 1) |= letter << (7 - offset);

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