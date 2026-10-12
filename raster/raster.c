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
    register int i;

    while (i++ < SCREEN_LONGS)
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

       if (row >= ROWS)
              return;

       if (col >= COLS)
              return;
       
       base += row * BYTE_COLS;
       base += col >> 3;

       rem = col & 7;
       mask = 1 << (7-rem);

       *base |= mask;
}
*/

/*----- Function: plot_horizontal_line -----

 PURPOSE: Plot a hoizontal line on the screen. The horizontal line is specified by the leftmost pixel of the line and the length of the line.

 INPUT: Address(UINT32*): to the start of the screen
        Position(row,col): the coordinates of the leftmost pixel of the horizontal line
        Length: the length in pixels of the line

 OUTPUT: None
*/
void plot_horizontal_line(UINT32 *base, int row, int col, UINT16 length)
{
}

/*----- Function: plot_vertical_line -----

 PURPOSE: Plot a hoizontal line on the screen. The vertical line is specified by the topmost pixel of the line and the length of the line.

 INPUT: Address(UINT32*): to the start of the screen
        Position(row,col): the coordinates of the topmost pixel of the vertical line
        Length: the length in pixels of the line

 OUTPUT: None
*/
void plot_vertical_line(UINT32 *base, int row, int col, UINT16 length)
{
}

/*----- Function: plot_line -----

 PURPOSE: Plots a line on the screen between the two given points.

 INPUT: Address(UINT32*): to the start of the screen
        Position(start_row,start_col): the coordinates of the start of the line
        Position(end_row,end_col): the coordinates of the end of the line

 OUTPUT: None
*/
void plot_line(UINT32 *base, int start_row, int start_col, int end_row, int end_col)
{
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
    register int i = 0; /* Loop counter for printing rows */
    int offset; /* Column remainder after finding byte*/

    linea0();   /* Initialize line-a variables */
    font = (char *)V_FNT_AD;    /* Get start address of font table */

    base += row * BYTE_COLS;    /* Move base pointer to the starting row. */
    base += col >> 3;   /* Move base pointer to the starting byte. */
    offset = col & 7;   /* Column offset amount for the letter bitmap */

    while (i < FONT_HEIGHT)
    {
        UINT8 letter = *(font + ch + (i << 8)); /* Current bitmap row of the letter being printed */

        *(base) |=  letter >> offset;   /* Shift if needed and mask */

        if (offset != 0)    /* If the bitmap was shifted, part will be in the next byte. */
            *(base + 1) |= letter << (7 - offset); 

        base += BYTE_COLS; /* Next row */
        i++;    /* Increment row count */
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