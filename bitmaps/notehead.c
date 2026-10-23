/*
    Peter Ursem
    purse327@mtroyal.ca

    Logan Campbell
    lcamp702@mtroyal.ca

    COMP2659-001
    Steve Kalmar

    In the Pocket - Checkpoint 2
    Due: Oct 19, 2026

    Definitions of the note head bitmaps used by the renderer.
    Each note head is a 16px tall shape. The widest form is 32px wide, narrower shapes are center aligned.
    Each array holds one entry per pixel row; the *_HEIGHT constants in bitmaps.h give the row counts.

    bitmaps/notehead.c
*/

#include "bitmaps.h"

/*
A whole note, no stem.
16x32 with a counter (hollow inset).
*/
const UINT32 note_head_whole[NOTE_HEAD_HEIGHT] = {
    0X001FF800, 0X01FFFF80, 0X0FE0FFF0, 0X1F801FF8,
    0X3F0007FC, 0X7F0003FE, 0XFF0001FF, 0XFF0001FF,
    0XFF8000FF, 0XFF8000FF, 0X7FC000FE, 0X3FF000FC,
    0X1FF801F8, 0X0FFF07F0, 0X01FFFF80, 0X001FF800};

/*
The head of a half note, requires a stem.
16x22 with a counter (hollow inset).
*/
const UINT32 note_head_half[NOTE_HEAD_HEIGHT] = {
    0X0001FC00, 0X000FFF80, 0X003FFFC0, 0X007FC1C0,
    0X00FE00E0, 0X01F800E0, 0X03F001E0, 0X03C001E0,
    0X078003C0, 0X07800FC0, 0X07001F80, 0X07007F00,
    0X0383FE00, 0X03FFFC00, 0X01FFF000, 0X003F8000};

/*
A solid black note head, requires a stem.
Used for quarter, eighth, sixteenth, and shorter notes.
*/
const UINT32 note_head_black[NOTE_HEAD_HEIGHT] = {
    0X0001FC00, 0X000FFF80, 0X003FFFC0, 0X007FFFC0,
    0X00FFFFE0, 0X01FFFFE0, 0X03FFFFE0, 0X03FFFFE0,
    0X07FFFFC0, 0X07FFFFC0, 0X07FFFF80, 0X07FFFF00,
    0X03FFFE00, 0X03FFFC00, 0X01FFF000, 0X003F8000};