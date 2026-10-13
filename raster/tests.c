#include "raster.h"
#include <OSBIND.H>

#include <stdio.h>

UINT32 *l_fb;
UINT8 *b_fb;

#define COLS 640
#define ROWS 400

void fill_screen(UINT32 *base)
{
	register int i = 0;

	while (i++ < 8000)
		*(base++) = -1;
}

void sync_wait(int frames)
{
	register int i = 0;

	while (i++ < frames)
		Vsync();
}

void disable_cursor()
{
	printf("\033f");
	fflush(stdout);
}

void test_clear_screen()
{
	plot_string(b_fb, 0, 0, "Clear Screen:");
	sync_wait(35);
	fill_screen(l_fb);
	sync_wait(35);
	clear_screen(l_fb);
	sync_wait(70);
}

void test_clear_region()
{
	int t;
	int r;
	int c;
	int row_num;
	int start_c;
	int tile_w[] = {1, 2, 4, 5, 8, 10, 16, 20, 40, 80};

	plot_string(b_fb, 0, 0, "Clear Region:");
	sync_wait(35);

	for (t = 0; t < 10; t++)
	{
		int size = tile_w[t];
		fill_screen(l_fb);

		for (r = 0; r < ROWS; r += size)
		{
			row_num = r / size;
			
			start_c = 0;
			if (row_num % 2 == 0)
				start_c = size;

			for (c = start_c; c < COLS; c += size * 2)
			{
				clear_region(l_fb, r, c, size, size);
			}
		}
		sync_wait(35);
	}
	clear_screen(l_fb);
}

void test_pixel()
{
	int c;
	int r;
	plot_string(b_fb, 0, 0, "Plot Pixel:");
	sync_wait(35);
	clear_screen(l_fb);
	for (c = 0; c < 640; c++)
		for (r = 0; r < 400; r++)
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

void test_char()
{
	int i = 0;
	plot_string(b_fb, 0, 0, "Plot Char:");
	sync_wait(35);
	clear_screen(l_fb);
	while (i < 6)
	{
		plot_character(b_fb, (i * 16), (i * 16), Cnecin());
		i++;
	}
	sync_wait(70);
	clear_screen(l_fb);
}

void test_string()
{
	int i;
	plot_string(b_fb, 0, 0, "Plot String:");
	sync_wait(35);
	clear_screen(l_fb);
	for (i = 0; i < 40; i++)
	{
		int row = (400 * i) / 40;
		int col = (640 * i) / 40;
		plot_string(b_fb, row, col, "Hi");
		Vsync();
	}
	sync_wait(70);
	clear_screen(l_fb);
}

int main()
{
	l_fb = Physbase();
	b_fb = Physbase();

	disable_cursor();
	clear_screen(l_fb);

	test_clear_screen();
	test_clear_region();
	test_pixel();
	test_h_line();
	test_v_line();
	test_char();
	test_string();

	return 0;
}