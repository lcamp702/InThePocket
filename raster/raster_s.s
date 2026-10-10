;	Peter Ursem
;	purse327@mtroyal.ca
;	COMP2659-001
;	Steve Kalmar
;	In the Pocket - Checkpoint 2
;	Due: Oct 19, 2026
;	raster_s.s    

;	Status:
;	  Not Tested:
;	  Tested:

;	Purpose:
;	A raster engine for our game, In the Pocket on the Atari ST.

		xdef		_clear_screen
		xdef		_plot_pixel

;       Screen details
ROWS		equ		400

COLS		equ		640
BYTE_COLS	equ		80

;	_clear_screen(UINT32 *base)
cs_base		equ		64	; Offset from SP, not A6

;	_plot_pixel(UINT8 *base, int row, int col)
pp_base		equ		20	; 4 + bytes in saved registers
pp_row		equ		24	; 8 + bytes in saved registers
pp_col		equ		26	; 10 + bytes in saved registers

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

		movem.l		d1-5,-(a0)	; Clear remaining 5 longs

		movem.l		(sp)+,d0-7/a0-6 ; Return existing register values
		rts

_plot_pixel:
		movem.l		d0-2/a0,-(sp)

		move.w		pp_row(sp),d0	
		mulu.w		#BYTE_COLS,d0	; d0.w = Row byte offset
		
		move.w		pp_col(sp),d1	; d1.w = Column #
		move.w		d1,d2		; d2.w = Column #
		
		lsr.w		#3,d1		; d1.w = Byte offset (col / 8)
		add.w		d1,d0		; d0.w = Total byte offset
		not.w		d2		; d2 (lowest 3 bits) = Column bit remainder (col % 8)
						; Works because bset source is mod 8. (https://68k.hax.com/BSET)

		movea.l		pp_base(sp),a0	; a0 points to framebuffer base
		bset.b		d2,0(a0,d0.w)	; Set specific bit in memory

		movem.l		(sp)+,d0-2/a0
		rts

zeros:		ds.l		13