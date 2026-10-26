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

; note_half_head[NOTE_HEAD_HEIGHT]
; The head of a half note, requires a stem.
; 16x22 with a counter (hollow inset).
		xdef		_note_half_head
_note_half_head:
		incbin		"notehead\note_hal.bin"

; note_black_head[NOTE_HEAD_HEIGHT]
; A solid black note head, requires a stem.
; Used for quarter, eighth, sixteenth, and shorter notes.
		xdef		_note_black_head
_note_black_head:
		incbin		"notehead\note_blk.bin"


; =============== ENJOYMENT METER ===============
; A saxophone in the top right that is filled
; according to the enjoyment meter level.

		xdef		_emp_left
_emp_left:	
		incbin		"meter\emp_left.bin"

		xdef		_emp_right
_emp_right:	
		incbin		"meter\emp_rght.bin"

		xdef		_full_left
_full_left:	
		incbin		"meter\ful_left.bin"

		xdef		_full_right
_full_right:	
		incbin		"meter\ful_rght.bin"

; ===============  BACKGROUNDS  ===============
; 640px wide bitmaps to plot in their entirety.
		xdef		_bg
_bg:		
		incbin		"bg\bg.bin"

; ===============  TARGETS  ===============
