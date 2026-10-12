#include "raster.h"
#include <OSBIND.H>

#include <stdio.h>

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

int main()
{
    int i = 0;
    int c = 0;
    int r = 0;
    UINT32 *l_fb = Physbase();
    UINT8 *b_fb = Physbase();

    disable_cursor();

    plot_string(b_fb, 0, 0, "Clear Screen:");
    sync_wait(35);
    fill_screen(l_fb);
    sync_wait(35);
    clear_screen(l_fb);

    plot_string(b_fb, 0, 0, "Plot Pixel:");
    sync_wait(35);
    clear_screen(l_fb);
    for (c=0; c<640; c++)
        for (r=0; r<400; r++)
            if((r&1) == (c&1))
                plot_pixel(b_fb, r, c);
    sync_wait(35);
    clear_screen(l_fb);

    plot_string(b_fb, 0, 0, "Plot Vertical Line:");
    sync_wait(35);
    clear_screen(l_fb);
    Vsync();
    for (c=0; c<160; c++)
    {
        plot_vertical_line(l_fb, 0, c, 100);
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
    sync_wait(35);
    clear_screen(l_fb);

    plot_string(b_fb, 0, 0, "Plot Char:");
    sync_wait(35);
    clear_screen(l_fb);
    while (i < 6) 
    {
        plot_character(b_fb, (i*16), (i*16), Cnecin());
        i++;
    }
    sync_wait(35);
    clear_screen(l_fb);

    plot_string(b_fb, 0, 0, "Plot String:");
    sync_wait(35);
    clear_screen(l_fb);
    for (i = 0; i < 40; i++) {
        int row = (400*i)/40;
        int col = (640*i)/40;
        plot_string(b_fb, row, col, "Hi");
        Vsync();
    }
    Cnecin();

    return 0;
}