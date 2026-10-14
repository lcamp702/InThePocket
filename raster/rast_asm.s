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
;		plot_line
;		plot_rectangle
;		plot_square
;		plot_triangle
;		plot_8bit_bitmap
;		plot_16bit_bitmap
;		plot_32bit_bitmap
;	  Visual Tests:
;		clear_screen
;		clear_region
;		plot_pixel
;		plot_horizontal_line (c)
;		plot_vertical_line
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

;       Screen details
ROWS		equ		400

COLS		equ		640
BYTE_COLS	equ		80
LONG_COLS	equ		20

;	_clear_screen(UINT32 *base)
cs_base		equ		64	; Offset from SP, not A6

;	_clear_region(UINT32 *base, int row, int col, UINT16 length, UINT16 width)
cr_base		equ		44
cr_row		equ		48
cr_col		equ		50
cr_len		equ		52
cr_wid		equ		54

;	_plot_pixel(UINT8 *base, int row, int col)
px_base		equ		20
px_row		equ		24
px_col		equ		26

;	_plot_horizontal_line(UINT32 *base, int row, int col, UINT16 length)
hl_base		equ		36
hl_row		equ		40
hl_col		equ		42
hl_len		equ		44

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
_clear_region:
		movem.l		d0-6/a0-2,-(sp)

		; Copy params to registers and bound check
		move.w		cr_row(sp),d0	; d0.w = Row #
		cmp.w		#ROWS,d0
		bhs.w		cr_end		; Fast row bound check [1]

		move.w		cr_col(sp),d1	; d1.w = Column #
		cmp.w		#COLS,d1
		bhs.w		cr_end		; Fast col bound check [1]

		move.w		cr_wid(sp),d2	; d2.w = Width
		ble.w		cr_end		; if (width < 0) return

		move.w		cr_len(sp),d3	; d3.w = Length
		ble.w		cr_end		; if (length < 0) return

		movea.l		cr_base(sp),a0	; a0.l = framebuffer base
		movea.l		a0,a1		; a1.l = framebuffer base

		; Calculate end row offset
		move.w		d0,d4
		add.w		d3,d4		; d4.w = row + length

		; Clip length
		cmp.w		#ROWS,d4
		ble.b		cr_row_clipped	; If end row < ROWS continue
		move.w		#ROWS,d3
		move.w		d3,d4		; d4.w = Clipped end row
		sub.w		d0,d3		; d3.w = Clipped length
cr_row_clipped:
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
		andi.w		#$FFFC,d1	; d1.w = Start byte offset (long aligned)
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

		move.l		#-1,d5		; d5.l = (0xFFFFFFFF)
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

		move.l		#-1,d0		; d0.l = (0xFFFFFFFF)
		lsr.l		d4,d0		; d0.l = Shifted end mask

		bra.b		cr_blank
cr_end_zero:	; Create zero end mask
		moveq.l		#0,d0		; d0.l = End mask (0x00000000)
cr_blank:	; Start blanking the screen
		move.l		#BYTE_COLS,d6		; d1.l = #80 for quicker sub loop
						; 8(length) - 12 clocks speedup. (Slower for length = 1)

		subq.w		#1,d3

		cmp.w		d1,d2
		bne		cr_thick

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
		add.w		d2,d0		; d0.w = row * 80 = row * BYTE_COLS
		
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

_plot_horizontal_line:
		movem.l		d0-5/a0,-(sp)

		; Copy params to registers and bound check
		move.w		hl_row(sp),d0	; d0.w = Row #
		cmp.w		#ROWS,d0
		bhs.w		cr_end		; Fast row bound check [1]

		move.w		hl_col(sp),d1	; d1.w = Column #
		cmp.w		#COLS,d1
		bhs.w		cr_end		; Fast col bound check [1]

		move.w		hl_len(sp),d2	; d2.w = Length
		ble.w		hl_end		; if (length < 0) return

		add.w		d1,d2		; d2.w = End col + 1
		; Length clipping
		cmpi.w		#COLS,d2
		blt.b		hl_clipped

		move.w		#COLS,d2
hl_clipped:
		subq.w		#1,d2		; d2.w = Clipped end col
		
		; Calculate shifts and make masks
		move.w		d1,d3		; d3.w = Start col
		andi.w		#31,d3		; d3.w = Start mask right shift amount

		move.l		#-1,d4		; d4.l = 0xFFFFFFFF
		lsr.l		d3,d4		; d4.l = Shifted start long

		move.w		d2,d3		; d3.w = End col
		not.w		d3
		andi.w		#$1F,d3		; d3.w = End mask left shift amount

		move.l		#-1,d5		; d5.l = 0xFFFFFFFF
		lsl.l		d3,d5		; d5.l = Shifted end long

		move.w		d0,d3
		lsl.w		#4,d3
		lsl.w		#6,d0
		add.w		d3,d0		; d0.w = Row byte offset

		move.w		d1,d3
		lsr.w		#3,d3		; d3.w = Start long offset
		andi.w		#$FFFC,d3	; d3.w = Start byte offset (long aligned)

		add.w		d3,d0		; d0.w = Total byte offset

		movea.l		hl_base(sp),a0	; a0.l = Framebuffer base
		adda.w		d0,a0		; a0.l = First long to write to

		lsr.w		#5,d1		; d1.w = Start long offset
		lsr.w		#5,d2		; d2.w = End long offset

		cmp.w		d1,d2
		bne		hl_long

		; Line fits within one long
		and.l		d5,d4		; d4.l = Full mask
		or.l		d4,(a0)		; Write mask to memory
hl_long:
		; Line spans multiple longs

		sub.w		d2,d1		; Total longs to print
		subq.w		#2,d1		; Fill loop counter
		blt.b		hl_long_end

		or.l		d4,(a0)+

hl_fill_loop:
		move.l		#-1,d4		; d4.l = 0xFFFFFFFF
		move.l		d4,(a0)+	; Write filled long to memory
		dbra		d1,hl_fill_loop
hl_long_end:
		or.l		d5,(a0)+

hl_end:
		movem.l		(sp)+,d0-5/a0
		rts

; ==== void plot_vertical_line(UINT32 *base, int row, int col, UINT16 length) ====
; PURPOSE: Plot a vertical line on the screen. The vertical line is specified by the topmost pixel of the line and the length of the line.
; INPUT: Address(UINT32*): to the start of the screen
;	 Position(row,col): the coordinates of the topmost pixel of the vertical line
;	 Length: the length in pixels of the line
; OUTPUT: None
_plot_vertical_line:
		movem.l		d0-4/a0,-(sp)

		move.w		vl_row(sp),d0	; d0.w = Row #
		cmp.w		#ROWS,d0
		bhs.b		vl_end		; Fast row bound check [1]

		move.w		vl_col(sp),d1	; d1.w = Column #
		cmp.w		#COLS,d1	
		bhs.b		vl_end		; Fast col bound check [1]

		move.w		vl_len(sp),d3	; d3.w = length
		ble.b		vl_end		; Zero / negative length check
				
		; Row clipping check
		move.w		d0,d2
		add.w		d3,d2		; d2.w = end row
		
		cmp.w		#ROWS,d2
		bls.b		vl_fits		; If end row < rows, line fits, continue

		move.w		#ROWS,d3	
		sub.w		d0,d3		; If end row >= rows, clip line length

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