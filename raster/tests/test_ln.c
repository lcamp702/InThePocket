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