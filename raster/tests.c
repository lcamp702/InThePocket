#include "raster.h"
#include <OSBIND.H>

#include <stdio.h>

UINT32 *l_fb;
UINT8 *b_fb;

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

void test_clear() 
{
    plot_string(b_fb, 0, 0, "Clear Screen:");
    sync_wait(35);
    fill_screen(l_fb);
    sync_wait(35);
    clear_screen(l_fb);
    sync_wait(70);
}

void test_pixel() 
{
    int c;
    int r;
    plot_string(b_fb, 0, 0, "Plot Pixel:");
    sync_wait(35);
    clear_screen(l_fb);
    for (c=0; c<640; c++)
        for (r=0; r<400; r++)
            if((r&1) == (c&1))
                plot_pixel(b_fb, r, c);
    sync_wait(70);
    clear_screen(l_fb);
}

void test_h_line()
{
    int r;
    plot_string(b_fb, 0, 0, "Plot Horizontal Line:");
    sync_wait(35);
    clear_screen(l_fb);
    Vsync();
    for (r=0; r<100; r++)
    {
        plot_horizontal_line(l_fb, r, 0, 160);
        Vsync();
    }
    for (r=100; r<200; r++)
    {
        plot_horizontal_line(l_fb, r, 160, 160);
    }
    for (r=200; r<300; r++)
    {
        plot_horizontal_line(l_fb, r, 320, 160);
    }
    for (r=300; r<400; r++)
    {
        plot_horizontal_line(l_fb, r, 480, 160);
    }
    sync_wait(70);
    clear_screen(l_fb);
}

void test_v_line()
{
    int c;
    plot_string(b_fb, 0, 0, "Plot Vertical Line:");
    sync_wait(35);
    clear_screen(l_fb);
    Vsync();
    for (c=0; c<160; c++)
    {
        plot_vertical_line(l_fb, 0, c, 100);
        Vsync();
    }
    for (c=160; c<320; c++)
    {
        plot_vertical_line(l_fb, 100, c, 100);
    }
    for (c=320; c<480; c++)
    {
        plot_vertical_line(l_fb, 200, c, 100);
    }
    for (c=480; c<640; c++)
    {
        plot_vertical_line(l_fb, 300, c, 100);
    }
    sync_wait(70);
    clear_screen(l_fb);
}

void test_char()
{
    int i = 0;
    plot_string(b_fb, 0, 0, "Plot Char:");
    sync_wait(35);
    clear_screen(l_fb);
    while (i < 6) 
    {
        plot_character(b_fb, (i*16), (i*16), Cnecin());
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
    for (i = 0; i < 40; i++) {
        int row = (400*i)/40;
        int col = (640*i)/40;
        plot_string(b_fb, row, col, "Hi");
        Vsync();
    }
    sync_wait(70);
}

int main()
{
    l_fb = Physbase();
    b_fb = Physbase();

    disable_cursor();
    clear_screen(l_fb);

    test_clear();    
    test_pixel();
    test_h_line();
    test_v_line();
    test_char();
    test_string();

    return 0;
}