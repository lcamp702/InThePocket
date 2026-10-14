#ifndef TESTS
#define TESTS

#include "../raster.h"
#include <OSBIND.H>

#define COLS 640
#define ROWS 400

extern UINT32 *l_fb;
extern UINT16 *w_fb;
extern UINT8 *b_fb;

/* test_clr.c */
void test_clear_screen();
void test_clear_region();

/* test_ln.c */
void test_pixel();
void test_h_line();
void test_v_line();

/* test_str.c */
void test_char();
void test_string();

/* test_bmp.c */
void test_bmp8();
void test_bmp16();
void test_bmp32();

void fill_screen(UINT32 *fb);
void sync_wait(int frames);

#endif