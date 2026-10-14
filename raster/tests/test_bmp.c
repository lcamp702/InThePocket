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
static const UINT8 bmp8_note[] = {
    0x08, 0x08, 0x0E, 0x0B, 0x09, 0x09, 0x08, 
    0x08, 0x08, 0x78, 0xF8, 0xF8, 0xF8, 0x70
};

static const UINT16 bmp16_solid[] = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

static const UINT16 bmp16_box[] = {
    0xFFFF, 0x8001, 0x8001, 0x8001,
    0x8001, 0x8001, 0x8001, 0x8001,
    0x8001, 0x8001, 0x8001, 0x8001,
    0x8001, 0x8001, 0x8001, 0xFFFF
};

static const UINT16 bmp16_cross[] = {
    0x8001, 0x4002, 0x2004, 0x1008,
    0x0810, 0x0420, 0x0240, 0x0180,
    0x0180, 0x0240, 0x0420, 0x0810,
    0x1008, 0x2004, 0x4002, 0x8001
};

static const UINT16 bmp16_note[] = {
    0x0040, 0x00C0, 0x00C0, 0x00E0, 
    0x00E0, 0x00F0, 0x00F8, 0x00CE, 
    0x00C6, 0x00C3, 0x00C3, 0x00C3, 
    0x00C3, 0x00C3, 0x00C2, 0x00C2, 
    0x00C4, 0x00C4, 0x00C0, 0x00C0, 
    0x00C0, 0x00C0, 0x0FC0, 0x3FC0, 
    0x7FC0, 0xFF00, 0xFE00, 0x7C00
};

static const UINT32 bmp32_solid[] = {
    0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
    0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
    0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
    0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
    0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
    0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
    0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
    0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL
};
static const UINT32 bmp32_box[] = {
    0xFFFFFFFFUL, 0x80000001UL, 0x80000001UL, 0x80000001UL,
    0x80000001UL, 0x80000001UL, 0x80000001UL, 0x80000001UL,
    0x80000001UL, 0x80000001UL, 0x80000001UL, 0x80000001UL,
    0x80000001UL, 0x80000001UL, 0x80000001UL, 0x80000001UL,
    0x80000001UL, 0x80000001UL, 0x80000001UL, 0x80000001UL,
    0x80000001UL, 0x80000001UL, 0x80000001UL, 0x80000001UL,
    0x80000001UL, 0x80000001UL, 0x80000001UL, 0x80000001UL,
    0x80000001UL, 0x80000001UL, 0x80000001UL, 0xFFFFFFFFUL
};
static const UINT32 bmp32_cross[] = {
    0x80000001UL, 0x40000002UL, 0x20000004UL, 0x10000008UL,
    0x08000010UL, 0x04000020UL, 0x02000040UL, 0x01000080UL,
    0x00800100UL, 0x00400200UL, 0x00200400UL, 0x00100800UL,
    0x00081000UL, 0x00042000UL, 0x00024000UL, 0x00018000UL,
    0x00018000UL, 0x00024000UL, 0x00042000UL, 0x00081000UL,
    0x00100800UL, 0x00200400UL, 0x00400200UL, 0x00800100UL,
    0x01000080UL, 0x02000040UL, 0x04000020UL, 0x08000010UL,
    0x10000008UL, 0x20000004UL, 0x40000002UL, 0x80000001UL
};
static const UINT32 bmp32_arrow[] = {
    0x80000000UL, 0xC0000000UL, 0xE0000000UL, 0xF0000000UL,
    0xF8000000UL, 0xFC000000UL, 0xFE000000UL, 0xFF000000UL,
    0xFF800000UL, 0xFFC00000UL, 0xFFE00000UL, 0xFFF00000UL,
    0xFFF80000UL, 0xFFFC0000UL, 0xFFFE0000UL, 0xFFC00000UL,
    0xFCC00000UL, 0xF8600000UL, 0xF0600000UL, 0xE0300000UL,
    0xC0300000UL, 0x80180000UL, 0x00180000UL, 0x000C0000UL,
    0x000C0000UL, 0x00060000UL, 0x00060000UL, 0x00030000UL,
    0x00030000UL, 0x00018000UL, 0x00018000UL, 0x00000000UL
};
static const UINT32 bmp32_tall[] = {
    0x00018000UL, 0x0003C000UL, 0x0007E000UL, 0x000FF000UL,
    0x001FF800UL, 0x003FFC00UL, 0x007FFE00UL, 0x00FFFF00UL,
    0x01FFFF80UL, 0x03FFFFC0UL, 0x07FFFFE0UL, 0x0FFFFFF0UL,
    0x1FFFFFF8UL, 0x3FFFFFFCUL, 0x7FFFFFFEUL, 0xFFFFFFFFUL,
    0xFFFFFFFFUL, 0x7FFFFFFEUL, 0x3FFFFFFCUL, 0x1FFFFFF8UL,
    0x0FFFFFF0UL, 0x07FFFFE0UL, 0x03FFFFC0UL, 0x01FFFF80UL,
    0x00FFFF00UL, 0x007FFE00UL, 0x003FFC00UL, 0x001FF800UL,
    0x000FF000UL, 0x0007E000UL, 0x0003C000UL, 0x00018000UL,
    0x00018000UL, 0x00018000UL, 0x00018000UL, 0x00018000UL,
    0x0003C000UL, 0x0007E000UL, 0x000FF000UL, 0x001FF800UL,
    0x003FFC00UL, 0x007FFE00UL, 0x00FFFF00UL, 0x01FFFF80UL,
    0x03FFFFC0UL, 0x07FFFFE0UL, 0x0FFFFFF0UL, 0x1FFFFFF8UL
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
    plot_8bit_bitmap(b_fb, (ROWS - 16) / 2, (COLS - 8) / 2, bmp8_note, 14);
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
                plot_8bit_bitmap(b_fb, r+2, c, bmp_cross, 8);
            }
            else
            {
                plot_8bit_bitmap(b_fb, r, c, bmp8_note, 14);
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
        plot_8bit_bitmap(b_fb, r, c, bmp8_note, 14);
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

void bmp16_subpixel_shifts()
{
    int shift;
    int base_col;
    int r;
    clear_screen(l_fb);
    base_col = 140;
    r = 40;

    for (shift = 0; shift < 16; shift++)
    {
        plot_16bit_bitmap(w_fb, r + (shift * 20), base_col + shift, bmp16_box, 16);
        plot_16bit_bitmap(w_fb, r + (shift * 20), base_col + shift + 40, bmp16_cross, 16);
    }
    sync_wait(70);
}

void bmp16_corners_and_center()
{
    clear_screen(l_fb);
    plot_16bit_bitmap(w_fb, 0, 0, bmp16_box, 16);
    plot_16bit_bitmap(w_fb, 0, COLS - 16, bmp16_box, 16);
    plot_16bit_bitmap(w_fb, ROWS - 16, 0, bmp16_box, 16);
    plot_16bit_bitmap(w_fb, ROWS - 16, COLS - 16, bmp16_box, 16);
    plot_16bit_bitmap(w_fb, (ROWS - 24) / 2, (COLS - 16) / 2, bmp16_note, 28);
    sync_wait(70);
}

void bmp16_grid_tiling()
{
    int r;
    int c;
    int toggle;
    clear_screen(l_fb);
    for (r = 0; r <= ROWS - 16; r += 32)
    {
        for (c = 0; c <= COLS - 16; c += 32)
        {
            toggle = ((r >> 5) + (c >> 5)) % 2;
            if (toggle == 0)
            {
                plot_16bit_bitmap(w_fb, r+6, c, bmp16_cross, 16);
            }
            else
            {
                plot_16bit_bitmap(w_fb, r, c, bmp16_note, 28);
            }
        }
    }
    sync_wait(70);
}

void bmp16_animated_sweep()
{
    int frame;
    int r;
    int c;
    r = 0;
    c = 0;
    for (frame = 0; frame < 200; frame++)
    {
        clear_screen(l_fb);
        plot_16bit_bitmap(w_fb, r, c, bmp16_note, 28);
        plot_16bit_bitmap(w_fb, ROWS - 16 - r, COLS - 16 - c, bmp16_box, 16);
        r += 1;
        c += 3;
        Vsync();
        Vsync();
    }
}

void bmp16_edge_clipping()
{
    clear_screen(l_fb);
    plot_16bit_bitmap(w_fb, 50, -8, bmp16_solid, 16);
    plot_16bit_bitmap(w_fb, 50, COLS - 8, bmp16_solid, 16);
    plot_16bit_bitmap(w_fb, -8, 300, bmp16_solid, 16);
    plot_16bit_bitmap(w_fb, ROWS - 8, 300, bmp16_solid, 16);
    plot_16bit_bitmap(w_fb, -30, 100, bmp16_solid, 16);
    plot_16bit_bitmap(w_fb, 500, 100, bmp16_solid, 16);
    plot_16bit_bitmap(w_fb, 100, -30, bmp16_solid, 16);
    plot_16bit_bitmap(w_fb, 100, 700, bmp16_solid, 16);
    sync_wait(70);
}

void test_bmp16()
{
    bmp16_subpixel_shifts();
    bmp16_corners_and_center();
    bmp16_grid_tiling();
    bmp16_animated_sweep();
    bmp16_edge_clipping();
    clear_screen(l_fb);
}

void bmp32_subpixel_shifts()
{
    int shift;
    int base_col;
    int r;
    clear_screen(l_fb);
    base_col = 60;
    r = 20;
    for (shift = 0; shift < 16; shift++)
    {
        /* 0 to 15 */
        plot_32bit_bitmap(l_fb, r + (shift * 22), base_col + shift, bmp32_box, 16);
        plot_32bit_bitmap(l_fb, r + (shift * 22), base_col + shift + 60, bmp32_cross, 16);

        /* 16 to 31 */
        plot_32bit_bitmap(l_fb, r + (shift * 22), base_col + 300 + (shift + 16), bmp32_box, 16);
        plot_32bit_bitmap(l_fb, r + (shift * 22), base_col + 300 + (shift + 16) + 60, bmp32_cross, 16);
    }
    sync_wait(70);
}

void bmp32_corners_and_center()
{
    clear_screen(l_fb);
    plot_32bit_bitmap(l_fb, 0, 0, bmp32_box, 32);
    plot_32bit_bitmap(l_fb, 0, COLS - 32, bmp32_box, 32);
    plot_32bit_bitmap(l_fb, ROWS - 32, 0, bmp32_box, 32);
    plot_32bit_bitmap(l_fb, ROWS - 32, COLS - 32, bmp32_box, 32);
    plot_32bit_bitmap(l_fb, (ROWS - 48) / 2, (COLS - 32) / 2, bmp32_tall, 48);
    sync_wait(70);
}

void bmp32_grid_tiling()
{
    int r;
    int c;
    int toggle;
    clear_screen(l_fb);
    for (r = 0; r <= ROWS - 32; r += 64)
    {
        for (c = 0; c <= COLS - 32; c += 64)
        {
            toggle = ((r >> 6) + (c >> 6)) % 2;
            if (toggle == 0)
            {
                plot_32bit_bitmap(l_fb, r, c, bmp32_cross, 32);
            }
            else
            {
                plot_32bit_bitmap(l_fb, r, c, bmp32_arrow, 32);
            }
        }
    }
    sync_wait(70);
}

void bmp32_animated_sweep()
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
        plot_32bit_bitmap(l_fb, r, c, bmp32_tall, 48);
        plot_32bit_bitmap(l_fb, ROWS - 32 - r, COLS - 32 - c, bmp32_box, 32);
        r += 1;
        c += 3;
        Vsync();
    }
}

void bmp32_edge_clipping()
{
    clear_screen(l_fb);
    plot_32bit_bitmap(l_fb, 50, -16, bmp32_solid, 32);
    plot_32bit_bitmap(l_fb, 50, COLS - 16, bmp32_solid, 32);
    plot_32bit_bitmap(l_fb, -16, 300, bmp32_solid, 32);
    plot_32bit_bitmap(l_fb, ROWS - 16, 300, bmp32_solid, 32);
    plot_32bit_bitmap(l_fb, -50, 100, bmp32_solid, 32);
    plot_32bit_bitmap(l_fb, ROWS + 20, 100, bmp32_solid, 32);
    plot_32bit_bitmap(l_fb, 100, -50, bmp32_solid, 32);
    plot_32bit_bitmap(l_fb, 100, COLS + 20, bmp32_solid, 32);
    sync_wait(70);
}

void test_bmp32()
{
    bmp32_subpixel_shifts();
    bmp32_corners_and_center();
    bmp32_grid_tiling();
    bmp32_animated_sweep();
    bmp32_edge_clipping();
    clear_screen(l_fb);
}