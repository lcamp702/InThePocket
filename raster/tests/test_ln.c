#include "tests.h"

void test_pixel()
{
	int c;
	int r;
	plot_string(b_fb, 0, 0, "Plot Pixel:");
	sync_wait(35);
	clear_screen(l_fb);
	for (r = -1; r < 401; r++)	/* Loop past boundary to test rejection. */
		for (c = -1; c < 641; c++)
			if ((r & 1) == (c & 1))
				plot_pixel(b_fb, r, c);
	sync_wait(70);
	clear_screen(l_fb);
}

void test_h_line()
{
	int t;
	int r;
	int c;
	int x;
	int row_num;
	int start_c;
	int tile_w[] = {1, 2, 4, 5, 8, 10, 16, 20, 40, 80};

	plot_string(b_fb, 0, 0, "Plot Horizontal Line:");
	sync_wait(35);
	clear_screen(l_fb);

	/* ==== Bound checking tests ==== */

	/* Invalid rows */
    plot_horizontal_line(l_fb, -1, 100, 50);
    plot_horizontal_line(l_fb, -32768, 200, 50);
    plot_horizontal_line(l_fb, ROWS, 300, 50);
    plot_horizontal_line(l_fb, ROWS + 100, 400, 50);
    plot_horizontal_line(l_fb, 32767, 500, 50);

    /* Invalid cols */
    plot_horizontal_line(l_fb, 10, -1, 50);
    plot_horizontal_line(l_fb, 20, -32768, 50);
    plot_horizontal_line(l_fb, 30, COLS, 50);
    plot_horizontal_line(l_fb, 40, COLS + 100, 50);
    plot_horizontal_line(l_fb, 50, 32767, 50);

    /* Invalid length */
    plot_horizontal_line(l_fb, 100, 0, 0);
    plot_horizontal_line(l_fb, 100, 320, 0);
    plot_horizontal_line(l_fb, 100, 639, 0);

    sync_wait(35);
    clear_screen(l_fb);

	/* ==== Start Clipping tests ==== */

	/* Touching left */
    plot_horizontal_line(l_fb, 20, 0, 320);
	/* Multi long clipping right */
    plot_horizontal_line(l_fb, 30, -1, 321);
    plot_horizontal_line(l_fb, 40, -320, 640);
    plot_horizontal_line(l_fb, 50, -2002, 2322);
	/* First long clipping */
	plot_horizontal_line(l_fb, 60, -16, 18);
	plot_horizontal_line(l_fb, 70, -30, 32);
    /* Single long clipping right */
    plot_horizontal_line(l_fb, 80, -32, 33);   /* Last pixel, exact */
    plot_horizontal_line(l_fb, 90, -33, 34); /* Last pixel, clipped */
    /* Large lengths (unsigned) */
    plot_horizontal_line(l_fb, 110, -320, 0x8000);
    plot_horizontal_line(l_fb, 120, -320, 0xC000);
    /* Large negative start */
    plot_horizontal_line(l_fb, 130, -32768, 0xFFFF);
	plot_horizontal_line(l_fb, 140, -32768, 0x8001);

    sync_wait(70);
    clear_screen(l_fb);

	/* ==== End Clipping tests ==== */

	/* Touching right */
    plot_horizontal_line(l_fb, 20, 320, 320);
	/* Multi long clipping right */
    plot_horizontal_line(l_fb, 30, 320, 321);
    plot_horizontal_line(l_fb, 40, 320, 500);
    plot_horizontal_line(l_fb, 50, 320, 2000);
	/* First long clipping */
	plot_horizontal_line(l_fb, 60, 608, 100);
	plot_horizontal_line(l_fb, 70, 620, 50);
    /* Single long clipping right */
    plot_horizontal_line(l_fb, 80, 639, 1);   /* Last pixel, exact */
    plot_horizontal_line(l_fb, 90, 639, 2); /* Last pixel, clipped */
    /* Large lengths (unsigned) */
    plot_horizontal_line(l_fb, 110, 320, 0x8000);
    plot_horizontal_line(l_fb, 120, 320, 0xC000);
    /* End pixel wraparound (c+length) */
    plot_horizontal_line(l_fb, 130, 320, 0xFFFF);
    plot_horizontal_line(l_fb, 140, 639, 0xFFFF);

    sync_wait(70);
    clear_screen(l_fb);

	/* ==== Valid line tests ==== */

	/* Top and bottom */
    plot_horizontal_line(l_fb, 0, 0, COLS);
    plot_horizontal_line(l_fb, ROWS - 1, 0, COLS);

    /* Plot pixels at word edges */
    plot_horizontal_line(l_fb, 10, 0, 1);
    plot_horizontal_line(l_fb, 12, 15, 1);
    plot_horizontal_line(l_fb, 14, 31, 1);
    plot_horizontal_line(l_fb, 16, 32, 1);
    plot_horizontal_line(l_fb, 18, 639, 1);

    /* Plot various lengths */
    for (r = 0; r < 80; r++)
    {
        plot_horizontal_line(l_fb, 50 + r, 16, r + 1);
    }

    /* Full longs */
    plot_horizontal_line(l_fb, 150, 32, 32);  /* 1 long */
    plot_horizontal_line(l_fb, 154, 32, 64);  /* 2 longs */
    plot_horizontal_line(l_fb, 158, 32, 96);  /* 3 longs */

    /* Multi long span */
    plot_horizontal_line(l_fb, 170, 31, 2); 
    plot_horizontal_line(l_fb, 174, 31, 34);

    sync_wait(70);
    clear_screen(l_fb);

	/* ==== Checkerboard ==== */
	for (t = 0; t < 10; t++)
	{
		int size = tile_w[t];
		for (r = 0; r < ROWS; r += size)
		{
			row_num = r / size;

			start_c = 0;
			if (row_num % 2 == 0)
				start_c = size;

			for (x = 0; x < size; x++)
			{
				for (c = start_c; c < COLS; c += size * 2)
				{
					plot_horizontal_line(l_fb, r+x, c, size);
				}
			}
		}
		sync_wait(35);
		clear_screen(l_fb);
	}
}

/*
plot_vertical_line Test Plan:

Screen #1: Invalid values
	Screen should appear blank for 0.5 sec.

1-5		|	Lines starting outside the valid rows	|	Lines should not appear
6-10	|	Lines starting outside the valid cols	|	Lines should not appear
11-13	|	Lines with zero length					| 	Lines should not appear

Screen #2: Clipped lines
	Lines should appear on the bottom half. Any lines on the top indicate wrap around.

1		| Line from half to the bottom (200-400px)	|	200px line should appear completely on the bottom half of the screen
2-4		| Line from half clipping past the bottom	|	200px line should appear completely on the bottom half of the screen. Nothing should appear on the top half of the screen.

*/

void test_v_line()
{
	int t;
	int r;
	int c;
	int x;
	int row_num;
	int start_c;
	int tile_w[] = {1, 2, 4, 5, 8, 10, 16, 20, 40, 80};

	plot_string(b_fb, 0, 0, "Plot Vertical Line:");
	sync_wait(35);
	clear_screen(l_fb);

	/* ==== Bound checking tests ==== */

    /* Invalid rows */
    plot_vertical_line(l_fb, -1, 100, 50);
    plot_vertical_line(l_fb, -32768, 200, 50);
    plot_vertical_line(l_fb, ROWS, 300, 50);
    plot_vertical_line(l_fb, ROWS + 100, 400, 50);
    plot_vertical_line(l_fb, 32767, 500, 50);

    /* Invalid cols */
    plot_vertical_line(l_fb, 10, -1, 50);
    plot_vertical_line(l_fb, 20, -32768, 50);
    plot_vertical_line(l_fb, 30, COLS, 50);
    plot_vertical_line(l_fb, 40, COLS + 100, 50);
    plot_vertical_line(l_fb, 50, 32767, 50);

    /* Invalid length */
    plot_vertical_line(l_fb, 0, 100, 0);
    plot_vertical_line(l_fb, 200, 320, 0);
    plot_vertical_line(l_fb, 399, 639, 0);

    sync_wait(35);
    clear_screen(l_fb);

    /* ==== Clipping tests ==== */

    /* Touching bottom */
    plot_vertical_line(l_fb, 200, 20, 200);
    /* Clipping past bottom */
    plot_vertical_line(l_fb, 200, 30, 201);
    plot_vertical_line(l_fb, 200, 40, 1000);
    /* Near bottom clipping */
    plot_vertical_line(l_fb, 368, 50, 100);
    plot_vertical_line(l_fb, 380, 60, 50);
    /* Single pixel clipping at bottom */
    plot_vertical_line(l_fb, 399, 70, 1);   /* Last pixel, exact */
    plot_vertical_line(l_fb, 399, 80, 2);   /* Last pixel, clipped */
    /* Large lengths (unsigned) */
    plot_vertical_line(l_fb, 200, 90, 0x8000); /* length = 32768 */
    plot_vertical_line(l_fb, 200, 100, 0xC000); /* length = 49152 */
    /* End pixel wraparound (r+length) */
    plot_vertical_line(l_fb, 200, 110, 0xFFFF);
    plot_vertical_line(l_fb, 399, 120, 0xFFFF);

    sync_wait(70);
    clear_screen(l_fb);

    /* ==== Valid line tests ==== */

    /* Left and right side */
    plot_vertical_line(l_fb, 0, 0, ROWS);
    plot_vertical_line(l_fb, 0, COLS - 1, ROWS);

    /* Pixels at row word boundaries */
    plot_vertical_line(l_fb, 0, 10, 1);
    plot_vertical_line(l_fb, 15, 12, 1);
    plot_vertical_line(l_fb, 31, 14, 1);
    plot_vertical_line(l_fb, 32, 16, 1);
    plot_vertical_line(l_fb, 399, 18, 1);

    /* Plot various lengths */
    for (c = 0; c < 80; c++)
    {
        plot_vertical_line(l_fb, 16, 50 + c, c + 1);
    }

    /* Lines at byte, word, and long bit boundaries */
    plot_vertical_line(l_fb, 150, 7, 32);    /* End of byte 0 */
    plot_vertical_line(l_fb, 150, 8, 32);    /* Start of byte 1 */
    plot_vertical_line(l_fb, 150, 15, 64);   /* End of word 0 */
    plot_vertical_line(l_fb, 150, 16, 64);   /* Start of word 1 */
    plot_vertical_line(l_fb, 150, 31, 96);   /* End of long 0 */
    plot_vertical_line(l_fb, 150, 32, 96);   /* Start of long 1 */

    sync_wait(70);
    clear_screen(l_fb);

	/* ==== Checkerboard ==== */
	for (t = 0; t < 10; t++)
	{
		int size = tile_w[t];
		for (r = 0; r < ROWS; r += size)
		{
			row_num = r / size;

			start_c = 0;
			if (row_num % 2 == 0)
				start_c = size;

			for (c = start_c; c < COLS; c += size * 2)
			{
				for (x = 0; x < size; x++)
				{
					plot_vertical_line(l_fb, r, c+x, size);
				}
			}
		}
		sync_wait(35);
		clear_screen(l_fb);
	}
}

void test_line()
{
	int i;

	plot_string(b_fb, 0, 0,"Plot line");

	/* Invalid rows */
    plot_line(l_fb, -1, 100, 200, 320);
    plot_line(l_fb, -32768, 200, 200, 320);
    plot_line(l_fb, ROWS, 300, 200, 320);
    plot_line(l_fb, ROWS + 100, 400, 200, 320);
    plot_line(l_fb, 32767, 500, 200, 320);

	plot_line(l_fb, 200, 320, -1, 100);
    plot_line(l_fb, 200, 320, -32768, 200);
    plot_line(l_fb, 200, 320, ROWS, 300,);
    plot_line(l_fb, 200, 320, ROWS + 100, 400);
    plot_line(l_fb, 200, 320, 32767, 500);

    /* Invalid cols */
    plot_line(l_fb, 10, -1, 200, 320);
    plot_line(l_fb, 20, -32768, 200, 320);
    plot_line(l_fb, 30, COLS, 200, 320);
    plot_line(l_fb, 40, COLS + 100, 200, 320);
    plot_line(l_fb, 50, 32767, 200, 320);

	plot_line(l_fb, 200, 320, 10, -1);
    plot_line(l_fb, 200, 320, 20, -32768);
    plot_line(l_fb, 200, 320, 30, COLS);
    plot_line(l_fb, 200, 320, 40, COLS + 100);
    plot_line(l_fb, 200, 320, 50, 32767);

	sync_wait(70);
	clear_screen(l_fb);

	/* Diagonals */
	plot_line(l_fb, 0, 0, 100, 100);	/* end > start */
	plot_line(l_fb, 0, 200, 100, 100);	/* end col < start col */
	plot_line(l_fb, 100, 200, 0, 300);	/* end row < start row */
	plot_line(l_fb, 100, 400, 0, 300);	/* end < start */

	/* Straight lines */
	plot_line(l_fb, 300, 200, 200, 200);
	plot_line(l_fb, 200, 300, 200, 200);
	plot_line(l_fb, 300, 300, 200, 300);	/* end row < start row */
	plot_line(l_fb, 300, 300, 300, 200);	/* end col < start col */

	/* Single pixel */
	plot_line(l_fb, 300, 500, 300, 500);

	sync_wait(35);
	clear_screen(l_fb);

    /* Outer edges */
    plot_line(l_fb, 0,        0,        0,        COLS - 1);
    plot_line(l_fb, ROWS - 1, 0,        ROWS - 1, COLS - 1);
    plot_line(l_fb, 0,        0,        ROWS - 1, 0);
    plot_line(l_fb, 0,        COLS - 1, ROWS - 1, COLS - 1);

    /* Corner-to-corner X */
    plot_line(l_fb, 0,        0,        ROWS - 1, COLS - 1);
    plot_line(l_fb, ROWS - 1, 0,        0,        COLS - 1);

	sync_wait(35);
	clear_screen(l_fb);

	/* Test all octants, including straight and diagonals */
    for (i = -150; i <= 150; i += 30)
    {
        plot_line(l_fb, 200, 320, 200 + i, 470); /* Right lines */
        plot_line(l_fb, 200, 320, 200 + i, 170); /* Left lines */
        plot_line(l_fb, 200, 320, 350, 320 + i); /* Bottom lines*/
        plot_line(l_fb, 200, 320, 50, 320 + i); /* Top lines */
    }

	sync_wait(35);
	clear_screen(l_fb);	

    /* 
	End Out of bounds
	Test all octants, including straight and diagonals 
	*/
    for (i = -400; i <= 400; i += 80)
    {
        plot_line(l_fb, 200, 320, 200 + i, 720); /* Right lines */
        plot_line(l_fb, 200, 320, 200 + i, -80); /* Left lines */
        plot_line(l_fb, 200, 320, 600, 320 + i); /* Bottom lines*/
        plot_line(l_fb, 200, 320, -200, 320 + i); /* Top lines */
    }

	sync_wait(35);
	clear_screen(l_fb);

	/* 
	Start Out of bounds
	Test all octants, including straight and diagonals 
	*/
    for (i = -400; i <= 400; i += 80)
    {
        plot_line(l_fb, 200 + i, 720, 200, 320); /* Right lines */
        plot_line(l_fb, 200 + i, -80, 200, 320); /* Left lines */
        plot_line(l_fb, 600, 320 + i, 200, 320); /* Bottom lines*/
        plot_line(l_fb, -200, 320 + i, 200, 320); /* Top lines */
    }

	sync_wait(70);
	clear_screen(l_fb);
}