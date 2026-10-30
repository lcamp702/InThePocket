/*
    Peter Ursem
    purse327@mtroyal.ca

    Logan Campbell
    lcamp702@mtroyal.ca

    COMP2659-001
    Steve Kalmar

    In the Pocket - Checkpoint 3
    Due: Nov 9, 2026

    The main game module for In the Pocket.
    This module will orchestrate the main game loop using submodule functions.
    pocket.c
*/

#include "model/model.h"
#include "render/render.h"
#include "system/system.h"
#include "events/events.h"

#define SONG_NOTES 374

static const Note song[SONG_NOTES] = {
    /* UINT32 time, INT16 x, UINT8 pitch, UINT8 duration */
    {   567, 2, 2,0 }, /* #00 |   8.10s | MIDI 73 | 0.25 beats (Quarter) */
    {   592, 3, 0,0 }, /* #01 |   8.46s | MIDI 72 | 6.25 beats (Whole) */
    {   835, 3, 2,0 }, /* #02 |  11.93s | MIDI 72 | 0.25 beats (Quarter) */
    {   891, 0, 2,0 }, /* #03 |  12.72s | MIDI 75 | 0.25 beats (Quarter) */
    {   928, 0, 2,0 }, /* #04 |  13.25s | MIDI 75 | 0.83 beats (Quarter) */
    {   965, 1, 2,0 }, /* #05 |  13.78s | MIDI 74 | 0.25 beats (Quarter) */
    {  1002, 1, 2,0 }, /* #06 |  14.31s | MIDI 74 | 0.83 beats (Quarter) */
    {  1039, 2, 2,0 }, /* #07 |  14.84s | MIDI 73 | 0.25 beats (Quarter) */
    {  1076, 2, 2,0 }, /* #08 |  15.37s | MIDI 73 | 0.83 beats (Quarter) */
    {  1112, 3, 2,0 }, /* #09 |  15.89s | MIDI 72 | 0.25 beats (Quarter) */
    {  1149, 3, 2,0 }, /* #10 |  16.42s | MIDI 72 | 0.83 beats (Quarter) */
    {  1186, 1, 2,0 }, /* #11 |  16.94s | MIDI 74 | 0.25 beats (Quarter) */
    {  1223, 1, 2,0 }, /* #12 |  17.47s | MIDI 74 | 0.83 beats (Quarter) */
    {  1260, 2, 2,0 }, /* #13 |  18.00s | MIDI 73 | 0.25 beats (Quarter) */
    {  1297, 2, 2,0 }, /* #14 |  18.52s | MIDI 73 | 0.83 beats (Quarter) */
    {  1333, 2, 2,0 }, /* #15 |  19.05s | MIDI 73 | 0.25 beats (Quarter) */
    {  1370, 2, 2,0 }, /* #16 |  19.57s | MIDI 73 | 1.00 beats (Quarter) */
    {  1517, 1, 2,0 }, /* #17 |  21.67s | MIDI 74 | 0.25 beats (Quarter) */
    {  1554, 3, 2,0 }, /* #18 |  22.19s | MIDI 72 | 0.25 beats (Quarter) */
    {  1590, 0, 2,0 }, /* #19 |  22.72s | MIDI 75 | 0.25 beats (Quarter) */
    {  1615, 1, 2,0 }, /* #20 |  23.07s | MIDI 74 | 0.25 beats (Quarter) */
    {  1651, 1, 2,0 }, /* #21 |  23.59s | MIDI 74 | 0.25 beats (Quarter) */
    {  1664, 0, 2,0 }, /* #22 |  23.77s | MIDI 75 | 0.25 beats (Quarter) */
    {  1700, 3, 2,0 }, /* #23 |  24.29s | MIDI 72 | 0.25 beats (Quarter) */
    {  1737, 1, 2,0 }, /* #24 |  24.81s | MIDI 74 | 0.25 beats (Quarter) */
    {  1773, 2, 2,0 }, /* #25 |  25.33s | MIDI 73 | 0.25 beats (Quarter) */
    {  1797, 1, 2,0 }, /* #26 |  25.67s | MIDI 74 | 0.25 beats (Quarter) */
    {  1809, 0, 2,0 }, /* #27 |  25.85s | MIDI 75 | 0.25 beats (Quarter) */
    {  1846, 1, 2,0 }, /* #28 |  26.37s | MIDI 74 | 0.25 beats (Quarter) */
    {  1881, 1, 2,0 }, /* #29 |  26.88s | MIDI 74 | 0.25 beats (Quarter) */
    {  1917, 2, 2,0 }, /* #30 |  27.39s | MIDI 73 | 0.25 beats (Quarter) */
    {  1953, 3, 2,0 }, /* #31 |  27.90s | MIDI 72 | 0.25 beats (Quarter) */
    {  1989, 1, 2,0 }, /* #32 |  28.41s | MIDI 74 | 1.00 beats (Quarter) */
    {  1989, 2, 2,0 }, /* #33 |  28.41s | MIDI 73 | 1.00 beats (Quarter) */
    {  2062, 0, 2,0 }, /* #34 |  29.45s | MIDI 75 | 1.00 beats (Quarter) */
    {  2062, 2, 2,0 }, /* #35 |  29.45s | MIDI 73 | 1.00 beats (Quarter) */
    {  2135, 0, 2,0 }, /* #36 |  30.50s | MIDI 75 | 0.25 beats (Quarter) */
    {  2135, 1, 2,0 }, /* #37 |  30.50s | MIDI 74 | 0.25 beats (Quarter) */
    {  2171, 0, 2,0 }, /* #38 |  31.01s | MIDI 75 | 0.25 beats (Quarter) */
    {  2171, 1, 2,0 }, /* #39 |  31.01s | MIDI 74 | 0.25 beats (Quarter) */
    {  2207, 0, 2,0 }, /* #40 |  31.53s | MIDI 75 | 0.25 beats (Quarter) */
    {  2207, 2, 2,0 }, /* #41 |  31.53s | MIDI 73 | 0.25 beats (Quarter) */
    {  2243, 0, 2,0 }, /* #42 |  32.04s | MIDI 75 | 0.25 beats (Quarter) */
    {  2243, 2, 2,0 }, /* #43 |  32.04s | MIDI 73 | 0.25 beats (Quarter) */
    {  2279, 0, 2,0 }, /* #44 |  32.56s | MIDI 75 | 0.25 beats (Quarter) */
    {  2279, 1, 2,0 }, /* #45 |  32.56s | MIDI 74 | 0.25 beats (Quarter) */
    {  2315, 0, 2,0 }, /* #46 |  33.07s | MIDI 75 | 0.25 beats (Quarter) */
    {  2315, 1, 2,0 }, /* #47 |  33.07s | MIDI 74 | 0.25 beats (Quarter) */
    {  2351, 1, 2,0 }, /* #48 |  33.59s | MIDI 74 | 0.25 beats (Quarter) */
    {  2351, 2, 2,0 }, /* #49 |  33.59s | MIDI 73 | 0.25 beats (Quarter) */
    {  2375, 0, 2,0 }, /* #50 |  33.93s | MIDI 75 | 1.00 beats (Quarter) */
    {  2375, 2, 2,0 }, /* #51 |  33.93s | MIDI 73 | 1.00 beats (Quarter) */
    {  2423, 1, 2,0 }, /* #52 |  34.62s | MIDI 74 | 0.25 beats (Quarter) */
    {  2423, 2, 2,0 }, /* #53 |  34.62s | MIDI 73 | 0.25 beats (Quarter) */
    {  2459, 1, 2,0 }, /* #54 |  35.13s | MIDI 74 | 0.25 beats (Quarter) */
    {  2459, 2, 2,0 }, /* #55 |  35.13s | MIDI 73 | 0.25 beats (Quarter) */
    {  2495, 1, 2,0 }, /* #56 |  35.64s | MIDI 74 | 0.25 beats (Quarter) */
    {  2495, 2, 2,0 }, /* #57 |  35.64s | MIDI 73 | 0.25 beats (Quarter) */
    {  2531, 0, 2,0 }, /* #58 |  36.16s | MIDI 75 | 1.00 beats (Quarter) */
    {  2531, 2, 2,0 }, /* #59 |  36.16s | MIDI 73 | 1.00 beats (Quarter) */
    {  2590, 1, 0,0 }, /* #60 |  37.00s | MIDI 74 | 3.00 beats (Whole) */
    {  2590, 3, 0,0 }, /* #61 |  37.00s | MIDI 72 | 3.00 beats (Whole) */
    {  2706, 2, 2,0 }, /* #62 |  38.65s | MIDI 73 | 0.25 beats (Quarter) */
    {  2742, 0, 2,0 }, /* #63 |  39.16s | MIDI 75 | 0.25 beats (Quarter) */
    {  2777, 1, 2,0 }, /* #64 |  39.68s | MIDI 74 | 0.25 beats (Quarter) */
    {  2813, 0, 2,0 }, /* #65 |  40.19s | MIDI 75 | 0.25 beats (Quarter) */
    {  2849, 3, 2,0 }, /* #66 |  40.70s | MIDI 72 | 0.25 beats (Quarter) */
    {  2885, 1, 2,0 }, /* #67 |  41.22s | MIDI 74 | 0.25 beats (Quarter) */
    {  2921, 2, 2,0 }, /* #68 |  41.73s | MIDI 73 | 0.25 beats (Quarter) */
    {  2957, 1, 2,0 }, /* #69 |  42.25s | MIDI 74 | 0.25 beats (Quarter) */
    {  2993, 1, 2,0 }, /* #70 |  42.76s | MIDI 74 | 0.25 beats (Quarter) */
    {  2993, 2, 2,0 }, /* #71 |  42.76s | MIDI 73 | 0.25 beats (Quarter) */
    {  3017, 1, 1,0 }, /* #72 |  43.10s | MIDI 74 | 2.50 beats (Half) */
    {  3017, 3, 1,0 }, /* #73 |  43.10s | MIDI 72 | 2.50 beats (Half) */
    {  3111, 2, 2,0 }, /* #74 |  44.44s | MIDI 73 | 0.25 beats (Quarter) */
    {  3135, 3, 1,0 }, /* #75 |  44.78s | MIDI 72 | 3.00 beats (Half) */
    {  3242, 1, 2,0 }, /* #76 |  46.32s | MIDI 74 | 0.13 beats (Quarter) */
    {  3269, 1, 1,0 }, /* #77 |  46.70s | MIDI 74 | 1.50 beats (Half) */
    {  3349, 2, 2,0 }, /* #78 |  47.84s | MIDI 73 | 0.25 beats (Quarter) */
    {  3361, 1, 2,0 }, /* #79 |  48.01s | MIDI 74 | 0.25 beats (Quarter) */
    {  3372, 0, 0,0 }, /* #80 |  48.18s | MIDI 75 | 3.00 beats (Whole) */
    {  3525, 1, 2,0 }, /* #81 |  50.36s | MIDI 74 | 0.25 beats (Quarter) */
    {  3549, 1, 0,0 }, /* #82 |  50.70s | MIDI 74 | 4.33 beats (Whole) */
    {  3986, 3, 0,0 }, /* #83 |  56.95s | MIDI 72 | 5.83 beats (Whole) */
    {  4201, 0, 2,0 }, /* #84 |  60.01s | MIDI 75 | 0.25 beats (Quarter) */
    {  4225, 1, 2,0 }, /* #85 |  60.35s | MIDI 74 | 0.25 beats (Quarter) */
    {  4249, 2, 0,0 }, /* #86 |  60.69s | MIDI 73 | 4.50 beats (Whole) */
    {  4417, 0, 2,0 }, /* #87 |  63.10s | MIDI 75 | 0.25 beats (Quarter) */
    {  4453, 0, 2,0 }, /* #88 |  63.62s | MIDI 75 | 0.75 beats (Quarter) */
    {  4490, 1, 2,0 }, /* #89 |  64.14s | MIDI 74 | 0.25 beats (Quarter) */
    {  4526, 1, 2,0 }, /* #90 |  64.66s | MIDI 74 | 0.75 beats (Quarter) */
    {  4562, 2, 2,0 }, /* #91 |  65.17s | MIDI 73 | 0.25 beats (Quarter) */
    {  4598, 2, 2,0 }, /* #92 |  65.69s | MIDI 73 | 0.75 beats (Quarter) */
    {  4635, 3, 2,0 }, /* #93 |  66.21s | MIDI 72 | 0.25 beats (Quarter) */
    {  4671, 3, 2,0 }, /* #94 |  66.72s | MIDI 72 | 0.75 beats (Quarter) */
    {  4707, 3, 2,0 }, /* #95 |  67.24s | MIDI 72 | 0.25 beats (Quarter) */
    {  4743, 3, 2,0 }, /* #96 |  67.76s | MIDI 72 | 1.00 beats (Quarter) */
    {  4922, 3, 2,0 }, /* #97 |  70.31s | MIDI 72 | 0.25 beats (Quarter) */
    {  4958, 1, 2,0 }, /* #98 |  70.83s | MIDI 74 | 0.25 beats (Quarter) */
    {  4994, 2, 2,0 }, /* #99 |  71.34s | MIDI 73 | 0.25 beats (Quarter) */
    {  5030, 1, 2,0 }, /* #100 |  71.86s | MIDI 74 | 0.25 beats (Quarter) */
    {  5067, 2, 2,0 }, /* #101 |  72.38s | MIDI 73 | 0.25 beats (Quarter) */
    {  5102, 0, 2,0 }, /* #102 |  72.89s | MIDI 75 | 0.25 beats (Quarter) */
    {  5138, 1, 2,0 }, /* #103 |  73.40s | MIDI 74 | 0.25 beats (Quarter) */
    {  5173, 0, 2,0 }, /* #104 |  73.90s | MIDI 75 | 0.25 beats (Quarter) */
    {  5209, 0, 2,0 }, /* #105 |  74.41s | MIDI 75 | 0.25 beats (Quarter) */
    {  5245, 1, 2,0 }, /* #106 |  74.92s | MIDI 74 | 0.25 beats (Quarter) */
    {  5268, 2, 2,0 }, /* #107 |  75.26s | MIDI 73 | 0.25 beats (Quarter) */
    {  5280, 3, 2,0 }, /* #108 |  75.43s | MIDI 72 | 0.25 beats (Quarter) */
    {  5316, 1, 2,0 }, /* #109 |  75.94s | MIDI 74 | 0.25 beats (Quarter) */
    {  5316, 2, 2,0 }, /* #110 |  75.94s | MIDI 73 | 0.25 beats (Quarter) */
    {  5351, 0, 2,0 }, /* #111 |  76.45s | MIDI 75 | 1.00 beats (Quarter) */
    {  5351, 2, 2,0 }, /* #112 |  76.45s | MIDI 73 | 1.00 beats (Quarter) */
    {  5423, 0, 2,0 }, /* #113 |  77.47s | MIDI 75 | 1.00 beats (Quarter) */
    {  5423, 1, 2,0 }, /* #114 |  77.47s | MIDI 74 | 1.00 beats (Quarter) */
    {  5495, 0, 2,0 }, /* #115 |  78.50s | MIDI 75 | 0.25 beats (Quarter) */
    {  5495, 1, 2,0 }, /* #116 |  78.50s | MIDI 74 | 0.25 beats (Quarter) */
    {  5530, 0, 2,0 }, /* #117 |  79.01s | MIDI 75 | 0.25 beats (Quarter) */
    {  5530, 1, 2,0 }, /* #118 |  79.01s | MIDI 74 | 0.25 beats (Quarter) */
    {  5566, 0, 2,0 }, /* #119 |  79.52s | MIDI 75 | 0.25 beats (Quarter) */
    {  5566, 2, 2,0 }, /* #120 |  79.52s | MIDI 73 | 0.25 beats (Quarter) */
    {  5602, 0, 2,0 }, /* #121 |  80.02s | MIDI 75 | 0.25 beats (Quarter) */
    {  5602, 2, 2,0 }, /* #122 |  80.02s | MIDI 73 | 0.25 beats (Quarter) */
    {  5637, 0, 2,0 }, /* #123 |  80.53s | MIDI 75 | 0.25 beats (Quarter) */
    {  5637, 1, 2,0 }, /* #124 |  80.53s | MIDI 74 | 0.25 beats (Quarter) */
    {  5673, 0, 2,0 }, /* #125 |  81.04s | MIDI 75 | 0.25 beats (Quarter) */
    {  5673, 1, 2,0 }, /* #126 |  81.04s | MIDI 74 | 0.25 beats (Quarter) */
    {  5708, 1, 2,0 }, /* #127 |  81.55s | MIDI 74 | 0.25 beats (Quarter) */
    {  5732, 0, 2,0 }, /* #128 |  81.89s | MIDI 75 | 1.00 beats (Quarter) */
    {  5780, 1, 2,0 }, /* #129 |  82.57s | MIDI 74 | 0.25 beats (Quarter) */
    {  5780, 2, 2,0 }, /* #130 |  82.57s | MIDI 73 | 0.25 beats (Quarter) */
    {  5815, 0, 2,0 }, /* #131 |  83.07s | MIDI 75 | 0.25 beats (Quarter) */
    {  5815, 2, 2,0 }, /* #132 |  83.07s | MIDI 73 | 0.25 beats (Quarter) */
    {  5851, 0, 2,0 }, /* #133 |  83.58s | MIDI 75 | 0.25 beats (Quarter) */
    {  5851, 1, 2,0 }, /* #134 |  83.58s | MIDI 74 | 0.25 beats (Quarter) */
    {  5886, 1, 2,0 }, /* #135 |  84.09s | MIDI 74 | 1.00 beats (Quarter) */
    {  5945, 1, 1,0 }, /* #136 |  84.93s | MIDI 74 | 1.75 beats (Half) */
    {  5945, 3, 1,0 }, /* #137 |  84.93s | MIDI 72 | 1.75 beats (Half) */
    {  6014, 2, 2,0 }, /* #138 |  85.91s | MIDI 73 | 1.25 beats (Quarter) */
    {  6060, 1, 2,0 }, /* #139 |  86.57s | MIDI 74 | 0.25 beats (Quarter) */
    {  6083, 1, 2,0 }, /* #140 |  86.91s | MIDI 74 | 0.75 beats (Quarter) */
    {  6119, 1, 2,0 }, /* #141 |  87.41s | MIDI 74 | 0.25 beats (Quarter) */
    {  6155, 3, 2,0 }, /* #142 |  87.92s | MIDI 72 | 1.25 beats (Quarter) */
    {  6202, 0, 2,0 }, /* #143 |  88.60s | MIDI 75 | 0.25 beats (Quarter) */
    {  6238, 1, 2,0 }, /* #144 |  89.11s | MIDI 74 | 0.25 beats (Quarter) */
    {  6262, 1, 2,0 }, /* #145 |  89.45s | MIDI 74 | 0.25 beats (Quarter) */
    {  6274, 2, 2,0 }, /* #146 |  89.62s | MIDI 73 | 0.17 beats (Quarter) */
    {  6297, 3, 2,0 }, /* #147 |  89.96s | MIDI 72 | 0.25 beats (Quarter) */
    {  6309, 3, 2,0 }, /* #148 |  90.13s | MIDI 72 | 0.83 beats (Quarter) */
    {  6345, 2, 2,0 }, /* #149 |  90.64s | MIDI 73 | 0.75 beats (Quarter) */
    {  6381, 2, 2,0 }, /* #150 |  91.15s | MIDI 73 | 0.25 beats (Quarter) */
    {  6405, 1, 2,0 }, /* #151 |  91.49s | MIDI 74 | 0.25 beats (Quarter) */
    {  6417, 2, 2,0 }, /* #152 |  91.67s | MIDI 73 | 0.25 beats (Quarter) */
    {  6440, 3, 2,0 }, /* #153 |  92.01s | MIDI 72 | 1.17 beats (Quarter) */
    {  6488, 0, 2,0 }, /* #154 |  92.69s | MIDI 75 | 0.25 beats (Quarter) */
    {  6512, 2, 2,0 }, /* #155 |  93.03s | MIDI 73 | 0.25 beats (Quarter) */
    {  6524, 1, 2,0 }, /* #156 |  93.19s | MIDI 74 | 0.25 beats (Quarter) */
    {  6547, 2, 2,0 }, /* #157 |  93.53s | MIDI 73 | 0.25 beats (Quarter) */
    {  6559, 2, 2,0 }, /* #158 |  93.70s | MIDI 73 | 0.99 beats (Quarter) */
    {  6595, 3, 2,0 }, /* #159 |  94.21s | MIDI 72 | 0.25 beats (Quarter) */
    {  6619, 3, 2,0 }, /* #160 |  94.55s | MIDI 72 | 1.17 beats (Quarter) */
    {  6667, 1, 2,0 }, /* #161 |  95.24s | MIDI 74 | 0.25 beats (Quarter) */
    {  6691, 1, 2,0 }, /* #162 |  95.58s | MIDI 74 | 1.17 beats (Quarter) */
    {  6739, 3, 2,0 }, /* #163 |  96.27s | MIDI 72 | 0.25 beats (Quarter) */
    {  6763, 3, 2,0 }, /* #164 |  96.62s | MIDI 72 | 1.17 beats (Quarter) */
    {  6812, 1, 2,0 }, /* #165 |  97.31s | MIDI 74 | 0.25 beats (Quarter) */
    {  6836, 1, 2,0 }, /* #166 |  97.66s | MIDI 74 | 1.17 beats (Quarter) */
    {  6885, 3, 2,0 }, /* #167 |  98.35s | MIDI 72 | 0.25 beats (Quarter) */
    {  6909, 3, 2,0 }, /* #168 |  98.70s | MIDI 72 | 0.17 beats (Quarter) */
    {  6921, 2, 2,0 }, /* #169 |  98.88s | MIDI 73 | 0.17 beats (Quarter) */
    {  6957, 1, 2,0 }, /* #170 |  99.39s | MIDI 74 | 0.17 beats (Quarter) */
    {  6957, 2, 2,0 }, /* #171 |  99.39s | MIDI 73 | 0.17 beats (Quarter) */
    {  6993, 0, 2,0 }, /* #172 |  99.90s | MIDI 75 | 0.17 beats (Quarter) */
    {  6993, 1, 2,0 }, /* #173 |  99.90s | MIDI 74 | 0.17 beats (Quarter) */
    {  7017, 0, 2,0 }, /* #174 | 100.24s | MIDI 75 | 1.17 beats (Quarter) */
    {  7065, 1, 2,0 }, /* #175 | 100.93s | MIDI 74 | 0.25 beats (Quarter) */
    {  7103, 1, 2,0 }, /* #176 | 101.47s | MIDI 74 | 0.25 beats (Quarter) */
    {  7128, 2, 2,0 }, /* #177 | 101.83s | MIDI 73 | 0.25 beats (Quarter) */
    {  7141, 1, 2,0 }, /* #178 | 102.01s | MIDI 74 | 0.25 beats (Quarter) */
    {  7166, 3, 2,0 }, /* #179 | 102.37s | MIDI 72 | 0.25 beats (Quarter) */
    {  7178, 3, 2,0 }, /* #180 | 102.55s | MIDI 72 | 0.25 beats (Quarter) */
    {  7216, 0, 2,0 }, /* #181 | 103.09s | MIDI 75 | 0.25 beats (Quarter) */
    {  7241, 0, 2,0 }, /* #182 | 103.44s | MIDI 75 | 0.25 beats (Quarter) */
    {  7253, 1, 2,0 }, /* #183 | 103.61s | MIDI 74 | 0.25 beats (Quarter) */
    {  7289, 2, 1,0 }, /* #184 | 104.13s | MIDI 73 | 1.50 beats (Half) */
    {  7350, 3, 2,0 }, /* #185 | 105.00s | MIDI 72 | 0.25 beats (Quarter) */
    {  7362, 1, 2,0 }, /* #186 | 105.18s | MIDI 74 | 0.25 beats (Quarter) */
    {  7398, 1, 2,0 }, /* #187 | 105.69s | MIDI 74 | 0.25 beats (Quarter) */
    {  7434, 1, 2,0 }, /* #188 | 106.20s | MIDI 74 | 0.25 beats (Quarter) */
    {  7458, 3, 2,0 }, /* #189 | 106.54s | MIDI 72 | 1.00 beats (Quarter) */
    {  7506, 0, 2,0 }, /* #190 | 107.23s | MIDI 75 | 0.25 beats (Quarter) */
    {  7529, 0, 2,0 }, /* #191 | 107.56s | MIDI 75 | 0.25 beats (Quarter) */
    {  7541, 3, 2,0 }, /* #192 | 107.73s | MIDI 72 | 0.25 beats (Quarter) */
    {  7577, 2, 2,0 }, /* #193 | 108.24s | MIDI 73 | 0.25 beats (Quarter) */
    {  7600, 1, 2,0 }, /* #194 | 108.57s | MIDI 74 | 0.25 beats (Quarter) */
    {  7624, 0, 2,0 }, /* #195 | 108.91s | MIDI 75 | 0.25 beats (Quarter) */
    {  7647, 0, 1,0 }, /* #196 | 109.24s | MIDI 75 | 1.83 beats (Half) */
    {  7719, 1, 1,0 }, /* #197 | 110.27s | MIDI 74 | 1.83 beats (Half) */
    {  7791, 2, 2,0 }, /* #198 | 111.30s | MIDI 73 | 0.99 beats (Quarter) */
    {  7827, 1, 2,0 }, /* #199 | 111.82s | MIDI 74 | 0.25 beats (Quarter) */
    {  7851, 0, 2,0 }, /* #200 | 112.16s | MIDI 75 | 0.25 beats (Quarter) */
    {  7863, 0, 2,0 }, /* #201 | 112.33s | MIDI 75 | 0.25 beats (Quarter) */
    {  7887, 0, 2,0 }, /* #202 | 112.68s | MIDI 75 | 0.25 beats (Quarter) */
    {  7936, 2, 2,0 }, /* #203 | 113.36s | MIDI 73 | 0.17 beats (Quarter) */
    {  7972, 1, 2,0 }, /* #204 | 113.88s | MIDI 74 | 0.17 beats (Quarter) */
    {  7996, 2, 2,0 }, /* #205 | 114.22s | MIDI 73 | 0.25 beats (Quarter) */
    {  8008, 3, 2,0 }, /* #206 | 114.39s | MIDI 72 | 0.25 beats (Quarter) */
    {  8044, 3, 2,0 }, /* #207 | 114.91s | MIDI 72 | 0.25 beats (Quarter) */
    {  8068, 2, 2,0 }, /* #208 | 115.25s | MIDI 73 | 0.25 beats (Quarter) */
    {  8080, 1, 2,0 }, /* #209 | 115.42s | MIDI 74 | 0.25 beats (Quarter) */
    {  8115, 1, 2,0 }, /* #210 | 115.94s | MIDI 74 | 0.25 beats (Quarter) */
    {  8139, 1, 1,0 }, /* #211 | 116.28s | MIDI 74 | 2.75 beats (Half) */
    {  8139, 2, 1,0 }, /* #212 | 116.28s | MIDI 73 | 2.75 beats (Half) */
    {  8282, 1, 2,0 }, /* #213 | 118.32s | MIDI 74 | 0.25 beats (Quarter) */
    {  8282, 2, 2,0 }, /* #214 | 118.32s | MIDI 73 | 0.25 beats (Quarter) */
    {  8318, 1, 2,0 }, /* #215 | 118.83s | MIDI 74 | 0.25 beats (Quarter) */
    {  8330, 1, 2,0 }, /* #216 | 118.99s | MIDI 74 | 0.25 beats (Quarter) */
    {  8330, 2, 2,0 }, /* #217 | 118.99s | MIDI 73 | 0.25 beats (Quarter) */
    {  8365, 1, 2,0 }, /* #218 | 119.50s | MIDI 74 | 0.25 beats (Quarter) */
    {  8365, 2, 2,0 }, /* #219 | 119.50s | MIDI 73 | 0.25 beats (Quarter) */
    {  8401, 1, 2,0 }, /* #220 | 120.01s | MIDI 74 | 0.25 beats (Quarter) */
    {  8424, 0, 0,0 }, /* #221 | 120.35s | MIDI 75 | 4.17 beats (Whole) */
    {  8580, 3, 2,0 }, /* #222 | 122.57s | MIDI 72 | 0.25 beats (Quarter) */
    {  8617, 1, 2,0 }, /* #223 | 123.09s | MIDI 74 | 0.25 beats (Quarter) */
    {  8653, 2, 2,0 }, /* #224 | 123.61s | MIDI 73 | 0.25 beats (Quarter) */
    {  8689, 0, 2,0 }, /* #225 | 124.13s | MIDI 75 | 0.25 beats (Quarter) */
    {  8726, 3, 2,0 }, /* #226 | 124.65s | MIDI 72 | 0.25 beats (Quarter) */
    {  8762, 1, 2,0 }, /* #227 | 125.17s | MIDI 74 | 0.25 beats (Quarter) */
    {  8798, 2, 2,0 }, /* #228 | 125.68s | MIDI 73 | 0.25 beats (Quarter) */
    {  8834, 1, 2,0 }, /* #229 | 126.20s | MIDI 74 | 0.25 beats (Quarter) */
    {  8870, 2, 2,0 }, /* #230 | 126.71s | MIDI 73 | 0.25 beats (Quarter) */
    {  8905, 0, 2,0 }, /* #231 | 127.22s | MIDI 75 | 0.25 beats (Quarter) */
    {  8941, 1, 2,0 }, /* #232 | 127.72s | MIDI 74 | 0.25 beats (Quarter) */
    {  8976, 0, 2,0 }, /* #233 | 128.23s | MIDI 75 | 0.25 beats (Quarter) */
    {  9012, 2, 2,0 }, /* #234 | 128.74s | MIDI 73 | 0.25 beats (Quarter) */
    {  9047, 0, 2,0 }, /* #235 | 129.24s | MIDI 75 | 0.25 beats (Quarter) */
    {  9082, 1, 2,0 }, /* #236 | 129.75s | MIDI 74 | 0.25 beats (Quarter) */
    {  9109, 1, 2,0 }, /* #237 | 130.13s | MIDI 74 | 0.24 beats (Quarter) */
    {  9118, 0, 2,0 }, /* #238 | 130.26s | MIDI 75 | 0.25 beats (Quarter) */
    {  9153, 0, 2,0 }, /* #239 | 130.76s | MIDI 75 | 1.00 beats (Quarter) */
    {  9153, 1, 2,0 }, /* #240 | 130.76s | MIDI 74 | 1.00 beats (Quarter) */
    {  9224, 0, 2,0 }, /* #241 | 131.77s | MIDI 75 | 0.25 beats (Quarter) */
    {  9259, 1, 2,0 }, /* #242 | 132.28s | MIDI 74 | 0.25 beats (Quarter) */
    {  9295, 1, 1,0 }, /* #243 | 132.78s | MIDI 74 | 1.50 beats (Half) */
    {  9295, 2, 1,0 }, /* #244 | 132.78s | MIDI 73 | 1.50 beats (Half) */
    {  9365, 0, 2,0 }, /* #245 | 133.78s | MIDI 75 | 0.25 beats (Quarter) */
    {  9388, 1, 2,0 }, /* #246 | 134.11s | MIDI 74 | 0.25 beats (Quarter) */
    {  9400, 1, 2,0 }, /* #247 | 134.28s | MIDI 74 | 0.25 beats (Quarter) */
    {  9423, 2, 2,0 }, /* #248 | 134.61s | MIDI 73 | 0.25 beats (Quarter) */
    {  9435, 2, 1,0 }, /* #249 | 134.78s | MIDI 73 | 1.83 beats (Half) */
    {  9505, 1, 2,0 }, /* #250 | 135.78s | MIDI 74 | 0.25 beats (Quarter) */
    {  9528, 2, 2,0 }, /* #251 | 136.11s | MIDI 73 | 0.25 beats (Quarter) */
    {  9540, 1, 2,0 }, /* #252 | 136.28s | MIDI 74 | 0.25 beats (Quarter) */
    {  9563, 2, 2,0 }, /* #253 | 136.61s | MIDI 73 | 0.25 beats (Quarter) */
    {  9575, 3, 1,0 }, /* #254 | 136.78s | MIDI 72 | 1.83 beats (Half) */
    {  9645, 0, 2,0 }, /* #255 | 137.78s | MIDI 75 | 0.99 beats (Quarter) */
    {  9681, 1, 2,0 }, /* #256 | 138.29s | MIDI 74 | 0.25 beats (Quarter) */
    {  9717, 1, 2,0 }, /* #257 | 138.81s | MIDI 74 | 0.25 beats (Quarter) */
    {  9753, 1, 2,0 }, /* #258 | 139.32s | MIDI 74 | 0.25 beats (Quarter) */
    {  9777, 2, 2,0 }, /* #259 | 139.67s | MIDI 73 | 0.25 beats (Quarter) */
    {  9789, 1, 1,0 }, /* #260 | 139.84s | MIDI 74 | 1.75 beats (Half) */
    {  9789, 2, 1,0 }, /* #261 | 139.84s | MIDI 73 | 1.75 beats (Half) */
    {  9860, 3, 2,0 }, /* #262 | 140.86s | MIDI 72 | 0.25 beats (Quarter) */
    {  9896, 2, 2,0 }, /* #263 | 141.36s | MIDI 73 | 0.25 beats (Quarter) */
    {  9919, 1, 2,0 }, /* #264 | 141.70s | MIDI 74 | 0.25 beats (Quarter) */
    {  9931, 1, 1,0 }, /* #265 | 141.87s | MIDI 74 | 1.75 beats (Half) */
    {  9931, 2, 1,0 }, /* #266 | 141.87s | MIDI 73 | 1.75 beats (Half) */
    { 10002, 1, 2,0 }, /* #267 | 142.89s | MIDI 74 | 0.25 beats (Quarter) */
    { 10038, 2, 2,0 }, /* #268 | 143.40s | MIDI 73 | 0.25 beats (Quarter) */
    { 10074, 0, 2,0 }, /* #269 | 143.91s | MIDI 75 | 0.25 beats (Quarter) */
    { 10074, 1, 2,0 }, /* #270 | 143.91s | MIDI 74 | 0.25 beats (Quarter) */
    { 10109, 1, 2,0 }, /* #271 | 144.41s | MIDI 74 | 0.25 beats (Quarter) */
    { 10144, 0, 2,0 }, /* #272 | 144.91s | MIDI 75 | 0.17 beats (Quarter) */
    { 10144, 1, 2,0 }, /* #273 | 144.91s | MIDI 74 | 0.17 beats (Quarter) */
    { 10161, 0, 1,0 }, /* #274 | 145.16s | MIDI 75 | 1.75 beats (Half) */
    { 10237, 1, 0,0 }, /* #275 | 146.25s | MIDI 74 | 3.08 beats (Whole) */
    { 10237, 3, 0,0 }, /* #276 | 146.25s | MIDI 72 | 3.08 beats (Whole) */
    { 10353, 2, 2,0 }, /* #277 | 147.90s | MIDI 73 | 0.25 beats (Quarter) */
    { 10388, 0, 2,0 }, /* #278 | 148.41s | MIDI 75 | 0.25 beats (Quarter) */
    { 10424, 1, 2,0 }, /* #279 | 148.91s | MIDI 74 | 0.25 beats (Quarter) */
    { 10459, 0, 2,0 }, /* #280 | 149.42s | MIDI 75 | 0.25 beats (Quarter) */
    { 10495, 3, 2,0 }, /* #281 | 149.92s | MIDI 72 | 0.25 beats (Quarter) */
    { 10530, 1, 2,0 }, /* #282 | 150.43s | MIDI 74 | 0.25 beats (Quarter) */
    { 10566, 2, 2,0 }, /* #283 | 150.94s | MIDI 73 | 0.25 beats (Quarter) */
    { 10601, 1, 2,0 }, /* #284 | 151.44s | MIDI 74 | 0.25 beats (Quarter) */
    { 10636, 1, 2,0 }, /* #285 | 151.95s | MIDI 74 | 0.25 beats (Quarter) */
    { 10636, 2, 2,0 }, /* #286 | 151.95s | MIDI 73 | 0.17 beats (Quarter) */
    { 10660, 1, 1,0 }, /* #287 | 152.28s | MIDI 74 | 2.33 beats (Half) */
    { 10660, 3, 1,0 }, /* #288 | 152.28s | MIDI 72 | 2.33 beats (Half) */
    { 10881, 0, 0,0 }, /* #289 | 155.44s | MIDI 75 | 3.50 beats (Whole) */
    { 10881, 1, 0,0 }, /* #290 | 155.44s | MIDI 74 | 3.50 beats (Whole) */
    { 11010, 1, 2,0 }, /* #291 | 157.29s | MIDI 74 | 0.25 beats (Quarter) */
    { 11022, 0, 2,0 }, /* #292 | 157.45s | MIDI 75 | 0.25 beats (Quarter) */
    { 11022, 1, 2,0 }, /* #293 | 157.45s | MIDI 74 | 0.25 beats (Quarter) */
    { 11045, 0, 0,0 }, /* #294 | 157.79s | MIDI 75 | 4.33 beats (Whole) */
    { 11045, 1, 0,0 }, /* #295 | 157.79s | MIDI 74 | 4.33 beats (Whole) */
    { 11301, 0, 2,0 }, /* #296 | 161.44s | MIDI 75 | 0.17 beats (Quarter) */
    { 11301, 1, 2,0 }, /* #297 | 161.44s | MIDI 74 | 0.17 beats (Quarter) */
    { 11324, 0, 0,0 }, /* #298 | 161.78s | MIDI 75 | 3.00 beats (Whole) */
    { 11324, 1, 0,0 }, /* #299 | 161.78s | MIDI 74 | 3.00 beats (Whole) */
    { 11441, 0, 2,0 }, /* #300 | 163.44s | MIDI 75 | 0.17 beats (Quarter) */
    { 11441, 1, 2,0 }, /* #301 | 163.44s | MIDI 74 | 0.17 beats (Quarter) */
    { 11464, 0, 0,0 }, /* #302 | 163.78s | MIDI 75 | 3.00 beats (Whole) */
    { 11464, 1, 0,0 }, /* #303 | 163.78s | MIDI 74 | 3.00 beats (Whole) */
    { 11581, 0, 2,0 }, /* #304 | 165.44s | MIDI 75 | 0.17 beats (Quarter) */
    { 11581, 1, 2,0 }, /* #305 | 165.44s | MIDI 74 | 0.17 beats (Quarter) */
    { 11604, 0, 0,0 }, /* #306 | 165.78s | MIDI 75 | 3.00 beats (Whole) */
    { 11604, 1, 0,0 }, /* #307 | 165.78s | MIDI 74 | 3.00 beats (Whole) */
    { 11720, 0, 2,0 }, /* #308 | 167.44s | MIDI 75 | 0.17 beats (Quarter) */
    { 11720, 1, 2,0 }, /* #309 | 167.44s | MIDI 74 | 0.17 beats (Quarter) */
    { 11744, 0, 0,0 }, /* #310 | 167.77s | MIDI 75 | 3.00 beats (Whole) */
    { 11744, 1, 0,0 }, /* #311 | 167.77s | MIDI 74 | 3.00 beats (Whole) */
    { 11859, 0, 2,0 }, /* #312 | 169.42s | MIDI 75 | 0.17 beats (Quarter) */
    { 11859, 1, 2,0 }, /* #313 | 169.42s | MIDI 74 | 0.17 beats (Quarter) */
    { 11883, 0, 2,0 }, /* #314 | 169.75s | MIDI 75 | 0.83 beats (Quarter) */
    { 11883, 1, 2,0 }, /* #315 | 169.75s | MIDI 74 | 0.83 beats (Quarter) */
    { 11941, 1, 2,0 }, /* #316 | 170.58s | MIDI 74 | 0.25 beats (Quarter) */
    { 11952, 1, 2,0 }, /* #317 | 170.75s | MIDI 74 | 0.25 beats (Quarter) */
    { 11976, 2, 2,0 }, /* #318 | 171.08s | MIDI 73 | 0.25 beats (Quarter) */
    { 11987, 0, 2,0 }, /* #319 | 171.24s | MIDI 75 | 0.25 beats (Quarter) */
    { 12010, 1, 2,0 }, /* #320 | 171.58s | MIDI 74 | 0.25 beats (Quarter) */
    { 12022, 1, 2,0 }, /* #321 | 171.74s | MIDI 74 | 0.25 beats (Quarter) */
    { 12045, 2, 2,0 }, /* #322 | 172.08s | MIDI 73 | 0.25 beats (Quarter) */
    { 12057, 0, 2,0 }, /* #323 | 172.24s | MIDI 75 | 0.25 beats (Quarter) */
    { 12080, 1, 2,0 }, /* #324 | 172.58s | MIDI 74 | 0.25 beats (Quarter) */
    { 12092, 1, 2,0 }, /* #325 | 172.74s | MIDI 74 | 0.25 beats (Quarter) */
    { 12115, 2, 2,0 }, /* #326 | 173.08s | MIDI 73 | 0.25 beats (Quarter) */
    { 12127, 0, 2,0 }, /* #327 | 173.24s | MIDI 75 | 0.25 beats (Quarter) */
    { 12150, 1, 2,0 }, /* #328 | 173.58s | MIDI 74 | 0.25 beats (Quarter) */
    { 12162, 1, 2,0 }, /* #329 | 173.74s | MIDI 74 | 0.25 beats (Quarter) */
    { 12185, 2, 2,0 }, /* #330 | 174.08s | MIDI 73 | 0.25 beats (Quarter) */
    { 12197, 0, 2,0 }, /* #331 | 174.24s | MIDI 75 | 0.25 beats (Quarter) */
    { 12220, 1, 2,0 }, /* #332 | 174.58s | MIDI 74 | 0.25 beats (Quarter) */
    { 12232, 1, 2,0 }, /* #333 | 174.74s | MIDI 74 | 0.25 beats (Quarter) */
    { 12255, 2, 2,0 }, /* #334 | 175.08s | MIDI 73 | 0.25 beats (Quarter) */
    { 12267, 0, 2,0 }, /* #335 | 175.24s | MIDI 75 | 0.25 beats (Quarter) */
    { 12290, 1, 2,0 }, /* #336 | 175.58s | MIDI 74 | 0.25 beats (Quarter) */
    { 12302, 1, 2,0 }, /* #337 | 175.74s | MIDI 74 | 0.25 beats (Quarter) */
    { 12325, 2, 2,0 }, /* #338 | 176.08s | MIDI 73 | 0.25 beats (Quarter) */
    { 12337, 0, 2,0 }, /* #339 | 176.24s | MIDI 75 | 0.25 beats (Quarter) */
    { 12360, 1, 2,0 }, /* #340 | 176.58s | MIDI 74 | 0.25 beats (Quarter) */
    { 12372, 1, 2,0 }, /* #341 | 176.74s | MIDI 74 | 0.25 beats (Quarter) */
    { 12395, 2, 2,0 }, /* #342 | 177.08s | MIDI 73 | 0.25 beats (Quarter) */
    { 12407, 0, 2,0 }, /* #343 | 177.24s | MIDI 75 | 0.25 beats (Quarter) */
    { 12430, 1, 2,0 }, /* #344 | 177.58s | MIDI 74 | 0.25 beats (Quarter) */
    { 12442, 1, 2,0 }, /* #345 | 177.74s | MIDI 74 | 0.25 beats (Quarter) */
    { 12465, 2, 2,0 }, /* #346 | 178.08s | MIDI 73 | 0.25 beats (Quarter) */
    { 12477, 0, 2,0 }, /* #347 | 178.24s | MIDI 75 | 0.25 beats (Quarter) */
    { 12500, 1, 2,0 }, /* #348 | 178.58s | MIDI 74 | 0.25 beats (Quarter) */
    { 12512, 1, 2,0 }, /* #349 | 178.75s | MIDI 74 | 0.25 beats (Quarter) */
    { 12536, 2, 2,0 }, /* #350 | 179.08s | MIDI 73 | 0.25 beats (Quarter) */
    { 12547, 0, 2,0 }, /* #351 | 179.25s | MIDI 75 | 0.25 beats (Quarter) */
    { 12571, 1, 2,0 }, /* #352 | 179.58s | MIDI 74 | 0.25 beats (Quarter) */
    { 12582, 1, 2,0 }, /* #353 | 179.75s | MIDI 74 | 0.25 beats (Quarter) */
    { 12606, 2, 2,0 }, /* #354 | 180.08s | MIDI 73 | 0.25 beats (Quarter) */
    { 12618, 0, 2,0 }, /* #355 | 180.25s | MIDI 75 | 0.25 beats (Quarter) */
    { 12641, 1, 2,0 }, /* #356 | 180.59s | MIDI 74 | 0.25 beats (Quarter) */
    { 12653, 1, 2,0 }, /* #357 | 180.75s | MIDI 74 | 0.25 beats (Quarter) */
    { 12676, 2, 2,0 }, /* #358 | 181.09s | MIDI 73 | 0.25 beats (Quarter) */
    { 12688, 0, 2,0 }, /* #359 | 181.26s | MIDI 75 | 0.25 beats (Quarter) */
    { 12711, 1, 2,0 }, /* #360 | 181.59s | MIDI 74 | 0.25 beats (Quarter) */
    { 12723, 1, 2,0 }, /* #361 | 181.76s | MIDI 74 | 0.25 beats (Quarter) */
    { 12746, 2, 2,0 }, /* #362 | 182.09s | MIDI 73 | 0.25 beats (Quarter) */
    { 12758, 0, 2,0 }, /* #363 | 182.26s | MIDI 75 | 0.25 beats (Quarter) */
    { 12782, 1, 2,0 }, /* #364 | 182.59s | MIDI 74 | 0.25 beats (Quarter) */
    { 12793, 1, 2,0 }, /* #365 | 182.76s | MIDI 74 | 0.25 beats (Quarter) */
    { 12817, 2, 2,0 }, /* #366 | 183.10s | MIDI 73 | 0.25 beats (Quarter) */
    { 12828, 0, 2,0 }, /* #367 | 183.26s | MIDI 75 | 0.25 beats (Quarter) */
    { 12852, 1, 2,0 }, /* #368 | 183.60s | MIDI 74 | 0.25 beats (Quarter) */
    { 12864, 1, 0,0 }, /* #369 | 183.77s | MIDI 74 | 3.17 beats (Whole) */
    { 12981, 0, 2,0 }, /* #370 | 185.44s | MIDI 75 | 0.17 beats (Quarter) */
    { 12981, 1, 2,0 }, /* #371 | 185.44s | MIDI 74 | 0.17 beats (Quarter) */
    { 13016, 0, 0,0 }, /* #372 | 185.94s | MIDI 75 | 16.50 beats (Whole) */
    { 13016, 1, 0,0 }  /* #373 | 185.94s | MIDI 74 | 16.50 beats (Whole) */
};

int main()
{
    static Model model;
    UINT32 *buff_1, *buff_2;
    UINT32 time_then, time_now, time_elapsed;
    UINT8 quit, buffer;

    disable_cursor();

    quit = 0;
    buffer = 0;
    buff_1 = get_pbase();
    buff_2 = buff_256();

    /* Initialize model */
    time_then = get_time();
    model_init(&model, song, SONG_NOTES, time_then);
    /* Render model */
    render(buff_1, &model);
    render(buff_2, &model);
    while (!quit)
    {
        time_now = get_time();
        time_elapsed = time_now - time_then;
        /* If input is pending */
        if (waiting_input())
        {
            /* Process async events */
            if (asynch_is_hit(&model, get_input()))
                asynch_apply_hit(&model);
            else
                asynch_apply_miss(&model);
        }
        /* If clock has ticked*/
        if (time_elapsed > 0)
        {
            /* Process sync events */
            synch_clock_tick(&model, time_now);
            /* Process cond events */
            cond_check_misses(&model);
            quit = cond_game_over(&model);
            /* Render frame */
            if (buffer == 0)
            {
                print_debug(buff_2, "Ticks:", time_elapsed);
                render(buff_2, &model);
                set_screen(buff_2);
                buffer = 1;
            }
            else
            {
                print_debug(buff_1, "Ticks:", time_elapsed);
                render(buff_1, &model);
                set_screen(buff_1);
                buffer = 0;
            }
            /* Update game module clock */
            time_then = time_now;
        }
    }

    set_screen(buff_1);

    return 0;
}