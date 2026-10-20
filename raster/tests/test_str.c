#include "tests.h"

#define CHAR_HEIGHT 16
#define CHAR_WIDTH 8

void char_subpixel_shifts()
{
    int shift;
    int base_col;
    int r;
    clear_screen(l_fb);
    base_col = 160;
    r = 100;
    for (shift = 0; shift < 8; shift++)
    {
        plot_character(b_fb, r + (shift * 16), base_col + shift, 'A');
        plot_character(b_fb, r + (shift * 16), base_col + shift + 32, 'B');
    }
    sync_wait(70);
}

void char_corners_and_center()
{
    clear_screen(l_fb);
    plot_character(b_fb, 0, 0, '#');
    plot_character(b_fb, 0, COLS - CHAR_WIDTH, '#');
    plot_character(b_fb, ROWS - CHAR_HEIGHT, 0, '#');
    plot_character(b_fb, ROWS - CHAR_HEIGHT, COLS - CHAR_WIDTH, '#');
    plot_character(b_fb, (ROWS - CHAR_HEIGHT) / 2, (COLS - CHAR_WIDTH) / 2, 'X');
    sync_wait(70);
}

void char_grid_tiling()
{
    int r;
    int c;
    int toggle;
    clear_screen(l_fb);
    for (r = 0; r <= ROWS - CHAR_HEIGHT; r += 16)
    {
        for (c = 0; c <= COLS - CHAR_WIDTH; c += 16)
        {
            toggle = ((r >> 4) + (c >> 4)) % 2;
            if (toggle == 0)
            {
                plot_character(b_fb, r, c, 'X');
            }
            else
            {
                plot_character(b_fb, r, c, 'O');
            }
        }
    }
    sync_wait(70);
}

void char_animated_sweep()
{
    int frame;
    int r;
    int c;
    r = 0;
    c = 0;
    for (frame = 0; frame < 200; frame++)
    {
        clear_screen(l_fb);
        Vsync();
        plot_character(b_fb, r, c, 'W');
        plot_character(b_fb, ROWS - CHAR_HEIGHT - r, COLS - CHAR_WIDTH - c, 'M');
        r += 1;
        c += 3;
        Vsync();
    }
}

void char_edge_clipping()
{
    clear_screen(l_fb);
    plot_character(b_fb, 50, -4, 'E');
    plot_character(b_fb, 50, COLS - 4, 'E');
    plot_character(b_fb, -4, 300, 'E');
    plot_character(b_fb, ROWS - 4, 300, 'E');
    plot_character(b_fb, -20, 100, 'E');
    plot_character(b_fb, 500, 100, 'E');
    plot_character(b_fb, 100, -20, 'E');
    plot_character(b_fb, 100, 700, 'E');
    sync_wait(70);
}

void test_char()
{
	plot_string(b_fb, 0, 0, "Plot Char:");
	sync_wait(35);
	clear_screen(l_fb);
    char_subpixel_shifts();
    char_corners_and_center();
    char_grid_tiling();
    char_animated_sweep();
    char_edge_clipping();
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