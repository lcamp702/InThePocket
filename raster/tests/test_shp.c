#include "tests.h"

void test_rect()
{
    plot_string(b_fb,0,0,"Test Rectangles");
    sync_wait(35);
    clear_screen(l_fb);

    plot_rectangle(l_fb, 0, 0, 400, 640); /* Full border */

    plot_rectangle(l_fb, 100, 320, 100, 122);
    plot_rectangle(l_fb, 200, 320, 100, 123);

    plot_rectangle(l_fb, 101, 200, 99, 120);
    plot_rectangle(l_fb, 200, 199, 99, 121);

    plot_rectangle(l_fb, 50, 50, 10, 10);
    /*
    sync_wait(35);
    clear_screen(l_fb);
    */
}

void test_square()
{
    plot_string(b_fb,0,0,"Test Squares");
    sync_wait(35);
    clear_screen(l_fb);

    plot_square(l_fb, 10, 10, 10);
    plot_square(l_fb, 25, 25, 25);

    plot_square(l_fb, 64, 64, 64);

    /*
    sync_wait(35);
    clear_screen(l_fb);
    */
}

void test_triangle()
{
    plot_string(b_fb,0,0,"Test Triangles");
    sync_wait(35);
    clear_screen(l_fb);

    plot_triangle(l_fb, 10, 10, 10, 10, 0);
    plot_triangle(l_fb, 30, 30, 10, 10, 1);
    plot_triangle(l_fb, 50, 50, 10, 10, 2);
    plot_triangle(l_fb, 70, 70, 10, 10, 3);

    /*
    sync_wait(35);
    clear_screen(l_fb);
    */
}