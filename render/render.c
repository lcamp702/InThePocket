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

    /*TODO: FIX STAFF (SPACING TO NOTE HEAD, WE WANT NOTEHEAD TO BE CENTERED ON LINE) DONE
            TARGETS TURN ON WHEN BUTTON PRESS
            DRAW TARGET
            DISPLAY SCORE (DONE)
    */

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
#define NOTEHEAD_OFFSET    25
#define SCORE_OFFSET       5
#define SCORE_MAX_LEN      20   /*this is almost certainly far too large but we can change it if needed*/

/*------ Function: render ------
PURPOSE: Master render function. Calls the other render functions, and renders the game world onto the screen.

INPUT: pointer to Model, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void render(Model *model, UINT32 *base){
    int i;
    clear_screen(base);

    render_staff(base); /*must render the staff first for it to appear in the background*/

    for(i=model->head;i<model_last_on_screen(model); i++){ /*model_last_on_screen returns index of first note off screen. it needs to be renamed probably but it is what it is.*/
        render_note(&model->notes[i], base);
    }
    render_targets(base, model->targets);

    render_meter(&model->meter, base);

}
/*------ Function: render_note ------
PURPOSE: Renders a single note to screen. It's place in the staff is determined by its pitch.

INPUT: pointer to Note, UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void render_note(Note *note, UINT32 *base){

    int row;

    row = LOWEST_NOTE - (note->pitch * NOTE_SPACING); 
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
    render_score(base, 140, 520, meter->score);
}
/*------ Function: render_staff ------
PURPOSE: Renders one four line staff. 

INPUT: UINT32 pointer to frame buffer

OUTPUT: N/A

*/
void render_staff(UINT32 *base){
   
    int row,i;
    
    for(i = 0; i <4; i++){
        row = LOWEST_NOTE - (i * NOTE_SPACING); 
        row -= NOTE16_HEAD_ROW;                  /* centre the notehead on that line */
        row+=NOTEHEAD_OFFSET;
        plot_horizontal_line(base, row, 0, SCREEN_WIDTH);
    }
}

/*------ Function: render_target ------
PURPOSE: Renders one target. 

INPUT: UINT32 pointer to frame buffer

OUTPUT: N/A
*/
void render_targets(UINT32 *base, UINT8 * targets){
    int row,i;
    for(i=0;i<4;i++){
        row = LOWEST_NOTE - (i * NOTE_SPACING); 
        row -= NOTE16_HEAD_ROW;           
        row+=NOTEHEAD_OFFSET;
        row-=TARGET_HEIGHT/2;                  
        if(targets[i]){
            plot_16bit_bitmap((UINT16 *)base, row, 40, target_on_bmp, TARGET_HEIGHT);
        }
        else{
            plot_16bit_bitmap((UINT16 *)base, row, 40, target_off_bmp, TARGET_HEIGHT);
        }
        
    }
}

/*------ Function: render_score ------
PURPOSE: Renders the score. 

INPUT: UINT32 pointer to frame buffer, int for row, int for col, UINT32 score value (which is to be printed)

OUTPUT: N/A
*/

void render_score(UINT32 *base, int row, int col, UINT32 score){
    char s[SCORE_MAX_LEN];
    char temp;
    int i,j;
    i=0;
    do{
        s[i++]= score % 10 + '0';
        score/=10;
    }while(score>0);
    s[i]='\0';
    for(j=i-1, i=0; i<j; i++, j--){
        temp = s[i];
        s[i]=s[j];
        s[j]=temp;
    }
    plot_string((UINT8 *)base, row+SCORE_OFFSET, col, s);
}