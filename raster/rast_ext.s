;	Peter Ursem
;	purse327@mtroyal.ca

;	Logan Campbell
;	lcamp702@mtroyal.ca

;	COMP2659-001
;	Steve Kalmar

;	In the Pocket - Checkpoint 3
;	Due: Nov 13, 2026
;	raster/rast_ext.s    

;	Status:
;	  Not Tested:
;
;	  Visual Tests:
;		plot_48px_vline

;	Purpose:
;	Extra more specific functions for our game, In the Pocket on the Atari ST.

;       Screen details
ROWS		equ		400
COLS		equ		640

; ==== void plot_48px_vline(UINT8* base, int row, int col) ====
; PURPOSE: Plots a 48px tall single pixel line on the screen.
; INPUT: Address(UINT8*): to the start of the screen
;       Position(row,col): the location of the pixel to plot
; OUTPUT: None
; ASSUMES NO CLIPPED LINES WILL BE DRAWN. THEY ARE REJECTED

		xdef		_plot_48px_vline

pl_base		equ		20
pl_row		equ		24
pl_col		equ		26
_plot_48px_vline:
		movem.l		d0-2/a0,-(sp)

		move.w		pl_row(sp),d0	; d0.w = Row #
		cmp.w		#ROWS-48,d0
		bhs.b		pl_end		; Fast row bound check [1]

		move.w		pl_col(sp),d1	; d1.w = Column #
		cmp.w		#COLS,d1
		bhs.b		pl_end		; Fast col bound check [1]

		; Multiplication broken into powers of two
		move.w		d0,d2		; d2.w = Row #
		lsl.w		#4,d0		; d0.w = row * 16
		lsl.w		#6,d2		; d2.w = row * 64
		add.w		d2,d0		; d0.w = Row byte offset = row * 80
		
		move.w		d1,d2		; d2.w = Column #
		not.w		d2		; d2 (lowest 3 bits) = Column bit shift (7 - (col % 8))
		and.w		#7,d2
		
		lsr.w		#3,d1		; d1.w = Col byte offset (col / 8)
		add.w		d1,d0		; d0.w = Total byte offset

		movea.l		pl_base(sp),a0	; a0 points to framebuffer base
        	adda.w		d0,a0		; a0 points to first row

		moveq.l		#0,d1
		bset.l		d2,d1		; Set specific bit in long
		moveq.l		#3,d0
pl_loop:
		or.b		d1,(a0)		; Unrolled 12 times
		or.b		d1,80(a0)
		or.b		d1,160(a0)
		or.b		d1,240(a0)
		or.b		d1,320(a0)
		or.b		d1,400(a0)
		or.b		d1,480(a0)
		or.b		d1,560(a0)
		or.b		d1,640(a0)
		or.b		d1,720(a0)
		or.b		d1,800(a0)
		or.b		d1,880(a0)
		adda.w		#960,a0
		dbra		d0,pl_loop

pl_end:
		movem.l		(sp)+,d0-2/a0
		rts