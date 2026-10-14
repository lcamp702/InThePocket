#include "tests.h"

static const UINT8 bmp_solid[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
static const UINT8 bmp_box[] = {
    0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF
};
static const UINT8 bmp_cross[] = {
    0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81
};
static const UINT8 bmp_arrow[] = {
    0x80, 0xC0, 0xE0, 0xF0, 0xD8, 0x0C, 0x06, 0x02
};
static const UINT8 bmp_tall[] = {
    0x00, 0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66,
    0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00
};

void bmp8_subpixel_shifts()
{
    int shift;
    int base_col;
    int r;
    clear_screen(l_fb);
    base_col = 160;
    r = 100;

    for (shift = 0; shift < 8; shift++)
    {
        plot_8bit_bitmap(b_fb, r + (shift * 16), base_col + shift, bmp_box, 8);
        plot_8bit_bitmap(b_fb, r + (shift * 16), base_col + shift + 32, bmp_cross, 8);
    }
    sync_wait(70);
}

void bmp8_corners_and_center()
{
    clear_screen(l_fb);
    plot_8bit_bitmap(b_fb, 0, 0, bmp_box, 8);
    plot_8bit_bitmap(b_fb, 0, COLS - 8, bmp_box, 8);
    plot_8bit_bitmap(b_fb, ROWS - 8, 0, bmp_box, 8);
    plot_8bit_bitmap(b_fb, ROWS - 8, COLS - 8, bmp_box, 8);
    plot_8bit_bitmap(b_fb, (ROWS - 16) / 2, (COLS - 8) / 2, bmp_tall, 16);
    sync_wait(70);
}

void bmp8_grid_tiling()
{
    int r;
    int c;
    int toggle;
    clear_screen(l_fb);
    for (r = 0; r < ROWS; r += 16)
    {
        for (c = 0; c < COLS; c += 16)
        {
            toggle = ((r >> 4) + (c >> 4)) % 2;
            if (toggle == 0)
            {
                plot_8bit_bitmap(b_fb, r, c, bmp_cross, 8);
            }
            else
            {
                plot_8bit_bitmap(b_fb, r, c, bmp_arrow, 8);
            }
        }
    }
    sync_wait(70);
}

void bmp8_animated_sweep()
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
        plot_8bit_bitmap(b_fb, r, c, bmp_tall, 16);
        plot_8bit_bitmap(b_fb, ROWS - 16 - r, COLS - 8 - c, bmp_box, 8);
        r += 1;
        c += 3;
        Vsync();
    }
}

void bmp8_edge_clipping()
{
    clear_screen(l_fb);
    plot_8bit_bitmap(b_fb, 50, -4, bmp_solid, 8);
    plot_8bit_bitmap(b_fb, 50, COLS - 4, bmp_solid, 8);
    plot_8bit_bitmap(b_fb, -4, 300, bmp_solid, 8);
    plot_8bit_bitmap(b_fb, ROWS - 4, 300, bmp_solid, 8);
    plot_8bit_bitmap(b_fb, -20, 100, bmp_solid, 8);
    plot_8bit_bitmap(b_fb, 500, 100, bmp_solid, 8);
    plot_8bit_bitmap(b_fb, 100, -20, bmp_solid, 8);
    plot_8bit_bitmap(b_fb, 100, 700, bmp_solid, 8);
    sync_wait(70);
}

void test_bmp8()
{
    bmp8_subpixel_shifts();
    bmp8_corners_and_center();
    bmp8_grid_tiling();
    bmp8_animated_sweep();
    bmp8_edge_clipping();
    clear_screen(l_fb);
}