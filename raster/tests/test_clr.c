#include "tests.h"

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

	fill_screen(l_fb);

	/* Corner edge clips */
	clear_region(l_fb, -8,-8,16,16);
	clear_region(l_fb, -8,632,16,16);
	clear_region(l_fb, 312,-8,16,16);
	clear_region(l_fb, 312,632,16,16);

	/* Side edge clips*/
	clear_region(l_fb, -8,-8,16,16);
	clear_region(l_fb, -8,312,16,16);
	clear_region(l_fb, 192,-8,16,16);
	clear_region(l_fb, 192,312,16,16);

	sync_wait(70);
	fill_screen(l_fb);

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