;	Peter Ursem
;	purse327@mtroyal.ca

;	Logan Campbell
;	lcamp702@mtroyal.ca

;	COMP2659-001
;	Steve Kalmar

;	In the Pocket - Checkpoint 3
;	Due: Nov 9, 2026
;	raster/bitmap.s    

;	Status:
;	  Not Tested:
;
;	  Visual Tests:
;		bg
;		note_whole_head
;		note_half_head
;		note_black_head
;		emp_left
;		emp_right
;		full_left
;		full_right

;	Purpose:
;	Define bitmaps from binary files

; ===============  BACKGROUNDS  ===============
; 640px wide bitmaps to plot in their entirety.
; UINT32 bg[BG_HEIGHT]
		xdef		_bg
_bg:		
		incbin		"bg\bg.bin"

; ===============  DOUGS  ===============
; Animation frames for the character on screen
; UINT32 bg[BG_HEIGHT]
		xdef		_doug0
_doug0:		
		incbin		"doug\doug0.bin"

		xdef		_doug1
_doug1:		
		incbin		"doug\doug1.bin"

		xdef		_doug2
_doug2:		
		incbin		"doug\doug2.bin"

		xdef		_doug3
_doug3:		
		incbin		"doug\doug3.bin"

		xdef		_doug4
_doug4:		
		incbin		"doug\doug4.bin"

; ===============  NOTE HEADS  ===============
; Each note head is a 16px tall shape. 
; The widest form is 32px wide,
; narrower shapes are center aligned.

; UINT32 note_whole_head[[NOTE_HEAD_HEIGHT]];
; A whole note, no stem.
; 16x32 with a counter (hollow inset).
		xdef		_note_whole_head
_note_whole_head:
		incbin		"notehead\note_who.bin"

; UINT32 note_half_head[NOTE_HEAD_HEIGHT]
; The head of a half note, requires a stem.
; 16x22 with a counter (hollow inset).
		xdef		_note_half_head
_note_half_head:
		incbin		"notehead\note_hal.bin"

; UINT32 note_black_head[NOTE_HEAD_HEIGHT]
; A solid black note head, requires a stem.
; Used for quarter, eighth, sixteenth, and shorter notes.
		xdef		_note_black_head
_note_black_head:
		incbin		"notehead\note_blk.bin"


; =============== ENJOYMENT METER ===============
; A saxophone in the top right that is filled
; according to the enjoyment meter level. Each
; bitmap is 64px wide, split in half to render
; with the plot_32bit_bitmap function.

; UINT32 emp_left[METER_HEIGHT], UINT32 emp_right[METER_HEIGHT]
; To be plotted from the top showing 
; the empty portion of the meter
		xdef		_sax_empty
_sax_empty:	
		incbin		"meter\sax_empt.bin"

; UINT32 full_left[METER_HEIGHT], UINT32 full_right[METER_HEIGHT]
; To be plotted from the bottom showing 
; the full portion of the meter
		xdef		_sax_full
_sax_full:	
		incbin		"meter\sax_full.bin"