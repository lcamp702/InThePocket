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
;		plot_rectangle
;		plot_square
;		plot_triangle
;	  Visual Tests:
;		clear_screen
;		clear_region (TO DO: ADD CLIPPING)
;		plot_pixel
;		plot_horizontal_line
;		plot_vertical_line
;		plot_line (c)
;		plot_8bit_bitmap
;		plot_16bit_bitmap
;		plot_32bit_bitmap
;		plot_character (c)
;		plot_string (c)

;	Credits:

;	[1] D. Salvato, “Assembly tricks for fast bounds checking,” @dansalvato, https://dansalva.to/assembly-tricks-for-fast-bounds-checking/ (accessed Oct. 13, 2026). 
;	[2] StingRay, "Plotter / Dot Flag," English Amiga Board, https://eab.abime.net/showthread.php?t=50595 (Accessed Oct. 13 2026)

;	Purpose:
;	A raster engine for our game, In the Pocket on the Atari ST.

		xdef		_clear_screen
		xdef		_clear_region
		xdef		_plot_pixel
		xdef		_plot_horizontal_line
		xdef		_plot_vertical_line
		xdef		_plot_line
		xdef		_plot_8bit_bitmap
		xdef		_plot_16bit_bitmap
		xdef		_plot_32bit_bitmap

;       Screen details
ROWS		equ		400
COLS		equ		640
BYTE_COLS	equ		80

; ==== void clear_screen(UINT32 *base) ====
; PURPOSE: Clears the entire screen.
; INPUT: Address(UINT32*): to the start of the screen
; OUTPUT: None
cs_base		equ		64	; Offset from SP, not A6
_clear_screen:	; Quick clear code from Raster Lab on D2L
		movem.l		d0-7/a0-6,-(sp) ; Store existing register values

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

; ==== void clear_region(UINT32 *base, int row, int col, UINT16 length, UINT16 width) ====
; PURPOSE: Clear a region of the screen. The section is specified by the coordinates of the top left corner, and the height and width of the region.
; INPUT: Address(UINT32*): to the start of the screen
;	   Position(row,col): the coordinates of the top left pixel of the region
;	   Length: the length (number of rows) in pixels of the region
;	   Width: the width (number of columns) in pixels of the region
; OUTPUT: None
cr_base		equ		44
cr_row		equ		48
cr_col		equ		50
cr_len		equ		52
cr_wid		equ		54
_clear_region:
		movem.l		d0-6/a0-2,-(sp)

		; Copy params to registers and bound check
		move.w		cr_len(sp),d3	; d3.w = Length
		ble.w		cr_end		; if (length < 0) return

		move.w		cr_row(sp),d0	; d0.w = Row #
		cmp.w		#ROWS,d0
		bhs.w		cr_end		; Fast row bound check [1]

		move.w		cr_wid(sp),d2	; d2.w = Width
		ble.w		cr_end		; if (width < 0) return

		move.w		cr_col(sp),d1	; d1.w = Column #
		cmp.w		#COLS,d1
		bhs.w		cr_end		; Fast col bound check [1]

		movea.l		cr_base(sp),a0	; a0.l = framebuffer base
		movea.l		a0,a1		; a1.l = framebuffer base

		; Calculate end row offset
		move.w		d0,d4
		add.w		d3,d4		; d4.w = row + length

		; Clip length
		cmp.w		#ROWS,d4
		ble.b		cr_row_clipped	; If end row < ROWS continue
		move.w		#ROWS,d3	; End row is too large, clip it
		move.w		d3,d4		; d4.w = Clipped end row
		sub.w		d0,d3		; d3.w = Clipped length
cr_row_clipped:	; End row is on the screen
		subq.w		#1,d4		; d4.w = End row

		; Calculate row offset in bytes
		move.w		d4,d5
		lsl.w		#4,d4
		lsl.w		#6,d5
		add.w		d5,d4		; d4.w = End row byte offset

		move.w		d1,d0		; d0.w = Copy of start col for shift calc

		; Calculate end col
		add.w		d1,d2		; d2.w = Col + Width

		; Move end pointer
		lsr.w		#3,d1		; d1.w = Start long offset
		andi.b		#$FC,d1		; d1.w = Start byte offset (long aligned)
		add.w		d4,d1		
		adda.w		d1,a1		; End pointer is at the first pixel of the last row to clear

		; Clip width
		cmp.w		#COLS,d2	
		ble.w		cr_col_clipped	; If end col < COLS continue
		move.w		#COLS,d2
cr_col_clipped:
		subq.w		#1,d2		; d2.w = End col

		move.w		d2,d4		; d4.w = Copy of end col for shift calc

		move.w		d0,d1		; d1.w = Start col
		lsr.w		#5,d1		; d1.w = Start long offset (d1 shifted right 3 when moving the pointer)
		lsr.w		#5,d2		; d2.w = End long offset

		andi.w		#31,d0		; d0.w = Start col % 32
		beq.b		cr_start_zero	; If remainder = 0 then shift = 32. Start mask = all zeroes
		; Create shifted start mask
		neg.w		d0		; d0.w = -(start col % 32)
		addi.w		#32,d0		; d0.w = Start mask left shift val

		moveq.l		#-1,d5		; d5.l = (0xFFFFFFFF)
		lsl.l		d0,d5		; d5.l = Shifted start mask

		bra.b		cr_end_mask
cr_start_zero:	; Create zero start mask
		moveq.l		#0,d5		; d5.l = Start mask (0x00000000)
cr_end_mask:
		andi.w		#31,d4		; d4.w = End col % 32
		cmpi.w		#31,d4
		beq.b		cr_end_zero	; If remainder = 31 then shift = 32. End mask = all zeroes
		; Create shifted end mask
		addq.w		#1,d4		; d4.w = End mask right shift val

		moveq.l		#-1,d0		; d0.l = (0xFFFFFFFF)
		lsr.l		d4,d0		; d0.l = Shifted end mask

		bra.b		cr_blank
cr_end_zero:	; Create zero end mask
		moveq.l		#0,d0		; d0.l = End mask (0x00000000)
cr_blank:	; Start blanking the screen
		move.l		#BYTE_COLS,d6		; d1.l = #80 for quicker sub loop
						; 8(length) - 12 clocks speedup. (Slower for length = 1)

		subq.w		#1,d3

		cmp.w		d1,d2
		bne.b		cr_thick

		; Clearing mask fits in one long
		or.l		d5,d0		; d0.l = Full mask
cr_thin_loop:	; Apply mask over entire length
		and.l		d0,(a1)		; Apply mask to target long in memory
		suba.l		d6,a1		; Move back one row
		dbra.w		d3,cr_thin_loop
		bra		cr_end

cr_thick:	; Clearing spans multiple longs
		moveq.l		#0,d4		; d4.l = zeroes mask
		sub.w		d1,d2		; d2 = End offset - start offset
		subq.w		#2,d2		; d2 = Fills count

cr_thick_loop:
		move.w		d2,d1		; d1 = Row fill counter

		movea.l		a1,a2		; a2.l = Row cursor
		suba.l		d6,a1		; Move a1 back one row

		and.l		d5,(a2)+	; Apply start mask to long in memory

		tst.w		d2
		blt.b		cr_thick_end	; Skip fills if fill count < 0
cr_thick_fill:
		move.l		d4,(a2)+	; Clear long in memory
		dbra.w		d1,cr_thick_fill

cr_thick_end:
		and.l		d0,(a2)		; Apply end mask to long in memory
		dbra.w		d3,cr_thick_loop	

cr_end:
		movem.l		(sp)+,d0-6/a0-2
		rts

; ==== void plot_pixel(UINT8* base, int row, int col) ====
; PURPOSE: Plots a single pixel on the screen.
; INPUT: Address(UINT8*): to the start of the screen
;       Position(row,col): the location of the pixel to plot
; OUTPUT: None
px_base		equ		20
px_row		equ		24
px_col		equ		26
_plot_pixel:
		movem.l		d0-2/a0,-(sp)

		move.w		px_row(sp),d0	; d0.w = Row #
		cmp.w		#ROWS,d0
		bhs.b		pp_end		; Fast row bound check [1]

		move.w		px_col(sp),d1	; d1.w = Column #
		cmp.w		#COLS,d1
		bhs.b		pp_end		; Fast col bound check [1]

		; Multiplication broken into powers of two
		move.w		d0,d2		; d2.w = Row #
		lsl.w		#4,d0		; d0.w = row * 16
		lsl.w		#6,d2		; d2.w = row * 64
		add.w		d2,d0		; d0.w = Row byte offset = row * 80
		
		move.w		d1,d2		; d2.w = Column #
		not.w		d2		; d2 (lowest 3 bits) = Column bit shift (7 - (col % 8))
						; From the discussion in [2], a faster bitwise alternative.
						; Works because bset source is mod 8. (https://68k.hax.com/BSET)
		
		lsr.w		#3,d1		; d1.w = Col byte offset (col / 8)
		add.w		d1,d0		; d0.w = Total byte offset

		movea.l		px_base(sp),a0	; a0 points to framebuffer base
		bset.b		d2,0(a0,d0.w)	; Set specific bit in memory
pp_end:
		movem.l		(sp)+,d0-2/a0
		rts

; ==== void plot_horizontal_line(UINT32 *base, int row, int col, UINT16 length) ====
; PURPOSE: Plot a horizontal line on the screen. The horizontal line is specified by the leftmost pixel of the line and the length of the line.
; INPUT: Address(UINT32*): to the start of the screen
;	 Position(row,col): the coordinates of the leftmost pixel of the horizontal line
;	 Length: the length in pixels of the line
; OUTPUT: None
hl_base		equ		24
hl_row		equ		28
hl_col		equ		30
hl_len		equ		32
_plot_horizontal_line:
		movem.l		d0-3/a0,-(sp)

		; Copy params to registers and bound check
		move.w		hl_row(sp),d0	; d0.w = Row #
		cmp.w		#ROWS,d0
		bhs.b		hl_end		; Fast row bound check [1]

		move.w		hl_len(sp),d2	; d2.w = Length
		beq.b		hl_end		; if (length == 0) return

		move.w		hl_col(sp),d1	; d1.w = Column #
		bpl.b		hl_valid_col
		neg.w		d1		
		sub.w		d1,d2		; Subtract from length	
		bls.b		hl_end
		clr.w		d1
hl_valid_col:
		cmp.w		#COLS,d1
		bhs.b		hl_end		; Fast col bound check [1]

		; Length clipping
		add.w		d1,d2		; d2.w = End col + 1
		blo.b		hl_clip		; On carry (overflow), length must be clipped
		cmpi.w		#COLS,d2
		blo.b		hl_clipped
hl_clip:
		move.w		#COLS,d2	; Clip end col down to last column
hl_clipped:
		subq.w		#1,d2		; d2.w = Clipped end col

		; Move fb pointer to first long
		move.w		d0,d3
		lsl.w		#2,d3		; d3.w = Row * 4
		lsl.w		#4,d0		; d0.w = Row * 16
		add.w		d3,d0		; d0.w = Row long offset (row * 20)

		move.w		d1,d3
		lsr.w		#5,d3		; d3.w = Start col long offset
		add.w		d3,d0		; d0.w = Total start long offset
		lsl.w		#2,d0		; d0.w = Total start byte offset (long aligned)

		movea.l		hl_base(sp),a0	; a0.l = Framebuffer base
		adda.w		d0,a0		; a0.l = First long to write to
		
		; Calculate start shift and mask
		andi.w		#31,d1		; d1.w = Start mask right shift amount
		moveq.l		#-1,d0		; d0.l = 0xFFFFFFFF
		lsr.l		d1,d0		; d0.l = Shifted start mask

		; Calculate long delta between start and end
		move.w		d2,d1		; d1.w = Copy of end col for end mask
		lsr.w		#5,d2		; d2.w = End col long offset
		sub.w		d3,d2		; d2.w = end col offset - start col offset

		; Calculate end shift and mask
		not.w		d1
		andi.w		#31,d1		; d1.w = End mask left shift amount
		moveq.l		#-1,d3		; d3.l = 0xFFFFFFFF
		lsl.l		d1,d3		; d3.l = Shifted end long

		tst.w		d2		
		bne.b		hl_long

		; If end col offset == start col offset
		; Line fits within one long
		and.l		d3,d0		; d0.l = Full mask
		or.l		d0,(a0)		; Apply mask to framebuffer
		bra		hl_end
hl_long:
		; If end col offset != start col offset
		; Line spans multiple longs
		or.l		d0,(a0)+	; Apply start mask to framebuffer

		subq.w		#2,d2		; d2.w = Fill loop counter
		blt.b		hl_long_end

		moveq.l		#-1,d0		; Fill long with ones
hl_fill_loop:
		move.l		d0,(a0)+	; Write filled long to framebuffer
		dbra		d2,hl_fill_loop
hl_long_end:
		or.l		d3,(a0)		; Apply end mask to framebuffer

hl_end:
		movem.l		(sp)+,d0-3/a0
		rts

; ==== void plot_vertical_line(UINT32 *base, int row, int col, UINT16 length) ====
; PURPOSE: Plot a vertical line on the screen. The vertical line is specified by the topmost pixel of the line and the length of the line.
; INPUT: Address(UINT32*): to the start of the screen
;	 Position(row,col): the coordinates of the topmost pixel of the vertical line
;	 Length: the length in pixels of the line
; OUTPUT: None
vl_base		equ		28
vl_row		equ		32
vl_col		equ		34
vl_len		equ		36
_plot_vertical_line:
		movem.l		d0-4/a0,-(sp)

		move.w		vl_len(sp),d3	; d3.w = length
		beq.b		vl_end		; Zero length check

		move.w		vl_row(sp),d0	; d0.w = Row #
		bpl.b		vl_valid_row

		neg.w		d0		
		sub.w		d0,d3		; Subtract from length	
		bls.b		vl_end
		clr.w		d0
vl_valid_row:
		cmp.w		#ROWS,d0
		bhs.b		vl_end		; Fast row bound check [1]

		move.w		vl_col(sp),d1	; d1.w = Column #
		cmp.w		#COLS,d1	
		bhs.b		vl_end		; Fast col bound check [1]
				
		; Row clipping check
		move.w		d0,d2
		add.w		d3,d2		; d2.w = end row
		blo.b		vl_clip		; If carry set, value wraps around. Must be clipped.
		
		cmp.w		#ROWS,d2
		bls.b		vl_clipped	; If end row < rows, line fits, continue
vl_clip:
		move.w		#ROWS,d3	
		sub.w		d0,d3		; If end row >= rows, clip line length

vl_clipped:	; Line fits on screen
		move.w		d1,d2		; d2.w = Column #
		not.w		d2		; d2 (lowest 3 bits) = Column bit set index

		move.w		d0,d4		; d4.w = Row #
		lsl.w		#4,d0		; d0.w = row * 16
		lsl.w		#6,d4		; d4.w = row * 64
		add.w		d4,d0		; d0.w = row * BYTE_COLS (Row offset)

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

; ==== long clip_pos(int row, int col, int dr, int dc, int code) ====
; PURPOSE: Cohen-Sutherland line clipping for one set of coordinates and total line deltas
; INPUT: Position(int row, int col): A row and column coordinate set that is out of bounds
;	 Deltas(int dr, int dc): The total row and column deltas for the line being clipped
;	 TBRL Code(int code): The Cohen-Sutherland clipping code associated with the coordinates to crop
; OUTPUT: d0 = row:col (bits 31-16:15-0)

CLIP_TOP	equ	8	; 1000
CLIP_BOTTOM	equ	4	; 0100
CLIP_RIGHT	equ	2	; 0010
CLIP_LEFT	equ	1	; 0001

CLIP_R		equ	12	; 1100

clip_row	equ	28
clip_col	equ	30	
clip_code	equ	32
clip_dr		equ	34
clip_dc		equ	36
clip_pos:
		movem.l		d1-6,-(sp)

		move.w		clip_row(sp),d3	; d3.w = Row
		move.w		clip_col(sp),d4	; d4.w = Col

		move.w		clip_code(sp),d0; d0 (bottom nibble) = Cohen-Sutherland clipping code (TBRL)
		beq.b		clip_accept	; No clip code, nothing to clip

		; Clipping is required
		move.w		clip_dr(sp),d5	; d5.w = Dr
		move.w		clip_dc(sp),d6	; d6.w = Dc

		moveq.l		#CLIP_R,d1	; Get row clipping mask to check against
		and.b		d0,d1		; Test clip code against row clip mask
		beq.b		clip_rows_done	; Row not clipping, check col

		; Clipping row		
		moveq.l		#0,d2		; Clear temp register for calculating clip
		moveq.l		#CLIP_BOTTOM,d1	; Clipping mask for bottom edge clipping
		and.b		d0,d1		; Check for bottom edge clipping
		beq.b		clip_off_top	; No bottom edge clipping, skip extra code
		; Extra code for bottom edge clipping
		move.w		#ROWS-1,d2
clip_off_top:	; Shared code for top / bottom edge clipping
		sub.w		d3,d2		; d2 (temp) = Row clip amount 
						; (positive for top clip, negative for bottom)
		add.w		d2,d3		; Clip row

		muls.w		d6,d2		; SLOW!! No other way to do this as far as I know -Peter
		bvs.b		clip_reject	; Reject on overflow
		divs.w		d5,d2		; d2 (temp) = Col clip amount
		bvs.b		clip_reject	; Reject on overflow
		add.w		d2,d4		; Clip col

clip_rows_done:	; Recalculate column clip codes
		moveq.l		#0,d0		; Clear clip code
		tst.w		d4
		bpl.b		clip_left_safe	; Col > 0, no left clip
		or.b		#CLIP_LEFT,d0 	; Set left clip code
		bra.b		clip_right_safe	; Left and right can't clip at once
clip_left_safe:	; No left clip
		cmpi.w		#COLS,d4
		blt		clip_right_safe	; Col < COLS, no right clip
		or.b		#CLIP_RIGHT,d0	; Set right clip code
clip_right_safe: ; No right clip
		tst.b		d0
		beq.b		clip_accept	; Clip code = 0, No more clipping.
		
		; Clip col	
		moveq.l		#0,d2		; Clear temp register for calculating clip
		moveq.l		#CLIP_RIGHT,d1	; Clipping mask for right edge clipping
		and.b		d0,d1		; Check for right edge clipping
		beq.b		clip_off_left	; No right edge clipping, skip extra code
		; Extra code for right edge clipping
		move.w		#COLS-1,d2
clip_off_left:	; Shared code for right / left edge clipping
		sub.w		d4,d2		; d2 (temp) = Col axis clip amount 
						; (positive for left clip, right for trailing)
		add.w		d2,d4		; Clip col

		muls.w		d5,d2		; SLOW!! No other way to do this as far as I know -Peter
		bvs.b		clip_reject	; Reject on overflow
		divs.w		d6,d2		; d2 (temp) = Row clip amount
		bvs.b		clip_reject	; Reject on overflow
		add.w		d2,d3		; Clip row

		; If row was pushed out of bounds, line does not appear on screen.
		tst.w           d3
                bmi.b           clip_reject     ; Row < 0 -> line missed the screen!
                cmpi.w          #ROWS,d3
                bge.b           clip_reject     ; Row >= ROWS -> line missed the screen!

clip_accept:	; Pack valid position into d0 and return
		move.w		d3,d0
		swap		d0
		move.w		d4,d0
		movem.l 	(sp)+,d1-6
		rts

clip_reject:	; Pack -1,-1 position into d0 and return
		moveq.l		#-1,d0		; High and low word = -1 (0xFFFF)
		movem.l 	(sp)+,d1-6
		rts

; ==== void plot_line(UINT32 *base, int start_row, int start_col, int end_row, int end_col) ====
; PURPOSE: Plots a line on the screen between the two given points.
; INPUT: Address(UINT32*): to the start of the screen
;	 Position(start_row,start_col): the coordinates of the start of the line
;	 Position(end_row,end_col): the coordinates of the end of the line
; OUTPUT: None

pl_base		equ	40
pl_srow		equ	44
pl_scol		equ	46
pl_erow		equ	48
pl_ecol		equ	50
_plot_line:
		movem.l		d0-7/a0,-(sp)

		; Check start row and col clipping
		clr.w		d0
		move.w		pl_srow(sp),d1	; Get start row
		bpl.b		pl_start_top_safe
		or.b		#CLIP_TOP,d0	; Set top clip code
		bra.b		pl_start_bottom_safe	; Top and bottom can't clip at once
pl_start_top_safe:
		cmpi.w		#ROWS,d1
		blt.w		pl_start_bottom_safe
		or.b		#CLIP_BOTTOM,d0	; Set bottom clip code
pl_start_bottom_safe:	; d1.w = start_row, d0.b = start clip code
		move.w		pl_scol(sp),d2	; Get start col
		bpl.b		pl_start_left_safe
		or.b		#CLIP_LEFT,d0 ; Set left clip code
		bra.b		pl_start_right_safe	; Left and right can't clip at once
pl_start_left_safe:
		cmpi.w		#COLS,d2
		blt		pl_start_right_safe
		or.b		#CLIP_RIGHT,d0	; Set right clip code
pl_start_right_safe:	; d2.w = start_col, d0.b = start clip code

		; Check end row and col clipping
		clr.w		d3
		move.w		pl_erow(sp),d4	; Get end row
		bpl.b		pl_end_top_safe
		or.b		#CLIP_TOP,d3	; Set top clip code
		bra.b		pl_end_bottom_safe	; Top and bottom can't clip at once
pl_end_top_safe:
		cmpi.w		#ROWS,d4
		blt.w		pl_end_bottom_safe
		or.b		#CLIP_BOTTOM,d3	; Set bottom clip code
pl_end_bottom_safe:	; d4.w = end_row, d3.b = end clip code
		move.w		pl_ecol(sp),d5	; Get end col
		bpl.b		pl_end_left_safe
		or.b		#CLIP_LEFT,d3 	; Set left clip code
		bra.b		pl_end_right_safe	; Left and right can't clip at once
pl_end_left_safe:
		cmpi.w		#COLS,d5
		blt		pl_end_right_safe
		or.b		#CLIP_RIGHT,d3	; Set right clip code
pl_end_right_safe:	; d5.w = end_col, d3.b = end clip code

		; Calculate deltas
		move.w		d4,d6
		sub.w		d1,d6		; d6.w = Row delta
		move.w		d5,d7
		sub.w		d2,d7		; d7.w = Column delta

		movea.w		d0,a0		; Save a copy of the start clip code in a0
		and.b		d3,d0		; d0.w = start code & end code
		bne.w		pl_end		; Line does not cross screen

		move.w		a0,d0
		or.b		d3,d0
		beq		pl_both_safe	; Skip checking individual codes if neither code indicates clipping
		move.w		a0,d0

		; Push dc and dr to stack for clip_pos call
		move.w		d7,-(sp)	; Push dc to stack
		move.w		d6,-(sp)	; Push dr to stack

		tst.b		d0
		beq		pl_start_safe	; Skip clipping start position

		; Call clipping subroutine (push code, then col, then row)
		move.w		d0,-(sp)	; Push clipping code to stack
		move.w		d2,-(sp)	; Push start col to stack
		move.w		d1,-(sp)	; Push start row to stack
		jsr		clip_pos
		addq.w		#6,sp		; Super pop row, col, and code
		tst.l		d0
		bmi.b		pl_clip_abort	; If d0 < 0 clip_pos rejected. No line on screen
		; Get row and col from d0
		move.w		d0,d2		; Unpack col
		swap		d0		; Bring row to lower word
		move.w		d0,d1		; Unpack row

pl_start_safe:
		tst.b		d3
		beq		pl_end_safe	; Skip clipping end position

		move.w		d3,-(sp)	; Push clipping code to stack
		move.w		d5,-(sp)	; Push start col to stack
		move.w		d4,-(sp)	; Push start row to stack
		jsr		clip_pos
		addq.w		#6,sp		; Super pop row, col, and code
		tst.l		d0
		bmi.b		pl_clip_abort	; If d0 < 0 clip_pos rejected. No line on screen
		; Get row and col from d0
		move.w		d0,d5		; Unpack col
		swap		d0		; Bring row to lower word
		move.w		d0,d4		; Unpack row

pl_end_safe:	; Both start and end positions are in bounds, one was updated. Fix deltas.
		addq.w		#4,sp		; Super pop deltas
		move.w		d4,d6
		sub.w		d1,d6		; d6.w = Row delta
		move.w		d5,d7
		sub.w		d2,d7		; d7.w = Column delta
		bra.b		pl_both_safe	; Continue to draw line

pl_clip_abort:	; clip_pos rejected coordinate, end early
		addq.w		#4,sp		; Super pop deltas
		bra.w		pl_end		; Exit

pl_both_safe:	; Both given start and end positions in bounds already
		tst.w		d7
		bpl.b		pl_dc_positive	; If dc is already positive move on
		; If dc is negative flip dc, dr, and start at the end coords
		neg.w		d6		; Flip sign on dr
		neg.w		d7		; Flip sign on dc
		move.w		d4,d1		; d4.w (Start row) = end row
		move.w		d5,d2		; d5.w (Start col) = end col
pl_dc_positive:	; dc is positive
		; Calculate byte offset for pointer
		ext.l		d1
		move.w		d1,d4
		lsl.w		#4,d4
		lsl.w		#6,d1
		add.w		d4,d1		; d1.w = start row * row byte offset
		
		move.w		d2,d4		; d4.w = Extra copy of start col
		lsr.w		#5,d4		; d4.w = Col long offset
		lsl.w		#2,d4		; d4.w = Col long aligned byte offset
		add.w		d4,d1		; d1.w = total byte offset

		movea.l		pl_base(sp),a0	; Get framebuffer base
		adda.w		d1,a0		; Move framebuffer pointer

		move.w		#BYTE_COLS,d1	; d1.w = Row step
		tst.w		d6
		bpl.b		pl_dr_positive	; If dr is already positive move on
		; If dr is negative flip dr and the row step direction
		neg.w		d6		; Flip sign on dr
		neg.w		d1		; Flip sign on row step direction
pl_dr_positive:	; dr is positive
		; Create mask
		not.w		d2
		andi.w		#31,d2		; d2.w = BSET pixel number

		moveq.l		#0,d0		; d0.l = Long mask
		bset.l		d2,d0		; Set first pixel on mask
		or.l		d0,(a0)		; Apply first mask

		cmp.w		d7,d6
		beq.b		pl_diagonal
		bhi.b		pl_row_maj
		bra.b		pl_col_maj		

pl_diagonal:	; Diagonal. Increment both row and col each step
		subq.w		#1,d7		; Predecrement loop counter
		bmi.b		pl_end		; Nothing to draw
pl_diag_loop:	
		adda.w		d1,a0		; Move fb pointer to next row
		ror.l		#1,d0		; Move bit to the right
		bhs.b		pl_diag_nowrap
		addq.l		#4,a0		; Carry flag set, move fb pointer to next long
pl_diag_nowrap:	; No carry flag, bit still within long
		or.l		d0,(a0)		; Apply mask to framebuffer
		dbra		d7,pl_diag_loop
		bra.b		pl_end		; Done drawing

pl_row_maj:	; Row major. Increment row every step
		move.w		d6,d4
		subq.w		#1,d4		; Predecrement loop counter
		bmi.b		pl_end		; Nothing to draw

		clr.w		d3
		sub.w		d6,d3		; d3.w = err (-dr)

		add.w		d6,d6		; d6.w = 2 * dr
		add.w		d7,d7		; d7.w = 2 * dc
pl_rmaj_loop:
		adda.w		d1,a0		; Move to next row
		add.w		d7,d3		; Update error
		bmi.b		pl_rmaj_nowrap	; If err < 0 skip col step
		; Error >= 0. Col step
		sub.w		d6,d3		; Update error
		ror.l		#1,d0		; Move bit to the right
		bhs.b		pl_rmaj_nowrap	; No overflow, don't move to next long
		addq.l		#4,a0		; Move fb pointer to next long
pl_rmaj_nowrap:	; No carry flag, bit still within long
		or.l		d0,(a0)
		dbra		d4,pl_rmaj_loop
		bra.b		pl_end		; Done drawing


pl_col_maj:	; Col major. Increment col every step
		; Optimize through multi bit mask in register
		move.w		d7,d4
		subq.w		#1,d4		; Predecrement loop counter
		bmi.b		pl_end		; Nothing to draw

		clr.w		d3
		sub.w		d7,d3		; d3.w = err (-dc)

		add.w		d6,d6		; d6.w = 2 * dr
		add.w		d7,d7		; d7.w = 2 * dc
pl_cmaj_loop:
		subq.w		#1,d2
		bhs.b		pl_cmaj_nowrap ; No carry, no wrap.
		; Wrap to next long. Apply and clear mask, reset pixel number
		or.l		d0,(a0)	; Apply mask to framebuffer
		moveq.l		#0,d0	; Clear mask
		moveq.l		#31,d2	; Reset pixel number to the end
		addq.l		#4,a0	; Move fb pointer to next long
pl_cmaj_nowrap:
		add.w		d6,d3	; Adjust error
		bmi.b		pl_cmaj_plot	; if err < 0 skip row step
		; Error >= 0. Row step. Apply and clear mask. 
		or.l		d0,(a0) ; Apply mask to framebuffer
		moveq.l		#0,d0	; Clear mask
		sub.w		d7,d3	; Adjust error
		adda.w		d1,a0	; Move fb pointer to next row
pl_cmaj_plot:
		bset.l		d2,d0	; Set pixel in the mask
		dbra		d4,pl_cmaj_loop
		or.l		d0,(a0) ; Apply last mask to framebuffer
pl_end:
		movem.l		(sp)+,d0-7/a0
		rts

; ==== Function: plot_8bit_bitmap ====
; PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.
; INPUT: Address(UINT8*): to the start of the screen
;	 Position(row,col): the coordinates of the top left pixel of the bitmap
;	 Bitmap(UINT8*): pointer to the bitmap to print
;	 Height: the length (number of rows) of the height in pixels of the bitmap
; OUTPUT: None
bm8_base	equ		24
bm8_row		equ		28
bm8_col		equ		30
bm8_bmp		equ		32
bm8_hei		equ		36
_plot_8bit_bitmap:
		movem.l		d0-2/a0-1,-(sp)

		; Height must be > 0
		move.w		bm8_hei(sp),d0	; d0.w = height
		beq.w		bm8_end		; if (height == 0) return

		; Load bitmap pointer
		movea.l		bm8_bmp(sp),a1	; a1.l = Bitmap base pointer

		; Load row
		move.w		bm8_row(sp),d1	; d1.w = Row #
		bpl.b		bm8_bottom	; Row > 0, no top clipping

		; Top clipping
		neg.w		d1
		sub.w		d1,d0		; Subtract from print height
		bls.w		bm8_end		; If height < negative row count, nothing to draw

		; Top clipped but partially visible
		adda.w		d1,a1		; Offset bitmap pointer by # of negative rows
		clr.w		d1		; Zero start row
		
bm8_bottom:
		cmpi.w		#ROWS,d1
		bhs.w		bm8_end		; If row >= max row, nothing to draw

		; Bottom clipping
		move.w		#ROWS,d2
		sub.w		d1,d2		; d2.w = Max visible height
		cmp.w		d2,d0
		blo.b		bm8_clipped_row

		; Bottom clipped but bitmap partially appears.
		move.w		d2,d0		; Clamp height to visible height

bm8_clipped_row:	; Clipped row + height found
		subq.w		#1,d0		; Predecrement height for print loops

		; Move framebuffer pointer to first byte of the row
		movea.l		bm8_base(sp),a0	; a0.l = Framebuffer base pointer

		move.w		d1,d2
		lsl.w		#4,d1
		lsl.w		#6,d2
		add.w		d2,d1		; d1.w = Row byte offset

		adda.w		d1,a0		; a0 points to start of row to draw on

		; Load column
		move.w		bm8_col(sp),d1	; d2.w = Column #
		bpl.b		bm8_right

		; Left clipping
		neg.w		d1		; d1.w = negative col count
		cmpi		#8,d1
		bhs.b		bm8_end		; If negative col count >= 8, nothing to draw

bm8_left_loop:	; No column offset, draw shifted bitmap on the left
		move.b		(a1)+,d2	; Load next bitmap row into register
		lsl.b		d1,d2		; Shift bitmap row
		or.b		d2,(a0)		; Apply mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next row of framebuffer
		dbra		d0,bm8_left_loop
		bra.b		bm8_end		; Done drawing

bm8_right:	; Col > 0, no left clip
		cmpi.w		#COLS,d1
		bhs.b		bm8_end		; If col >= max col, nothing to draw

		move.w		d1,d2		; d2.w = Copy of col # for offset calc
		lsr.w		#3,d1		; d2.w = Col byte offset
		adda.w		d1,a0		; a0.l = Pointer to first byte to write to

		; Get bit shift
		andi.w		#7,d2		; d2.w = Start shift right amount (col % 8)
		bne		bm8_shifted	
		
bm8_aligned_loop:	; If start shift == 0, print bmp into bytes
		move.b		(a1)+,d1	; d1.b = Bitmap row
		or.b		d1,(a0)		; Apply bitmap row mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next line of the framebuffer
		dbra		d0,bm8_aligned_loop

		bra		bm8_end

bm8_shifted:	; If start shift != 0, shift bmp across 2 bytes
		cmpi.w		#BYTE_COLS-1,d1
		bhs.b		bm8_right_loop

bm8_shift_loop:
		clr.w		d1		; Clear word to shift byte within
		move.b		(a1)+,d1	; d1.b = Bitmap row
		ror.w		d2,d1		; d1.b = Shifted start byte
		or.b		d1,(a0)+	; Apply bitmap row masks to framebuffer
		ror.w		#8,d1		; d1.b = Shifted end byte
		or.b		d1,(a0)

		adda.w		#BYTE_COLS-1,a0	; Move to next row

		dbra		d0,bm8_shift_loop
		bra.b		bm8_end

bm8_right_loop:
		move.b		(a1)+,d1
		lsr.b		d2,d1
		or.b		d1,(a0)
		adda.w		#BYTE_COLS,a0
		dbra		d0,bm8_right_loop

bm8_end:
		movem.l		(sp)+,d0-2/a0-1
		rts

; ==== Function: plot_16bit_bitmap ====
; PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.
; INPUT: Address(UINT16*): to the start of the screen
;	 Position(row,col): the coordinates of the top left pixel of the bitmap
;	 Bitmap(UINT16*): pointer to the bitmap to print
;	 Height: the length (number of rows) of the height in pixels of the bitmap
; OUTPUT: None
bm16_base	equ		24
bm16_row	equ		28
bm16_col	equ		30
bm16_bmp	equ		32
bm16_hei	equ		36
_plot_16bit_bitmap:
		movem.l		d0-2/a0-1,-(sp)

		; Height check
		move.w		bm16_hei(sp),d0	; d0.w = height
		beq.w		bm16_end	; if (length == 0), nothing to draw

		; Load bitmap pointer
		movea.l		bm16_bmp(sp),a1	; a1.l = Bitmap base pointer

		; Load row
		move.w		bm16_row(sp),d1	; d1.w = Row #
		bpl.b		bm16_bottom	; Row > 0, no top clipping

		; Top clipping
		neg		d1
		sub.w		d1,d0		; Subtract negative row count from height
		bls.w		bm16_end	; If height < negative row count, nothing to draw

		; Top clipped but bitmap partially appears
		add.w		d1,d1		; d1.w = Bitmap row offset (row * 2)
		adda.w		d1,a1		; Offset bitmap pointer by # of negative rows
		clr.w		d1		; Start row = 0

bm16_bottom:
		cmpi.w		#ROWS,d1
		bhs.w		bm16_end	; If row >= max row, nothing to draw

		; Bottom clip
		move.w		#ROWS,d2
		sub.w		d1,d2		; d2.w = Max visible height
		cmp.w		d2,d0
		blo.b		bm16_clipped_row	; If height < visible height, no clipping

		; Bottom clipped but bitmap partially appears
		move.w		d2,d0		; Clamp height to visible height

bm16_clipped_row:	; Clipped row + height found
		subq.w		#1,d0		; Predecrement height for print loops

		; Move framebuffer pointer to first byte of the row
		movea.l		bm16_base(sp),a0	; a0.l = Framebuffer base pointer

		move.w		d1,d2
		lsl.w		#4,d1
		lsl.w		#6,d2
		add.w		d2,d1		; d1.w = Row byte offset

		adda.w		d1,a0		; a0 points to the first row to draw on

		; Load column
		move.w		bm16_col(sp),d1	; d1.w = Column #
		bpl.b		bm16_right	; If col > 0, no left clip

		; Left clipping
		neg.w		d1		; d1.w = negative col count
		cmpi.w		#16,d1
		bhs.b		bm16_end	; If negative col count >= 32, nothing to draw

bm16_left_loop:	; No column offset needed, draw shifted bitmap on the left
		move.w		(a1)+,d2	; Load next bitmap row into register
		lsl.w		d1,d2		; Shift bitmap row
		or.w		d2,(a0)		; Apply mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next row of framebuffer
		dbra		d0,bm16_left_loop
		bra.b		bm16_end	; Done drawing

bm16_right:	; Col > 0, no left clip
		cmpi.w		#COLS,d1
		bhs.b		bm16_end	; If col >= max col, nothing to draw

		move.w		d1,d2
		lsr.w		#4,d1		; d1.w = Col word offset
		add.w		d1,d1		; d1.w = Col word aligned byte offset
		adda.w		d1,a0		; a0.l = Pointer to first byte to write to

		; Get bit shift
		andi.w		#15,d2		; d2.w = Start shift right amount (col % 16)
		bne		bm16_shifted
		
bm16_aligned_loop:	; If start shift == 0, print bmp into words
		move.w		(a1)+,d1	; d1.w = Bitmap row
		or.w		d1,(a0)		; Apply bitmap row mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next line of the framebuffer
		dbra		d0,bm16_aligned_loop

		bra		bm16_end

bm16_shifted:
		cmpi.w		#BYTE_COLS-2,d1
		bhs.b		bm16_right_loop

bm16_shift_loop:
		; Speed optimized shift with swap.w and longs
		moveq.l		#0,d1		; Clear long to shift word in
		move.w		(a1)+,d1	; d1.w = Bitmap row
		ror.l		d2,d1		; d1.l = Shifted bitmap
		or.w		d1,(a0)+	; Apply start word bitmap row to framebuffer
		swap.w		d1		; d1.w = End word bitmap row
		or.w		d1,(a0)		; Apply end word bitmap row

		adda.w		#BYTE_COLS-2,a0		; Move to next row

		dbra		d0,bm16_shift_loop
		bra.b		bm16_end

bm16_right_loop:
		move.w		(a1)+,d1
		lsr.w		d2,d1
		or.w		d1,(a0)
		adda.w		#BYTE_COLS,a0
		dbra		d0,bm16_right_loop

bm16_end:
		movem.l		(sp)+,d0-2/a0-1
		rts

; ==== Function: plot_32bit_bitmap ====
; PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.
; INPUT: Address(UINT32*): to the start of the screen
;	 Position(row,col): the coordinates of the top left pixel of the bitmap
;	 Bitmap(UINT32*): pointer to the bitmap to print
;	 Height: the length (number of rows) of the height in pixels of the bitmap
; OUTPUT: None
bm32_base	equ		32
bm32_row	equ		36
bm32_col	equ		38
bm32_bmp	equ		40
bm32_hei	equ		44
_plot_32bit_bitmap:
		movem.l		d0-4/a0-1,-(sp)

		; Height must be > 0
		move.w		bm32_hei(sp),d0	; d0.w = height
		beq.w		bm32_end	; if (height == 0) return

		; Load bitmap pointer
		movea.l		bm32_bmp(sp),a1	; a1.l = Bitmap base pointer

		; Load row
		move.w		bm32_row(sp),d1	; d1.w = Row #
		bpl.b		bm32_bottom	; Row > 0, no top clipping.

		; Top clipping
		neg.w		d1
		sub.w		d1,d0		; Subtract negative row count from height
		bls.w		bm32_end	; If height < negative row count, nothing to draw

		; Top clipped but bitmap partially appears
		ext.l		d1
		lsl.l		#2,d1		; Row * 4 = Bitmap row offset
		adda.l		d1,a1		; Offset bitmap pointer by # of negative rows
		clr.w		d1		; Zero start row

bm32_bottom:
		cmpi.w		#ROWS,d1
		bhs.w		bm32_end	; If row >= max row, nothing to draw

		; Bottom clip
		move.w		#ROWS,d2
		sub.w		d1,d2		; d2.w = Max visible height (ROWS - row)
		cmp.w		d2,d0
		blo.b		bm32_clipped_row	; If height < visible height, no clipping

		; Bottom clipped but bitmap partially appears
		move.w		d2,d0		; Clamp height to visible height

bm32_clipped_row:	; Clipped row + height found
		subq.w		#1,d0		; Predecrement height for print loop

		; Move fb pointer to first byte of row
		movea.l		bm32_base(sp),a0	; a0.l = Framebuffer base pointer
		move.w		d1,d2
		lsl.w		#4,d1
		lsl.w		#6,d2
		add.w		d2,d1		; d1.w = Row byte offset
		adda.w		d1,a0		; a0 points to the first row to draw on

		; Load column
		move.w		bm32_col(sp),d1	; d1.w = Column #
		bpl.b		bm32_right	; If col > 0, no left clip

		; Left clipping
		neg.w		d1		; d1.w = negative col count
		cmpi.w		#32,d1
		bhs.b		bm32_end	; If negative col count >= 32, nothing to draw

bm32_left_loop:	; No column offsets needed, so we can draw on the left side
		move.l		(a1)+,d2	; Load bitmap row into register
		lsl.l		d1,d2		; Shift bitmap row
		or.l		d2,(a0)		; Apply mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next row of framebuffer
		dbra		d0,bm32_left_loop
		bra.b		bm32_end	; Done drawing

bm32_right:	; Col > 0, no left clip
		cmpi.w		#COLS,d1
		bhs.b		bm32_end	; If col >= max col, nothing to draw

		move.w		d1,d2
		lsr.w		#5,d1		; d1.w = Col long offset
		lsl.w		#2,d1		; d1.w = Col long aligned byte offset
		adda.w		d1,a0		; a0.l = Pointer to first byte to write to

		; Get bit shift
		andi.w		#31,d2		; d2.w = Start shift right amount (col % 32)
		bne		bm32_shifted	
		
bm32_aligned_loop:	; If start shift == 0, print bmp into bytes
		move.l		(a1)+,d3	; d3.l = Bitmap row
		or.l		d3,(a0)		; Apply bitmap row mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next line of the framebuffer
		dbra		d0,bm32_aligned_loop

		bra		bm32_end

bm32_shifted:	; If start shift != 0, shift bmp across 2 bytes
		moveq.l		#32,d1		; d1.w = 32
		sub.w		d2,d1		; d1.w = 32-(col % 32)

		cmpi.w		#BYTE_COLS-4,d1
		bhs.b		bm32_right_loop	; Print only one long at the end of the screen

bm32_shift_loop:
		move.l		(a1)+,d3	; d3.l = Bitmap row
		move.l		d3,d4		; d4.l = Copy of bitmap row
		lsr.l		d2,d3		; d3.l = Shifted start byte
		lsl.l		d1,d4		; d4.l = Shifted end byte

		or.l		d3,(a0)+	; Apply bitmap row masks to framebuffer
		or.l		d4,(a0)

		adda.w		#BYTE_COLS-4,a0		; Move to next row

		dbra		d0,bm32_shift_loop
		bra.b		bm32_end

bm32_right_loop:
		move.l		(a1)+,d3	; d3.l = Bitmap row
		lsr.l		d2,d3		; Shifter start long
		or.l		d3,(a0)		; Write the left longword
		adda.w		#BYTE_COLS,a0	; Move to next row
		dbra		d0,bm32_right_loop

bm32_end:
		movem.l		(sp)+,d0-4/a0-1
		rts

zeros:		ds.l		13