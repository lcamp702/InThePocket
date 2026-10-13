#include <linea.h>
#include "raster.h"

#define COLS 640
#define BYTE_COLS 80
#define WORD_COLS 40
#define LONG_COLS 20
#define ROWS 400

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

       if (row < 0 || row >= ROWS) return;
       if (col < 0 || col >= COLS) return;
       
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
       int col_rem;
       int row_offset;
       int col_offset;
       int end_rem;
       int fills;
       UINT32 start_mask;
       UINT32 end_mask;

       /* Bound check */
       if (row < 0 || row >= ROWS) return;
       if (col < 0 || col >= COLS) return;

       row_offset = row;
       row_offset *= LONG_COLS;

       base += row_offset;

       col_offset = col;
       col_offset >>= 5;

       base += col_offset;

       col_rem = col;
       col_rem &= 31;

       fills = length;
       fills -= 31;
       fills += col_rem;    /* fills = length - start*/
       end_rem = fills;

       if (fills < 0) return;

       fills >>= 5;          /* fills = # of long fills */

       end_rem &= 31;       /* end_rem = # of pixels to fill at the end*/

       start_mask = -1;
       start_mask >>= col_rem;

       *(base++) |= start_mask;

       end_mask = -1;
       end_mask >>= end_rem;
       end_mask = ~end_mask;
       
       while (--fills >= 0) {
              *(base++) = -1;
       }

       *(base) |= end_mask;
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
       int p_offset;
       UINT32 *cursor;
       UINT32 mask;
       int l;

       ( Bound checking )
       if (row < 0)  return;
       if (row >= ROWS) return;
       if (col < 0) return;
       if (col >= COLS) return;
       if (length == 0) return;

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
       p_offset = c_offset;
       c_offset >>= 5;

       p_offset &= 31;

       ( Move cursor pointer )
       cursor = base;
       cursor += r_offset;
       cursor += c_offset;

       ( In assembly this will take as long as subtracting to get bit position from the right )
       mask = 0x80000000;
       mask >>= p_offset;

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
    char *font; /* Base of font table */
    register int l = 0; /* Loop counter for printing rows */
    int offset; /* Column remainder after finding byte*/

    linea0();   /* Initialize line-a variables */
    font = (char *)V_FNT_AD;    /* Get start address of font table */

    base += row * BYTE_COLS;    /* Move base pointer to the starting row. */
    base += col >> 3;   /* Move base pointer to the starting byte. */
    offset = col & 7;   /* Column offset amount for the letter bitmap */

    while (l < FONT_HEIGHT)
    {
        UINT8 letter = *(font + ch + (l << 8)); /* Current bitmap row of the letter being printed */

        *(base) |=  letter >> offset;   /* Shift if needed and mask */

        if (offset != 0)    /* If the bitmap was shifted, part will be in the next byte. */
            *(base + 1) |= letter << (7 - offset); 

        base += BYTE_COLS; /* Next row */
        l++;    /* Increment row count */
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
    while (*ch != '\0') 
    {   /* Loop over string until null terminator. */
        plot_character(base, row, col, *ch);

        /* Increment by 8 columns (1 byte in memory)*/
        col+= 8;
        /* Get next character */
        ch++;
        
        if (col > (COLS - 8))   /* Check for the end of the row */
        {
            row += FONT_HEIGHT;
            col = 0;
        }
    }
}