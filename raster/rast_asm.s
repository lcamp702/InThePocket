;	Peter Ursem
;	purse327@mtroyal.ca

;	Logan Campbell
;	lcamp702@mtroyal.ca

;	COMP2659-001
;	Steve Kalmar

;	In the Pocket - Checkpoint 2
;	Due: Oct 19, 2026
;	rast_asm.s    

;	Status:
;	  Not Tested:
;	  Visual Tests:
;		clear_screen
;		plot_pixel

;	Purpose:
;	A raster engine for our game, In the Pocket on the Atari ST.

		xdef		_clear_screen
		xdef		_plot_pixel
		xdef		_plot_vertical_line

;       Screen details
ROWS		equ		400

COLS		equ		640
BYTE_COLS	equ		80
LONG_COLS	equ		20

;	_clear_screen(UINT32 *base)
cs_base		equ		64	; Offset from SP, not A6

;	_plot_pixel(UINT8 *base, int row, int col)
px_base		equ		20	; 4 + (4*4) bytes in saved registers
px_row		equ		24	; 8 + (4*4) bytes in saved registers
px_col		equ		26	; 10 + (4*4) bytes in saved registers

;	_plot_vertical_line(UINT32 *base, int row, int col, UINT16 length)
vl_base		equ		28
vl_row		equ		32
vl_col		equ		34
vl_len		equ		36

; ==== void clear_screen(UINT32 *base) ====
; PURPOSE: Clears the entire screen.
; INPUT: Address(UINT32*): to the start of the screen
; OUTPUT: None
_clear_screen:	; Quick clear code from Raster Lab on D2L
		movem.l	d0-7/a0-6,-(sp) ; Store existing register values

		lea		zeros,a0
		movem.l		(a0)+,d1-7/a1-6	; Clear registers d1-7 and a1-6

		movea.l		cs_base(sp),a0	; a0 points framebuffer base
		adda.l		#32000,a0	; a0 points to end of framebuffer
		move.w		#614,d0	; Set loop counter to 614 (615 * 13 = 7995 longs)
fill_loop:
		movem.l		d1-7/a1-6,-(a0) ; Clear 13 longs at once and decrement
		dbra		d0,fill_loop
	; End loop
		movem.l		d1-5,-(a0)	; Clear remaining 5 longs

		movem.l		(sp)+,d0-7/a0-6 ; Return existing register values
		rts

; ==== void plot_pixel(UINT8* base, int row, int col) ====
; PURPOSE: Plots a single pixel on the screen.
; INPUT: Address(UINT8*): to the start of the screen
;       Position(row,col): the location of the pixel to plot
; OUTPUT: None
_plot_pixel:
		movem.l		d0-2/a0,-(sp)

		move.w		px_row(sp),d0	; d0.w = Row #
		; Bound check for row (bhs allows testing d0 < 0 and d0 > ROWS with carry bit)
		cmp.w		#ROWS,d0
		bhs.b		pp_end

		move.w		px_col(sp),d1	; d1.w = Column #
		; Bound check for col
		cmp.w		#COLS,d1
		bhs.b		pp_end

		; Multiplication broken into powers of two
		move.w		d0,d2		; d2.w = Row #
		lsl.w		#4,d0		; d0.w = row * 16
		lsl.w		#6,d2		; d2.w = row * 64
		add.w		d2,d0		; d0.w = row * 80 = row * BYTE_COLS
		
		move.w		d1,d2		; d2.w = Column #
		not.w		d2		; d2 (lowest 3 bits) = Column bit remainder (col % 8)
						; Works because bset source is mod 8. (https://68k.hax.com/BSET)
		
		lsr.w		#3,d1		; d1.w = Col byte offset (col / 8)
		add.w		d1,d0		; d0.w = Total byte offset


		movea.l		px_base(sp),a0	; a0 points to framebuffer base
		bset.b		d2,0(a0,d0.w)	; Set specific bit in memory
pp_end:
		movem.l		(sp)+,d0-2/a0
		rts

; ==== void plot_vertical_line(UINT32 *base, int row, int col, UINT16 length) ====
; PURPOSE: Plot a vertical line on the screen. The vertical line is specified by the topmost pixel of the line and the length of the line.
; INPUT: Address(UINT32*): to the start of the screen
;        Position(row,col): the coordinates of the topmost pixel of the vertical line
;        Length: the length in pixels of the line
; OUTPUT: None
_plot_vertical_line:
		movem.l		d0-4/a0,-(sp)

		move.w		vl_row(sp),d0	; d0.w = Row #
		; Bound check for row
		cmp.w		#ROWS,d0
		bhs.b		vl_end

		move.w		vl_col(sp),d1	; d1.w = Column #
		; Bound check for col
		cmp.w		#COLS,d1
		bhs.b		vl_end

		; Zero length check
		move.w		vl_len(sp),d3	; d3.w = length
		ble.b		vl_end
				
		; Row clipping check
		move.w		d0,d2
		add.w		d3,d2		; d2.w = end row
		
		cmp.w		#ROWS,d2
		bls.b		vl_fits
		move.w		#ROWS,d3	; If end row >= rows, clip line length
		sub.w		d0,d3

vl_fits:	; Line fits on screen
		move.w		d1,d2		; d2.w = Column #

		move.w		d0,d4		; d4.w = Row #
		lsl.w		#4,d0		; d0.w = row * 16
		lsl.w		#6,d4		; d4.w = row * 64
		add.w		d4,d0		; d0.w = row * BYTE_COLS (Row offset)

		not.w		d2		; d2 (lowest 3 bits) = Column bit set index
		lsr.w		#3,d1		; d1.w = Col byte offset
		add.w		d1,d0		; d0.w = Total byte offset

		move.l		vl_base(sp),a0	; a0.l = Framebuffer base pointer
		adda.w		d0,a0		; a0.l = fb cursor at first byte to change

		move.w		#BYTE_COLS,d4	; Keep in register for speed
		subq.w		#1,d3
vl_loop:	; Loop over all rows
		bset.b		d2,(a0)
		adda.w		d4,a0		; Move fb pointer to next row

		dbra		d3,vl_loop	; Count down length and loop
vl_end:
		movem.l		(sp)+,d0-4/a0
		rts

zeros:		ds.l		13