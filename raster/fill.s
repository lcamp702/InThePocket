;	Screen details
ROWS		equ		400
COLS		equ		640
BYTE_COLS	equ		80

; ==== void fill_region(UINT32 *base, int row, int col, UINT16 length, UINT16 width) ====
; PURPOSE: Fills a region of the screen. The section is specified by the coordinates of the top left corner, and the height and width of the region.
; INPUT: Address(UINT32*): to the start of the screen
;	   Position(row,col): the coordinates of the top left pixel of the region
;	   Length: the length (number of rows) in pixels of the region
;	   Width: the width (number of columns) in pixels of the region
; OUTPUT: None
		xdef		_fill_region

fr_base		equ		44
fr_row		equ		48
fr_col		equ		50
fr_len		equ		52
fr_wid		equ		54
_fill_region:
		movem.l		d0-6/a0-2,-(sp)

		; Copy params to registers and bound check
		move.w		fr_len(sp),d3	; d3.w = Length
		beq.w		fr_end		; if (length == 0) return

		move.w		fr_row(sp),d0	; d0.w = Row #
		bpl.b		fr_pos_row
		neg.w		d0		
		sub.w		d0,d3		; Subtract from length	
		bls.w		fr_end		; Not on screen, exit
		clr.w		d0		; Reset row to 0
fr_pos_row:
		cmp.w		#ROWS,d0
		bge.w		fr_end		; Row upper bound check

		move.w		fr_wid(sp),d2	; d2.w = Width
		ble.w		fr_end		; if (width < 0) return

		move.w		fr_col(sp),d1	; d1.w = Column #
		bpl.b		fr_pos_col
		neg.w		d1		
		sub.w		d1,d2		; Subtract from width	
		bls.w		fr_end		; Not on screen, exit
		clr.w		d1		; Reset start col to 0
fr_pos_col:
		cmp.w		#COLS,d1	
		bge.w		fr_end		; Col upper bound check

		movea.l		fr_base(sp),a0	; a0.l = framebuffer base
		movea.l		a0,a1		; a1.l = framebuffer base

		; Calculate end row offset
		move.w		d0,d4
		add.w		d3,d4		; d4.w = row + length

		; Clip length
		cmp.w		#ROWS,d4
		ble.b		fr_row_clipped	; If end row < ROWS continue
		move.w		#ROWS,d3	; End row is too large, clip it
		move.w		d3,d4		; d4.w = Clipped end row
		sub.w		d0,d3		; d3.w = Clipped length
fr_row_clipped:	; End row is on the screen
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
		andi.w		#$FFFC,d1		; d1.w = Start byte offset (long aligned)
		add.w		d4,d1		
		adda.w		d1,a1		; End pointer is at the first pixel of the last row to clear

		; Clip width
		cmp.w		#COLS,d2	
		ble.w		fr_col_clipped	; If end col < COLS continue
		move.w		#COLS,d2
fr_col_clipped:
		subq.w		#1,d2		; d2.w = End col

		move.w		d2,d4		; d4.w = Copy of end col for shift calc

		move.w		d0,d1		; d1.w = Start col
		lsr.w		#5,d1		; d1.w = Start long offset
		lsr.w		#5,d2		; d2.w = End long offset

		; Create shifted start mask

		andi.w		#31,d0		; d0.w = Start col % 32
		moveq.l		#-1,d5		; d5.l = (0xFFFFFFFF)
		lsr.l		d0,d5		; d5.l = Shifted start mask

		andi.w		#31,d4		; d4.w = End col % 32
		neg.w		d4		; d4.w = -(End col % 32)
		addi.w		#31,d4		; d4.w = End mask left shift val

		; Create shifted end mask
		moveq.l		#-1,d0		; d0.l = (0xFFFFFFFF)
		lsl.l		d4,d0		; d0.l = Shifted end mask

		; Start filling the screen
		move.l		#BYTE_COLS,d6	; d1.l = #80 for quicker sub loop
						; 8(length) - 12 clocks speedup. (Slower for length = 1)

		subq.w		#1,d3

		cmp.w		d1,d2
		bne.b		fr_thick

		; Clearing mask fits in one long
		and.l		d5,d0		; d0.l = Full mask
fr_thin_loop:	; Apply mask over entire length
		or.l		d0,(a1)		; Apply mask to target long in memory
		suba.l		d6,a1		; Move back one row
		dbra.w		d3,fr_thin_loop
		bra		fr_end

fr_thick:	; Clearing spans multiple longs
		moveq.l		#-1,d4		; d4.l = ones mask
		sub.w		d1,d2		; d2 = End offset - start offset
		subq.w		#2,d2		; d2 = Fills count

fr_thick_loop:
		move.w		d2,d1		; d1.w = Row fill counter

		movea.l		a1,a2		; a2.l = Row cursor
		suba.l		d6,a1		; Move a1 back one row

		or.l		d5,(a2)+	; Apply start mask to long in memory

		tst.w		d2
		blt.b		fr_thick_end	; Skip fills if fill count < 0
fr_thick_fill:
		move.l		d4,(a2)+	; Clear long in memory
		dbra.w		d1,fr_thick_fill

fr_thick_end:
		or.l		d0,(a2)		; Apply end mask to long in memory
		dbra.w		d3,fr_thick_loop	

fr_end:
		movem.l		(sp)+,d0-6/a0-2
		rts


; ==== void clear_20l(UINT32 *base, int row, UINT16 height) ====
; PURPOSE: Clears entire scanlines.
; INPUT: Address(UINT32*): to the start of the screen
;	height(UINT16): the number of lines to clear
; OUTPUT: None
        	xdef		_clear_20l

cl_base		equ		52	; Offset from SP, not A6
cl_row		equ		56
cl_height	equ		58
_clear_20l:	; Quick fill code based on Raster Lab on D2L
		movem.l		d0-7/a0-3,-(sp) ; Store existing register values

		lea		zeros,a0
		movem.l		(a0)+,d1-7/a1-3	; Clear registers d1-7 and a1-3

		move.w		cl_height(sp),d0	; Set loop counter to height
		beq.b		cl_end		; End if no height

		move.w		cl_row(sp),d1	; d1.w = Row #
		bpl.b		cl_pos_row
		neg.w		d1		
		sub.w		d1,d0		; Subtract from height	
		bls.w		cl_end		; Not on screen, exit
		clr.w		d1		; Reset row to 0
cl_pos_row:
		cmp.w		#ROWS,d1
		bge.w		cl_end		; Row upper bound check

		add.w		d0,d1		; d1.w = row + height
		move.w		d1,d2
		lsl.w		#6,d1
		lsl.w		#4,d2
		add.w		d2,d1		; d1.w = 80 * (row + height)

		movea.l		d1,a0		; a0 = framebuffer offset
		cmpa.l		#32000,a0	; check if offset is within framebuffer
		ble		cl_print
		movea.l		#32000,a0	; clip to end of framebuffer
cl_print:
		moveq.l		#0,d1		; Re-clear
		moveq.l		#0,d2
		adda.l		cl_base(sp),a0	; a0 points to end of clearing row in framebuffer

		subq.w		#1,d0

cl_loop:
		movem.l		d1-7/a1-3,-(a0) ; Clear 10 longs at once and decrement
		movem.l		d1-7/a1-3,-(a0)	; Clear remaining 10 longs and decrement
		dbra		d0,cl_loop
cl_end:
		movem.l		(sp)+,d0-7/a0-3 ; Return existing register values
		rts

		even
zeros:		ds.l		10