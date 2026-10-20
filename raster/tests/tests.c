#include "tests.h"
#include <stdio.h>

UINT32 *l_fb;
UINT16 *w_fb;
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

int main()
{
	int i;
	int j;
	l_fb = Physbase();
	w_fb = Physbase();
	b_fb = Physbase();
	disable_cursor();
	clear_screen(l_fb);

	for(i = 0; i < 16; i++)
	{
		for (j = 0; j < 16; j++)
		{
			plot_character(b_fb, j*16, i*8, i+(j*16));
		}
	}

	Cnecin();
	test_rect();
	Cnecin();
	test_square();
	Cnecin();
	test_triangle();

	Cnecin();
	test_clear_screen();
	Cnecin();
	test_clear_region();
	Cnecin();
	test_pixel();
	Cnecin();
	test_h_line();
	Cnecin();
	test_v_line();
	Cnecin();
	test_line();
	Cnecin();
	test_bmp8();
	Cnecin();
	test_bmp16();
	Cnecin();
	test_bmp32();
	Cnecin();
	test_char();
	Cnecin();
	test_string();
	Cnecin();

	return 0;
}