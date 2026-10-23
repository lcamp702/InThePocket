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
;
;	  Visual Tests:
;		clear_screen
;		clear_region
;		plot_pixel
;		plot_horizontal_line
;		plot_vertical_line
;		plot_line
;		plot_rectangle
;		plot_square
;		plot_triangle
;		plot_8bit_bitmap
;		plot_16bit_bitmap
;		plot_32bit_bitmap
;		plot_character
;		plot_string

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
		xdef		_plot_rectangle
		xdef		_plot_square
		xdef		_plot_triangle
		xdef		_plot_8bit_bitmap
		xdef		_plot_16bit_bitmap
		xdef		_plot_32bit_bitmap
		xdef		_plot_character
		xdef		_plot_string

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
		beq.w		cr_end		; if (length == 0) return

		move.w		cr_row(sp),d0	; d0.w = Row #
		bpl.b		cr_valid_row
		neg.w		d0		
		sub.w		d0,d3		; Subtract from length	
		bls.w		cr_end		; Not on screen, exit
		clr.w		d0		; Reset row to 0
		cmp.w		#ROWS,d0
		bge.w		cr_end		; Row upper bound check
cr_valid_row:
		move.w		cr_wid(sp),d2	; d2.w = Width
		ble.w		cr_end		; if (width < 0) return

		move.w		cr_col(sp),d1	; d1.w = Column #
		bpl.b		cr_valid_col
		neg.w		d1		
		sub.w		d1,d2		; Subtract from width	
		bls.w		cr_end		; Not on screen, exit
		clr.w		d1		; Reset start col to 0
		cmp.w		#COLS,d1	
		bge.w		cr_end		; Col upper bound check
cr_valid_col:
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

; ==== void plot_rectangle(UINT32 *base, int row, int col, UINT16 length, UINT16 width) ====
; PURPOSE: Plots a rectangle on the screen given by the top left pixel, and the length and width of the rectangle.
; INPUT: Address(UINT32*): to the start of the screen
;		Position(row,col): the coordinates of the top left pixel of the rectangle
;		Length: the length (number of rows) in pixels of the rectangle
;		Width: the width (number of columns) in pixels of the rectangle
; OUTPUT: None
pr_base		equ	8
pr_row		equ	12
pr_col		equ	14
pr_len		equ	16
pr_wid		equ	18
_plot_rectangle:
		link		a6,#0
		movem.l		d0-1,-(sp)

		move.w		pr_len(a6),d0	; d0.w = Length (# of rows tall)
		move.w		pr_wid(a6),d1	; d1.w = Width (# of cols wide)

		; Load parameters for line drawing
		sub.w		#10,sp		; Advance sp to load up parameters backwards
		move.l		pr_base(a6),(sp)	; 1: base	0(sp)
		move.w		pr_row(a6),4(sp)	; 2: row	4(sp)
		move.w		pr_col(a6),6(sp)	; 3: col	6(sp)
		move.w		d0,8(sp)		; 4: length	8(sp)

		; Plot vertical in first corner
		jsr		_plot_vertical_line

		; Plot horizontal in first corner
		move.w		d1,8(sp)	; Move width to stack for horizontal line
		jsr		_plot_horizontal_line

		; Plot bottom horizontal line
		subq.w		#1,d0		; Decrement for end line position
		add.w		d0,4(sp)	; row = row + length - 1
		jsr		_plot_horizontal_line
		sub.w		d0,4(sp)	; Reset row

		; Plot right vertical line
		move.w		d0,8(sp)	; Move length back to stack for vertical line
		add.w		d1,6(sp)
		subq.w		#1,6(sp)	; col = col + width - 1
		jsr		_plot_vertical_line

		adda.w		#10,sp		; Super pop 10 bytes

		movem.l		(sp)+,d0-1
		unlk		a6
		rts

; ==== void plot_square(UINT32 *base, int row, int col, UINT16 side) ====
; PURPOSE: Plots a square on the screen given by the top left pixel, and the length of the sides of the square.
; INPUT: Address(UINT32*): to the start of the screen
;		Position(row,col): the coordinates of the top left pixel of the square
;		Side: the length of each side, in pixels, of the square
; OUTPUT: None
ps_base		equ	8
ps_row		equ	12
ps_col		equ	14
ps_side		equ	16
_plot_square:
		link		a6,#0
		move.l		d0,-(sp)

		move.w		ps_side(a6),d0	; d0.w = Side length

		; Load parameters for line drawing
		sub.w		#10,sp		; Advance sp to load up parameters backwards
		move.l		ps_base(a6),(sp)	; 1: base	0(sp)
		move.w		ps_row(a6),4(sp)	; 2: row	4(sp)
		move.w		ps_col(a6),6(sp)	; 3: col	6(sp)
		move.w		d0,8(sp)		; 4: length	8(sp)

		subq.w		#1,d0	; Decrement length for end line position

		; Plot horizontal and vertical lines in first corner
		jsr		_plot_vertical_line
		jsr		_plot_horizontal_line

		; Plot bottom horizontal line
		add.w		d0,4(sp)	; Row += side
		jsr		_plot_horizontal_line
		sub.w		d0,4(sp)	; Reset row

		; Plot right vertical line
		add.w		d0,6(sp)	; Col += side 
		jsr		_plot_vertical_line

		adda.w		#10,sp		; Super pop 10 bytes

		move.l		(sp)+,d0
		unlk		a6
		rts

; ==== void plot_triangle(UINT32 *base, int row, int col, UINT16 triangle_base, UINT16 height, UINT8 direction) ====
; PURPOSE: Plots a triangle on the screen given by the coordinate of the 90° angle, the length of the base, the length of the height, and the direction of the triangle.
; INPUT: Address(UINT32*): to the start of the screen
;		Position(row,col): the coordinates of the pixel of the 90° angle of the triangle
;		Base: the length (number of columns) of the base in pixels of the triangle (must be positive)
;		Height: the length (number of rows) of the height in pixels of the triangle (must be positive)
;		Direction: Describes where the coordinate is relative to the rest of the triangle
;			  0 - Coordinate is the top left point of the triangle		0000
;			  1 - Coordinate is the top right point of the triangle		0001
;			  2 - Coordinate is the bottom left point of the triangle	0010
;			  3 - Coordinate is the bottom right point of the triangle	0011
; OUTPUT: None
TRI_ROW_DIR	equ		1	; Bit # determining row direction
TRI_COL_DIR	equ		0	; Bit # determining column direction

pt_base		equ		8
pt_row		equ		12
pt_col		equ		14
pt_wid		equ		16
pt_hei		equ		18
pt_dir		equ		21
_plot_triangle:
		link		a6,#0
		movem.l		d0-4,-(sp)

		move.w		pt_row(a6),d0	; Get row for position fix
		move.w		pt_col(a6),d1	; Get col for position fix
		move.b		pt_dir(a6),d2	; Get direction code for row check
		move.w		pt_hei(a6),d3	; Get height
		move.w		pt_wid(a6),d4	; Get triangle_base (width)

		; Prepare registers for straight line drawing
		suba.w		#10,sp
		move.l		pt_base(a6),(sp)	; 1: base	0(sp)
		move.w		d0,4(sp)		; 2: row	4(sp)
		move.w		d1,6(sp)		; 3: col	6(sp)
		move.w		d3,8(sp)		; 4: length	8(sp)							

		subq.w		#1,d3		; Trim height for row offset

		; Test row direction
		btst.l		#TRI_ROW_DIR,d2
		beq.b		pt_vert
		; Row goes down, adjust line params
		sub.w		d3,4(sp)	; New starting row moved to top of line
		neg.w		d3		; Height = -height
pt_vert:	; Plot vertical line
		jsr		_plot_vertical_line

		move.w		d0,4(sp)	; Reset row
		move.w		d4,8(sp)	; Pass width to line drawing subroutine

		subq.w		#1,d4		; Trim width for col offset

		; Test col direction
		btst.l		#TRI_COL_DIR,d2
		beq.b		pt_horiz
		; Col goes down, adjust params
		sub.w		d4,6(sp)	; Move starting col to end of line
		neg.w		d4		; Width = -width
pt_horiz:
		jsr		_plot_horizontal_line

		; Plot hypotenuse
		subq.w		#2,sp		; Move sp back 2 more bytes for general line plot
		move.l		pt_base(a6),(sp)	; 1: base	0(sp)
		
		add.w		d3,d0	; Increment row
		move.w		d0,4(sp)		; 2: start_row	4(sp)
		move.w		d1,6(sp)		; 3: end_row	6(sp)
		
		sub.w		d3,d0	; Reset row
		add.w		d4,d1	; Increment col
		move.w		d0,8(sp)		; 4: end_row	8(sp)
		move.w		d1,10(sp)		; 5: end_col	10(sp)

		jsr		_plot_line

		adda.w		#12,sp

		movem.l		(sp)+,d0-4
		unlk		a6
		rts

; ==== void plot_8bit_bitmap(UINT8 *base, int row, int col, const UINT8 *bitmap, UINT16 height) ====
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

; ==== void plot_16bit_bitmap(UINT16 *base, int row, int col, const UINT16 *bitmap, UINT16 height) ====
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

; ==== void plot_32bit_bitmap(UINT32 *base, int row, int col, const UINT32 *bitmap, UINT16 height) ====
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
		cmpi.w		#BYTE_COLS-4,d1
		bhs.b		bm32_right_loop	; Print only one long at the end of the screen

		moveq.l		#32,d1		; d1.w = 32
		sub.w		d2,d1		; d1.w = 32-(col % 32)

bm32_shift_loop:
		move.l		(a1)+,d3	; d3.l = Bitmap row
		move.l		d3,d4		; d4.l = Copy of bitmap row
		lsr.l		d2,d3		; d3.l = Shifted start byte
		lsl.l		d1,d4		; d4.l = Shifted end byte

		or.l		d3,(a0)+	; Apply bitmap row masks to framebuffer
		or.l		d4,(a0)

		adda.w		#BYTE_COLS-4,a0	; Move to next row

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

; ==== void plot_character(UINT8 *base, int row, int col, char ch) ====
; PURPOSE: Plots a single character, as a bitmap from a font table, to the screen.
; INPUT: Address(UINT8*): to the start of the screen
;		Position(row,col): the coordinates of the top left pixel of the character
;		ch(char): the character to be written to the screen
; OUTPUT: None

FONT_HEIGHT	equ		8

pc_base		equ		8
pc_row		equ		12
pc_col		equ		14
pc_char		equ		17
_plot_character:
		link	a6,#0
		movem.l	d0/a0,-(sp)

		; IS_PRINTABLE
		move.b		pc_char(a6),d0	; Get char #
		cmpi.b		#' ',d0		; Compare to space (ASCII 32) for printable check
		blt.b		pc_end		; Below printable range, exit
		cmpi.b		#'~',d0		; Compare to tilde (ASCII 126) for printable check
		bgt.b		pc_end		; Above printable range, exit

		; GLYPH_START
		ext.w		d0
		subi.w		#' ',d0		; Get font table symbol index
		mulu.w		#FONT_HEIGHT,d0		; Get font table byte address (x8 for 8px height)

		lea		font,a0		; a0 = font base pointer
		adda.w		d0,a0		; Offset by # of bytes

		; Push parameters for bitmap subroutine
		move.w		#FONT_HEIGHT,-(sp)	; Set height
		move.l		a0,-(sp)		; Pass bitmap char pointer
		move.w		pc_col(a6),-(sp)	; Pass col
		move.w		pc_row(a6),-(sp)	; Pass row
		move.l		pc_base(a6),-(sp)	; Pass framebuffer base pointer
		jsr		_plot_8bit_bitmap	; Plot bitmap
pc_end:
		adda.w		#14,sp		; Super pop
		movem.l		(sp)+,d0/a0
		unlk		a6
		rts

; ==== void plot_string(UINT8 *base, int row, int col, char *ch) ====
; PURPOSE: Plots a string, as a sequence of bitmaps from a font table, to the screen.
; INPUT: Address(UINT8*): to the start of the screen
;	 Position(row,col): the coordinates of the top left pixel of the string
;	 ch(c-string): the string to be written to the screen
; OUTPUT: None

pst_base	equ		8
pst_row		equ		12
pst_col		equ		14
pst_char	equ		16
_plot_string:
		link		a6,#0
		movem.l		a5,-(sp)

		move.l		pst_char(a6),a5	; a5.l = String char pointer

		suba.w		#10,sp

		move.l		pst_base(a6),(sp)	; 0: base	0(sp)
		move.w		pst_row(a6),4(sp)	; 1: row	4(sp)
		move.w		pst_col(a6),6(sp)	; 2: col	6(sp)
		move.b		#0,8(sp)	; Clear top byte of param 3 for char
pst_loop:
		move.b		(a5)+,9(sp)	; 3: char	9(sp)
		beq		pst_end		; Stop printing on null terminator
		; More to print
		jsr		_plot_character

		addq.w		#8,6(sp)
		cmpi.w		#COLS-8,6(sp)
		ble.b		pst_loop
		; Wrap to next line
		move.w		#0,6(sp)
		add.w		#FONT_HEIGHT,4(sp)
		bra		pst_loop

pst_end:
		adda.w		#10,sp
		movem.l		(sp)+,a5
		unlk		a6
		rts

; ==== void region_shift_left(base, row, col, height, width) ====
; Purpose: 

zeros:		ds.l		13

; 8x8 Font Bitmap table
font:           dc.b            $00,$00,$00,$00,$00,$00,$00,$00  ; <space>
                dc.b            $30,$30,$30,$30,$00,$30,$00,$00  ; !
                dc.b            $D8,$D8,$00,$00,$00,$00,$00,$00  ; "
                dc.b            $48,$FC,$48,$48,$FC,$48,$00,$00  ; #
                dc.b            $10,$3C,$70,$38,$1C,$78,$10,$00  ; $
                dc.b            $C4,$C8,$10,$20,$4C,$8C,$00,$00  ; %
                dc.b            $20,$70,$20,$74,$D8,$74,$00,$00  ; &
                dc.b            $30,$30,$20,$00,$00,$00,$00,$00  ; '
                dc.b            $18,$30,$30,$30,$30,$18,$00,$00  ; (
                dc.b            $60,$30,$30,$30,$30,$60,$00,$00  ; )
                dc.b            $00,$50,$20,$F8,$20,$50,$00,$00  ; *
                dc.b            $00,$20,$20,$F8,$20,$20,$00,$00  ; +
                dc.b            $00,$00,$00,$00,$30,$30,$20,$00  ; ,
                dc.b            $00,$00,$00,$F8,$00,$00,$00,$00  ; -
                dc.b            $00,$00,$00,$00,$30,$30,$00,$00  ; .
                dc.b            $00,$08,$10,$20,$40,$80,$00,$00  ; /
                dc.b            $7C,$E2,$E2,$E2,$E2,$7C,$00,$00  ; 0
                dc.b            $18,$38,$78,$38,$38,$38,$00,$00  ; 1
                dc.b            $3C,$4E,$0E,$1C,$38,$7E,$00,$00  ; 2
                dc.b            $3C,$4E,$1C,$0E,$4E,$3C,$00,$00  ; 3
                dc.b            $0C,$1C,$3C,$5C,$7E,$1C,$00,$00  ; 4
                dc.b            $7E,$70,$7C,$0E,$4E,$3C,$00,$00  ; 5
                dc.b            $18,$30,$7C,$72,$72,$3C,$00,$00  ; 6
                dc.b            $7E,$0E,$0E,$1C,$38,$38,$00,$00  ; 7
                dc.b            $3C,$72,$3C,$4E,$4E,$3C,$00,$00  ; 8
                dc.b            $3C,$4E,$4E,$3E,$0C,$18,$00,$00  ; 9
                dc.b            $00,$60,$60,$00,$60,$60,$00,$00  ; :
                dc.b            $00,$60,$60,$00,$60,$60,$40,$00  ; ;
                dc.b            $00,$18,$30,$60,$30,$18,$00,$00  ; <
                dc.b            $00,$00,$F8,$00,$F8,$00,$00,$00  ; =
                dc.b            $00,$60,$30,$18,$30,$60,$00,$00  ; >
                dc.b            $78,$9C,$18,$30,$00,$30,$00,$00  ; ?
                dc.b            $78,$D4,$AC,$BC,$C0,$78,$00,$00  ; @
                dc.b            $38,$38,$5C,$7C,$8E,$8E,$00,$00  ; A
                dc.b            $FC,$E2,$FC,$E2,$E2,$FC,$00,$00  ; B
                dc.b            $3C,$66,$E0,$E0,$66,$3C,$00,$00  ; C
                dc.b            $F8,$E4,$E2,$E2,$E4,$F8,$00,$00  ; D
                dc.b            $FE,$E0,$FC,$E0,$E0,$FE,$00,$00  ; E
                dc.b            $FE,$E0,$FC,$E0,$E0,$E0,$00,$00  ; F
                dc.b            $3C,$66,$E0,$EE,$62,$3C,$00,$00  ; G
                dc.b            $E2,$E2,$FE,$E2,$E2,$E2,$00,$00  ; H
                dc.b            $38,$38,$38,$38,$38,$38,$00,$00  ; I
                dc.b            $0E,$0E,$0E,$0E,$8E,$7C,$00,$00  ; J
                dc.b            $E2,$E4,$E8,$F8,$E4,$E2,$00,$00  ; K
                dc.b            $E0,$E0,$E0,$E0,$E0,$FE,$00,$00  ; L
                dc.b            $86,$CE,$FE,$AE,$8E,$8E,$00,$00  ; M
                dc.b            $E2,$F2,$FA,$BE,$9E,$8E,$00,$00  ; N
                dc.b            $38,$64,$E2,$E2,$64,$38,$00,$00  ; O
                dc.b            $F8,$E4,$E2,$E4,$F8,$E0,$00,$00  ; P
                dc.b            $38,$64,$E2,$E2,$64,$3E,$00,$00  ; Q
                dc.b            $F8,$E4,$E2,$E4,$F8,$E6,$00,$00  ; R
                dc.b            $7C,$F2,$78,$3C,$9E,$7C,$00,$00  ; S
                dc.b            $FE,$38,$38,$38,$38,$38,$00,$00  ; T
                dc.b            $E2,$E2,$E2,$E2,$E2,$7C,$00,$00  ; U
                dc.b            $E2,$E2,$74,$74,$38,$38,$00,$00  ; V
                dc.b            $E2,$E2,$EA,$FE,$E6,$C2,$00,$00  ; W
                dc.b            $E2,$F4,$78,$3C,$5E,$8E,$00,$00  ; X
                dc.b            $E2,$F4,$78,$30,$20,$40,$00,$00  ; Y
                dc.b            $FE,$1E,$3C,$78,$F0,$FE,$00,$00  ; Z
                dc.b            $3C,$30,$30,$30,$30,$3C,$00,$00  ; [
                dc.b            $00,$80,$40,$20,$10,$08,$00,$00  ; \
                dc.b            $F0,$30,$30,$30,$30,$F0,$00,$00  ; ]
                dc.b            $20,$70,$F8,$20,$20,$20,$00,$00  ; ^
                dc.b            $00,$00,$00,$00,$00,$00,$FE,$00  ; _
                dc.b            $30,$30,$10,$00,$00,$00,$00,$00  ; `
				dc.b            $38,$38,$5C,$7C,$8E,$8E,$00,$00  ; A
                dc.b            $FC,$E2,$FC,$E2,$E2,$FC,$00,$00  ; B
                dc.b            $3C,$66,$E0,$E0,$66,$3C,$00,$00  ; C
                dc.b            $F8,$E4,$E2,$E2,$E4,$F8,$00,$00  ; D
                dc.b            $FE,$E0,$FC,$E0,$E0,$FE,$00,$00  ; E
                dc.b            $FE,$E0,$FC,$E0,$E0,$E0,$00,$00  ; F
                dc.b            $3C,$66,$E0,$EE,$62,$3C,$00,$00  ; G
                dc.b            $E2,$E2,$FE,$E2,$E2,$E2,$00,$00  ; H
                dc.b            $38,$38,$38,$38,$38,$38,$00,$00  ; I
                dc.b            $0E,$0E,$0E,$0E,$8E,$7C,$00,$00  ; J
                dc.b            $E2,$E4,$E8,$F8,$E4,$E2,$00,$00  ; K
                dc.b            $E0,$E0,$E0,$E0,$E0,$FE,$00,$00  ; L
                dc.b            $86,$CE,$FE,$AE,$8E,$8E,$00,$00  ; M
                dc.b            $E2,$F2,$FA,$BE,$9E,$8E,$00,$00  ; N
                dc.b            $38,$64,$E2,$E2,$64,$38,$00,$00  ; O
                dc.b            $F8,$E4,$E2,$E4,$F8,$E0,$00,$00  ; P
                dc.b            $38,$64,$E2,$E2,$64,$3E,$00,$00  ; Q
                dc.b            $F8,$E4,$E2,$E4,$F8,$E6,$00,$00  ; R
                dc.b            $7C,$F2,$78,$3C,$9E,$7C,$00,$00  ; S
                dc.b            $FE,$38,$38,$38,$38,$38,$00,$00  ; T
                dc.b            $E2,$E2,$E2,$E2,$E2,$7C,$00,$00  ; U
                dc.b            $E2,$E2,$74,$74,$38,$38,$00,$00  ; V
                dc.b            $E2,$E2,$EA,$FE,$E6,$C2,$00,$00  ; W
                dc.b            $E2,$F4,$78,$3C,$5E,$8E,$00,$00  ; X
                dc.b            $E2,$F4,$78,$30,$20,$40,$00,$00  ; Y
                dc.b            $FE,$1E,$3C,$78,$F0,$FE,$00,$00  ; Z
                dc.b            $3C,$30,$E0,$30,$30,$3C,$00,$00  ; {
                dc.b            $20,$20,$20,$20,$20,$20,$00,$00  ; |
                dc.b            $F0,$30,$1C,$30,$30,$F0,$00,$00  ; }
