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
		not.w		d1		; Invert (Nevative printing)
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
		lsl.w		d1,d2		; Shift bitmap row
		not.w		d2		; Invert for negative drawing
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
		not.w		d1		; Invert for negative drawing
		and.w		d1,(a0)		; Apply bitmap row mask to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next line of the framebuffer
		dbra		d0,nbm16_aligned_loop

		bra		nbm16_end

nbm16_shifted:
		cmpi.w		#BYTE_COLS-2,d1
		bhs.b		nbm16_right_loop	; Clipping with no 2nd word

nbm16_shift_loop:	; Clipping with 2nd word
		; Speed optimized shift with swap.w and longs
		moveq.l		#0,d1		; Clear long to shift word in
		move.w		(a1)+,d1	; d1.w = Bitmap row
		ror.l		d2,d1		; d1.l = Shifted bitmap
		not.l		d1		; Invert for negative drawing
		and.w		d1,(a0)+	; Apply start word bitmap row to framebuffer
		swap.w		d1		; d1.w = End word bitmap row
		and.w		d1,(a0)		; Apply end word bitmap row

		adda.w		#BYTE_COLS-2,a0		; Move to next row

		dbra		d0,nbm16_shift_loop
		bra.b		nbm16_end

nbm16_right_loop:
		move.w		(a1)+,d1	; Copy word mask
		lsr.w		d2,d1		; Shift right to get start mask
		not.w		d1		; Invert for negative drawing
		and.w		d1,(a0)		; Apply start word bitmap row to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next row
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
		not.l		d3
		and.l		d3,(a0)		; Apply bitmap row mask to framebuffer
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
		not.l		d3		; Invert for negative printing
		not.l		d4		; Invert for negative printing

		and.l		d3,(a0)+	; Apply bitmap row masks to framebuffer
		and.l		d4,(a0)

		adda.w		#BYTE_COLS-4,a0	; Move to next row

		dbra		d0,nbm32_shift_loop
		bra.b		nbm32_end

nbm32_right_loop:
		move.l		(a1)+,d3	; d3.l = Bitmap row	
		lsr.l		d2,d3		; Shift start long
		not.l		d3		; Invert for negative printing
		and.l		d3,(a0)		; Apply to framebuffer
		adda.w		#BYTE_COLS,a0	; Move to next row
		dbra		d0,nbm32_right_loop

nbm32_end:
		movem.l		(sp)+,d0-4/a0-1
		rts

; ==== void plot_20l_bitmap(UINT32 *base, int row, UINT32 *bitmap, UINT16 height) ====
; PURPOSE: Plots a full width bitmap across the screen from the given row
; INPUT: Address(UINT32 *): to the start of the screen
;	 Row: the row of the top left pixel of the bitmap
;	 Bitmap(UINT32 *): pointer to the start of the bitmap 
;	 Height: the height of the bitmap
; OUTPUT: None
		xdef		_plot_20l_bitmap

bml20_base	equ		56
bml20_row	equ		60
bml20_bmp	equ		62
bml20_hei	equ		66
_plot_20l_bitmap:
		movem.l		d0-7/a0-4,-(sp)

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
		move.l		d1,d2
		lsl.l		#6,d1
		lsl.l		#4,d2
		add.l		d2,d1		; Bitmap row offset
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
		; Move fb pointer to first byte of row
		movea.l		bml20_base(sp),a0	; a0.l = Framebuffer base pointer
		add.w		d0,d1
		move.w		d1,d2
		lsl.w		#4,d1
		lsl.w		#6,d2
		add.w		d2,d1		; d1.w = Final row byte offset
		adda.w		d1,a0		; a0 points to the first row to draw on
		
		subq.w		#1,d0		; Predecrement loop count
bml20_loop:	; Print entire rows. Longs are printed in reverse order.
		movem.l		(a1)+,d1-7/a2-4
		movem.l		d1-7/a2-4,-(a0)	; Transfer 10 registers
		movem.l		(a1)+,d1-7/a2-4
		movem.l		d1-7/a2-4,-(a0)	; Transfer 10 more registers
		dbra		d0,bml20_loop

		bra		bml20_end

bml20_end:
		movem.l		(sp)+,d0-7/a0-4
		rts


		even
ones:		dcb.l		13,-1
zeros:		ds.l		13