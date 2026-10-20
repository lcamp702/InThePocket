/*
	Peter Ursem
	purse327@mtroyal.ca

	Logan Campbell
	lcamp702@mtroyal.ca

	COMP2659-001
	Steve Kalmar

	In the Pocket - Checkpoint 2
	Due: Oct 19, 2026

	Implementation file for the render module. Contains function definitions for the functions in render.h.
	
    render.c*/

#include "render.h"
#include "bitmaps.h"
#include "../raster/raster.h"

#define STAFF_LINES        5    /* lines per staff */
#define STAFF_LINE_GAP     6    /* rows between lines in one staff */
#define STAFF_HEIGHT       24   /* (STAFF_LINES - 1) * STAFF_LINE_GAP */
#define STAFF_SPACING      40   /* rows from one staff's top line to the next */
#define LOWEST_STAFF_TOP   372  /* top line of the bottom staff (pitch 0) */
#define LANE_COUNT         4    /* one staff per pitch (KEY_A..KEY_F) */
#define SCREEN_WIDTH       640

static int staff_top(UINT8 pitch);
/*------ Function: render ------
PURPOSE: Master render function. Calls the other render functions, and renders the game world onto the screen.

INPUT: pointer to Model, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void render(Model *model, UINT32 *base){
    int i;
    clear_screen(base);

    render_staff(base); /*must render the track first for it to appear in the background*/

    for(i=model->head;i<model_last_on_screen(model); i++){ /*model_last_on_screen returns index of first note off screen. it needs to be renamed probably but it is what it is.*/
        render_note(&model->notes[i], base);
    }

    render_meter(&model->meter, base);

}
/*------ Function: render_note ------
PURPOSE: Renders a single note to screen. It's place in the staff is determined by its pitch.

INPUT: pointer to Note, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void render_note(Note *note, UINT32 *base){

    int row;

    row = staff_top(note->pitch) + (STAFF_HEIGHT / 2); /* middle line of the note's staff */
    row -= NOTE16_HEAD_ROW;                             /* centre the notehead on that line */
    plot_16bit_bitmap((UINT16 *)base, row, note->x, note16_bmp, NOTE16_HEIGHT);
}
/*------ Function: render_meter ------
PURPOSE: Renders meter. Fills it up based on enjoyment.

INPUT: pointer to Meter, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void render_meter(Meter *meter, UINT32 *base){
    int i;

    plot_rectangle(base, 40, 520, 100, 80);
    for(i=0; i<meter->enjoyment; i++){ /*rn this function is linear time. There has to be a faster way.*/
        plot_horizontal_line(base, 140-i, 520, 80);
    }
}
/*------ Function: render_staff ------
PURPOSE: Renders one five-line staff for each pitch lane, one above the other.
         Must be drawn before the notes so the notes appear on top of it.

INPUT: UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void render_staff(UINT32 *base){
    UINT8 lane;
    int line, row;

    for(lane=0; lane<LANE_COUNT; lane++){
        row = staff_top(lane);
        for(line=0; line<STAFF_LINES; line++){
            plot_horizontal_line(base, row, 0, SCREEN_WIDTH);
            row += STAFF_LINE_GAP;
        }
    }
}

/*------ Function: staff_top ------
PURPOSE: Gives the screen row of the top line of the staff for a pitch lane.
         Pitch 0 is the bottom staff; each higher pitch is one staff higher.

INPUT: pitch (lane number, 0 to LANE_COUNT - 1)

OUTPUT: Returns the row of that staff's top line.
*/
static int staff_top(UINT8 pitch){
    return LOWEST_STAFF_TOP - (pitch * STAFF_SPACING);
}
