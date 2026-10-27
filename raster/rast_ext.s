;	Peter Ursem
;	purse327@mtroyal.ca

;	Logan Campbell
;	lcamp702@mtroyal.ca

;	COMP2659-001
;	Steve Kalmar

;	In the Pocket - Checkpoint 3
;	Due: Nov 9, 2026
;	raster/rast_ext.s    

;	Status:
;	  Not Tested:
;
;	  Visual Tests:
;		fill_screen
;		fill_region
;		plot_n8bit_bitmap
;		plot_n16bit_bitmap
;		plot_n32bit_bitmap
;		plot_ncharacter
;		plot_nstring
;		plot_20l_bitmap

;	Purpose:
;	Extra functions for the raster engine for our game, In the Pocket on the Atari ST.


;		xdef		_shift_region_left

;       Screen details
ROWS		equ		400
COLS		equ		640
BYTE_COLS	equ		80

; ==== void fill_screen(UINT32 *base) ====
; PURPOSE: Fills the entire screen.
; INPUT: Address(UINT32*): to the start of the screen
; OUTPUT: None
        	xdef		_fill_screen

fs_base		equ		64	; Offset from SP, not A6
_fill_screen:	; Quick fill code based on Raster Lab on D2L
		movem.l		d0-7/a0-6,-(sp) ; Store existing register values

		lea		ones,a0
		movem.l		(a0)+,d1-7/a1-6	; Clear registers d1-7 and a1-6

		movea.l		fs_base(sp),a0	; a0 points framebuffer base
		adda.l		#32000,a0	; a0 points to end of framebuffer
		move.w		#614,d0	; Set loop counter to 614 (615 * 13 = 7995 longs)
fill_loop:
		movem.l		d1-7/a1-6,-(a0) ; Clear 13 longs at once and decrement
		dbra		d0,fill_loop
		; End loop
		movem.l		d1-5,-(a0)	; Clear remaining 5 longs

		movem.l		(sp)+,d0-7/a0-6 ; Return existing register values
		rts

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
		addi.w		#32,d4		; d4.w = End mask left shift val

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

; ==== void plot_n8bit_bitmap(UINT8 *base, int row, int col, const UINT8 *bitmap, UINT16 height) ====
; PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.
; INPUT: Address(UINT8*): to the start of the screen
;	 Position(row,col): the coordinates of the top left pixel of the bitmap
;	 Bitmap(UINT8*): pointer to the bitmap to print
;	 Height: the length (number of rows) of the height in pixels of the bitmap
; OUTPUT: None
		xdef		_plot_n8bit_bitmap

nbm8_base	equ		24
nbm8_row	equ		28
nbm8_col	equ		30
nbm8_bmp	equ		32
nbm8_hei	equ		36
_plot_n8bit_bitmap:
		movem.l		d0-2/a0-1,-(sp)

		; Height must be > 0
		move.w		nbm8_hei(sp),d0	; d0.w = height
		beq.w		nbm8_end	; if (height == 0) return

		; Load bitmap pointer
		movea.l		nbm8_bmp(sp),a1	; a1.l = Bitmap base pointer

		; Load row
		move.w		nbm8_row(sp),d1	; d1.w = Row #
		bpl.b		nbm8_bottom	; Row > 0, no top clipping

		; Top clipping
		neg.w		d1
		sub.w		d1,d0		; Subtract from print height
		bls.w		nbm8_end	; If height < negative row count, nothing to draw

		; Top clipped but partially visible
		adda.w		d1,a1		; Offset bitmap pointer by # of negative rows
		clr.w		d1		; Zero start row
		
nbm8_bottom:
		cmpi.w		#ROWS,d1
		bhs.w		nbm8_end	; If row >= max row, nothing to draw

		; Bottom clipping
		move.w		#ROWS,d2
		sub.w		d1,d2		; d2.w = Max visible height
		cmp.w		d2,d0
		blo.b		nbm8_clipped_row

		; Bottom clipped but bitmap partially appears.
		move.w		d2,d0		; Clamp height to visible height

nbm8_clipped_row:	; Clipped row + height found
		subq.w		#1,d0		; Predecrement height for print loops

		; Move framebuffer pointer to first byte of the row
		movea.l		nbm8_base(sp),a0	; a0.l = Framebuffer base pointer

		move.w		d1,d2
		lsl.w		#4,d1
		lsl.w		#6,d2
		add.w		d2,d1		; d1.w = Row byte offset

		adda.w		d1,a0		; a0 points to start of row to draw on

		; Load column
		move.w		nbm8_col(sp),d1	; d2.w = Column #
		bpl.b		nbm8_right

		; Left clipping
		neg.w		d1		; d1.w = negative col count
		cmpi		#8,d1
		bhs.b		nbm8_end	; If negative col count >= 8, nothing to draw

nbm8_left_loop:	; No column offset, draw shifted bitmap on the left
		move.b		(a1)+,d2	; Load next bitmap row into register
		lsl.b		d1,d2		; Shift bitmap row
		not.b		d2		; Invert (Negative printing)
		and.b		d2,(a0)		; Apply mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next row of framebuffer
		dbra		d0,nbm8_left_loop
		bra.b		nbm8_end	; Done drawing

nbm8_right:	; Col > 0, no left clip
		cmpi.w		#COLS,d1
		bhs.b		nbm8_end		; If col >= max col, nothing to draw

		move.w		d1,d2		; d2.w = Copy of col # for offset calc
		lsr.w		#3,d1		; d2.w = Col byte offset
		adda.w		d1,a0		; a0.l = Pointer to first byte to write to

		; Get bit shift
		andi.w		#7,d2		; d2.w = Start shift right amount (col % 8)
		bne		nbm8_shifted	
		
nbm8_aligned_loop:	; If start shift == 0, print bmp into bytes
		move.b		(a1)+,d1	; d1.b = Bitmap row
		not.b		d1		; Invert (Negaitve printing)
		and.b		d1,(a0)		; Apply bitmap row mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next line of the framebuffer
		dbra		d0,nbm8_aligned_loop

		bra		nbm8_end

nbm8_shifted:	; If start shift != 0, shift bmp across 2 bytes
		cmpi.w		#BYTE_COLS-1,d1
		bhs.b		nbm8_right_loop

nbm8_shift_loop:
		clr.w		d1		; Clear word to shift byte within
		move.b		(a1)+,d1	; d1.b = Bitmap row
		ror.w		d2,d1		; d1.b = Shifted start byte
		not.b		d1		; Invert (Nevative printing)
		and.b		d1,(a0)+	; Apply bitmap row masks to framebuffer
		ror.w		#8,d1		; d1.b = Shifted end byte
		and.b		d1,(a0)

		adda.w		#BYTE_COLS-1,a0	; Move to next row

		dbra		d0,nbm8_shift_loop
		bra.b		nbm8_end

nbm8_right_loop:
		move.b		(a1)+,d1	; d1.b = Bitmap row
		lsr.b		d2,d1		; Shift right to clip
		not.b		d1		; Invert (negative printing)
		and.b		d1,(a0)		; Apply bitmap row to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next row
		dbra		d0,nbm8_right_loop

nbm8_end:
		movem.l		(sp)+,d0-2/a0-1
		rts

; ==== void plot_n16bit_bitmap(UINT16 *base, int row, int col, const UINT16 *bitmap, UINT16 height) ====
; PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.
; INPUT: Address(UINT16*): to the start of the screen
;	 Position(row,col): the coordinates of the top left pixel of the bitmap
;	 Bitmap(UINT16*): pointer to the bitmap to print
;	 Height: the length (number of rows) of the height in pixels of the bitmap
; OUTPUT: None
		xdef		_plot_n16bit_bitmap

nbm16_base	equ		24
nbm16_row	equ		28
nbm16_col	equ		30
nbm16_bmp	equ		32
nbm16_hei	equ		36
_plot_n16bit_bitmap:
		movem.l		d0-2/a0-1,-(sp)

		; Height check
		move.w		nbm16_hei(sp),d0	; d0.w = height
		beq.w		nbm16_end	; if (length == 0), nothing to draw

		; Load bitmap pointer
		movea.l		nbm16_bmp(sp),a1	; a1.l = Bitmap base pointer

		; Load row
		move.w		nbm16_row(sp),d1	; d1.w = Row #
		bpl.b		nbm16_bottom	; Row > 0, no top clipping

		; Top clipping
		neg		d1
		sub.w		d1,d0		; Subtract negative row count from height
		bls.w		nbm16_end	; If height < negative row count, nothing to draw

		; Top clipped but bitmap partially appears
		add.w		d1,d1		; d1.w = Bitmap row offset (row * 2)
		adda.w		d1,a1		; Offset bitmap pointer by # of negative rows
		clr.w		d1		; Start row = 0

nbm16_bottom:
		cmpi.w		#ROWS,d1
		bhs.w		nbm16_end	; If row >= max row, nothing to draw

		; Bottom clip
		move.w		#ROWS,d2
		sub.w		d1,d2		; d2.w = Max visible height
		cmp.w		d2,d0
		blo.b		nbm16_clipped_row	; If height < visible height, no clipping

		; Bottom clipped but bitmap partially appears
		move.w		d2,d0		; Clamp height to visible height

nbm16_clipped_row:	; Clipped row + height found
		subq.w		#1,d0		; Predecrement height for print loops

		; Move framebuffer pointer to first byte of the row
		movea.l		nbm16_base(sp),a0	; a0.l = Framebuffer base pointer

		move.w		d1,d2
		lsl.w		#4,d1
		lsl.w		#6,d2
		add.w		d2,d1		; d1.w = Row byte offset

		adda.w		d1,a0		; a0 points to the first row to draw on

		; Load column
		move.w		nbm16_col(sp),d1	; d1.w = Column #
		bpl.b		nbm16_right	; If col > 0, no left clip

		; Left clipping
		neg.w		d1		; d1.w = negative col count
		cmpi.w		#16,d1
		bhs.b		nbm16_end	; If negative col count >= 32, nothing to draw

nbm16_left_loop:	; No column offset needed, draw shifted bitmap on the left
		move.w		(a1)+,d2	; Load next bitmap row into register
		neg.w		d2		; Negate for inverse drawing
		lsl.w		d1,d2		; Shift bitmap row
		and.w		d2,(a0)		; Apply mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next row of framebuffer
		dbra		d0,nbm16_left_loop
		bra.b		nbm16_end	; Done drawing

nbm16_right:	; Col > 0, no left clip
		cmpi.w		#COLS,d1
		bhs.b		nbm16_end	; If col >= max col, nothing to draw

		move.w		d1,d2
		lsr.w		#4,d1		; d1.w = Col word offset
		add.w		d1,d1		; d1.w = Col word aligned byte offset
		adda.w		d1,a0		; a0.l = Pointer to first byte to write to

		; Get bit shift
		andi.w		#15,d2		; d2.w = Start shift right amount (col % 16)
		bne		nbm16_shifted
		
nbm16_aligned_loop:	; If start shift == 0, print bmp into words
		move.w		(a1)+,d1	; d1.w = Bitmap row
		not.w		d1		; Negate for inverse drawing
		and.w		d1,(a0)		; Apply bitmap row mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next line of the framebuffer
		dbra		d0,nbm16_aligned_loop

		bra		nbm16_end

nbm16_shifted:
		cmpi.w		#BYTE_COLS-2,d1
		bhs.b		nbm16_right_loop

nbm16_shift_loop:
		; Speed optimized shift with swap.w and longs
		moveq.l		#0,d1		; Clear long to shift word in
		move.w		(a1)+,d1	; d1.w = Bitmap row
		ror.l		d2,d1		; d1.l = Shifted bitmap
		not.w		d1		; Negate for inverse drawing
		and.w		d1,(a0)+	; Apply start word bitmap row to framebuffer
		swap.w		d1		; d1.w = End word bitmap row
		and.w		d1,(a0)		; Apply end word bitmap row

		adda.w		#BYTE_COLS-2,a0		; Move to next row

		dbra		d0,nbm16_shift_loop
		bra.b		nbm16_end

nbm16_right_loop:
		move.w		(a1)+,d1
		lsr.w		d2,d1
		and.w		d1,(a0)
		adda.w		#BYTE_COLS,a0
		dbra		d0,nbm16_right_loop

nbm16_end:
		movem.l		(sp)+,d0-2/a0-1
		rts

; ==== void plot_n32bit_bitmap(UINT32 *base, int row, int col, const UINT32 *bitmap, UINT16 height) ====
; PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.
; INPUT: Address(UINT32*): to the start of the screen
;	 Position(row,col): the coordinates of the top left pixel of the bitmap
;	 Bitmap(UINT32*): pointer to the bitmap to print
;	 Height: the length (number of rows) of the height in pixels of the bitmap
; OUTPUT: None
		xdef		_plot_n32bit_bitmap

nbm32_base	equ		32
nbm32_row	equ		36
nbm32_col	equ		38
nbm32_bmp	equ		40
nbm32_hei	equ		44
_plot_n32bit_bitmap:
		movem.l		d0-4/a0-1,-(sp)

		; Height must be > 0
		move.w		nbm32_hei(sp),d0	; d0.w = height
		beq.w		nbm32_end	; if (height == 0) return

		; Load bitmap pointer
		movea.l		nbm32_bmp(sp),a1	; a1.l = Bitmap base pointer

		; Load row
		move.w		nbm32_row(sp),d1	; d1.w = Row #
		bpl.b		nbm32_bottom	; Row > 0, no top clipping.

		; Top clipping
		neg.w		d1
		sub.w		d1,d0		; Subtract negative row count from height
		bls.w		nbm32_end	; If height < negative row count, nothing to draw

		; Top clipped but bitmap partially appears
		ext.l		d1
		lsl.l		#2,d1		; Row * 4 = Bitmap row offset
		adda.l		d1,a1		; Offset bitmap pointer by # of negative rows
		clr.w		d1		; Zero start row

nbm32_bottom:
		cmpi.w		#ROWS,d1
		bhs.w		nbm32_end	; If row >= max row, nothing to draw

		; Bottom clip
		move.w		#ROWS,d2
		sub.w		d1,d2		; d2.w = Max visible height (ROWS - row)
		cmp.w		d2,d0
		blo.b		nbm32_clipped_row	; If height < visible height, no clipping

		; Bottom clipped but bitmap partially appears
		move.w		d2,d0		; Clamp height to visible height

nbm32_clipped_row:	; Clipped row + height found
		subq.w		#1,d0		; Predecrement height for print loop

		; Move fb pointer to first byte of row
		movea.l		nbm32_base(sp),a0	; a0.l = Framebuffer base pointer
		move.w		d1,d2
		lsl.w		#4,d1
		lsl.w		#6,d2
		add.w		d2,d1		; d1.w = Row byte offset
		adda.w		d1,a0		; a0 points to the first row to draw on

		; Load column
		move.w		nbm32_col(sp),d1	; d1.w = Column #
		bpl.b		nbm32_right	; If col > 0, no left clip

		; Left clipping
		neg.w		d1		; d1.w = negative col count
		cmpi.w		#32,d1
		bhs.b		nbm32_end	; If negative col count >= 32, nothing to draw

nbm32_left_loop:	; No column offsets needed, so we can draw on the left side
		move.l		(a1)+,d2	; Load bitmap row into register
		lsl.l		d1,d2		; Shift bitmap row
		not.l		d2
		and.l		d2,(a0)		; Apply mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next row of framebuffer
		dbra		d0,nbm32_left_loop
		bra.b		nbm32_end	; Done drawing

nbm32_right:	; Col > 0, no left clip
		cmpi.w		#COLS,d1
		bhs.b		nbm32_end	; If col >= max col, nothing to draw

		move.w		d1,d2
		lsr.w		#5,d1		; d1.w = Col long offset
		lsl.w		#2,d1		; d1.w = Col long aligned byte offset
		adda.w		d1,a0		; a0.l = Pointer to first byte to write to

		; Get bit shift
		andi.w		#31,d2		; d2.w = Start shift right amount (col % 32)
		bne		nbm32_shifted	
		
nbm32_aligned_loop:	; If start shift == 0, print bmp into bytes
		move.l		(a1)+,d3	; d3.l = Bitmap row
		and.l		d3,(a0)		; Apply bitmap row mask to framebuffer
		not.l		d3
		adda.w		#BYTE_COLS,a0	; Move to next line of the framebuffer
		dbra		d0,nbm32_aligned_loop

		bra		nbm32_end

nbm32_shifted:	; If start shift != 0, shift bmp across 2 bytes
		cmpi.w		#BYTE_COLS-4,d1
		bhs.b		nbm32_right_loop	; Print only one long at the end of the screen

		moveq.l		#32,d1		; d1.w = 32
		sub.w		d2,d1		; d1.w = 32-(col % 32)

nbm32_shift_loop:
		move.l		(a1)+,d3	; d3.l = Bitmap row
		move.l		d3,d4		; d4.l = Copy of bitmap row
		lsr.l		d2,d3		; d3.l = Shifted start byte
		lsl.l		d1,d4		; d4.l = Shifted end byte
		not.l		d3
		not.l		d4

		and.l		d3,(a0)+	; Apply bitmap row masks to framebuffer
		and.l		d4,(a0)

		adda.w		#BYTE_COLS-4,a0	; Move to next row

		dbra		d0,nbm32_shift_loop
		bra.b		nbm32_end

nbm32_right_loop:
		move.l		(a1)+,d3	; d3.l = Bitmap row
		not.l		d3
		lsr.l		d2,d3		; Shifter start long
		and.l		d3,(a0)		; Write the left long
		adda.w		#BYTE_COLS,a0	; Move to next row
		dbra		d0,nbm32_right_loop

nbm32_end:
		movem.l		(sp)+,d0-4/a0-1
		rts

; ==== void plot_ncharacter(UINT8 *base, int row, int col, char ch) ====
; PURPOSE: Plots a single character, as a bitmap from a font table, to the screen.
; INPUT: Address(UINT8*): to the start of the screen
;		Position(row,col): the coordinates of the top left pixel of the character
;		ch(char): the character to be written to the screen
; OUTPUT: None
		xdef		_plot_ncharacter
		xref		font

FONT_HEIGHT	equ		8

pn_base		equ		8
pn_row		equ		12
pn_col		equ		14
pn_char		equ		17
_plot_ncharacter:
		link	a6,#0
		movem.l	d0/a0,-(sp)

		; IS_PRINTABLE
		move.b		pn_char(a6),d0	; Get char #
		cmpi.b		#' ',d0		; Compare to space (ASCII 32) for printable check
		blt.b		pn_end		; Below printable range, exit
		cmpi.b		#'~',d0		; Compare to tilde (ASCII 126) for printable check
		bgt.b		pn_end		; Above printable range, exit

		; GLYPH_START
		ext.w		d0
		subi.w		#' ',d0		; Get font table symbol index
		mulu.w		#FONT_HEIGHT,d0		; Get font table byte address (x8 for 8px height)

		lea		font,a0		; a0 = font base pointer
		adda.w		d0,a0		; Offset by # of bytes

		; Push parameters for bitmap subroutine
		move.w		#FONT_HEIGHT,-(sp)	; Set height
		move.l		a0,-(sp)		; Pass bitmap char pointer
		move.w		pn_col(a6),-(sp)	; Pass col
		move.w		pn_row(a6),-(sp)	; Pass row
		move.l		pn_base(a6),-(sp)	; Pass framebuffer base pointer
		jsr		_plot_n8bit_bitmap	; Plot negative bitmap
pn_end:
		adda.w		#14,sp		; Super pop
		movem.l		(sp)+,d0/a0
		unlk		a6
		rts

; ==== void plot_nstring(UINT8 *base, int row, int col, char *ch) ====
; PURPOSE: Plots a string, as a sequence of bitmaps from a font table, to the screen.
; INPUT: Address(UINT8*): to the start of the screen
;	 Position(row,col): the coordinates of the top left pixel of the string
;	 ch(c-string): the string to be written to the screen
; OUTPUT: None
		xdef		_plot_nstring

pns_base	equ		8
pns_row		equ		12
pns_col		equ		14
pns_char	equ		16
_plot_nstring:
		link		a6,#0
		movem.l		a5,-(sp)

		move.l		pns_char(a6),a5	; a5.l = String char pointer

		suba.w		#10,sp

		move.l		pns_base(a6),(sp)	; 0: base	0(sp)
		move.w		pns_row(a6),4(sp)	; 1: row	4(sp)
		move.w		pns_col(a6),6(sp)	; 2: col	6(sp)
		move.b		#0,8(sp)	; Clear top byte of param 3 for char
pns_loop:
		move.b		(a5)+,9(sp)	; 3: char	9(sp)
		beq		pns_end		; Stop printing on null terminator
		; More to print
		jsr		_plot_ncharacter

		addq.w		#8,6(sp)
		cmpi.w		#COLS-8,6(sp)
		ble.b		pns_loop
		; Wrap to next line
		move.w		#0,6(sp)
		add.w		#FONT_HEIGHT,4(sp)
		bra		pns_loop

pns_end:
		adda.w		#10,sp
		movem.l		(sp)+,a5
		unlk		a6
		rts

; ==== void plot_20l_bitmap(UINT32 *base, int row, UINT32 *bitmap, UINT16 height) ====
; PURPOSE: Plots a full width bitmap across the screen from the given row
; INPUT: Address(UINT32 *): to the start of the screen
;	 Row: the row of the top left pixel of the bitmap
;	 Bitmap(UINT32 *): pointer to the start of the bitmap 
;	 Height: the height of the bitmap
; OUTPUT: None
		xdef		_plot_20l_bitmap

bml20_base	equ		32
bml20_row	equ		36
bml20_bmp	equ		38
bml20_hei	equ		42
_plot_20l_bitmap:
		movem.l		d0-4/a0-1,-(sp)

		; Height must be > 0
		move.w		bml20_hei(sp),d0	; d0.w = height
		beq.w		bml20_end	; if (height == 0) return

		; Load bitmap pointer
		movea.l		bml20_bmp(sp),a1	; a1.l = Bitmap base pointer

		; Load row
		move.w		bml20_row(sp),d1	; d1.w = Row #
		bpl.b		bml20_bottom	; Row > 0, no top clipping.

		; Top clipping
		neg.w		d1
		sub.w		d1,d0		; Subtract negative row count from height
		bls.w		bml20_end	; If height < negative row count, nothing to draw

		; Top clipped but bitmap partially appears
		ext.l		d1
		lsl.l		#2,d1		; Row * 4 = Bitmap row offset
		adda.l		d1,a1		; Offset bitmap pointer by # of negative rows
		clr.w		d1		; Zero start row

bml20_bottom:
		cmpi.w		#ROWS,d1
		bhs.w		bml20_end	; If row >= max row, nothing to draw

		; Bottom clip
		move.w		#ROWS,d2
		sub.w		d1,d2		; d2.w = Max visible height (ROWS - row)
		cmp.w		d2,d0
		blo.b		bml20_clipped_row	; If height < visible height, no clipping

		; Bottom clipped but bitmap partially appears
		move.w		d2,d0		; Clamp height to visible height

bml20_clipped_row:	; Clipped row + height found
		mulu.w		#20,d0		; Multiply by 20 for long count
		subq.w		#1,d0		; Predecrement long count for print loop


		; Move fb pointer to first byte of row
		movea.l		bml20_base(sp),a0	; a0.l = Framebuffer base pointer
		move.w		d1,d2
		lsl.w		#4,d1
		lsl.w		#6,d2
		add.w		d2,d1		; d1.w = Row byte offset
		adda.w		d1,a0		; a0 points to the first row to draw on
		
bml20_aligned_loop:
		move.l		(a1)+,d3	; d3.l = Bitmap row
		or.l		d3,(a0)+	; Apply bitmap row mask to framebuffer
		dbra		d0,bml20_aligned_loop

		bra		bml20_end

bml20_end:
		movem.l		(sp)+,d0-4/a0-1
		rts



ones:		dcb.l		13,-1