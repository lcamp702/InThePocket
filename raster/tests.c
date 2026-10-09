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
    char hi[] = "Hi";
    UINT32 *l_fb = Physbase();
    UINT8 *b_fb = Physbase();

    disable_cursor();

    fill_screen(l_fb);
    sync_wait(35);
    clear_screen(l_fb);

    while (i < 6) 
    {
        sync_wait(10);
        plot_character(b_fb, (i*16), (i*16), Cnecin());
        sync_wait(10);
        i++;
    }
    clear_screen(l_fb);
    Cnecin();
    for (i = 0; i < 40; i++) {
        int row = (400*i)/40;
        int col = (640*i)/40;
        plot_string(b_fb, row, col, hi);
        Vsync();
    }
    
    Cnecin();

    return 0;
}