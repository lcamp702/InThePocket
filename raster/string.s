; ==== void plot_xcharacter(UINT8 *base, int row, int col, char ch, UINT8 size) ====
; ==== void plot_xncharacter(UINT8 *base, int row, int col, char ch, UINT8 size) ====
; PURPOSE: Plots a single character, as a bitmap from a font table, to the screen.
; INPUT: Address(UINT32*): to the start of the screen
;		Position(row,col): the coordinates of the top left pixel of the character
;		ch(char): the character to be written to the screen
;		size(UINT8): 0-2 font size (0: 8px, 1: 16px, 2: 32px)
; OUTPUT: None
		xdef		_plot_xcharacter
		xdef		_plot_xncharacter

		xref		_plot_8bit_bitmap
		xref		_plot_16bit_bitmap
		xref		_plot_32bit_bitmap
		xref		_plot_n8bit_bitmap
		xref		_plot_n16bit_bitmap
		xref		_plot_n32bit_bitmap

		xref		pocket8
		xref		pocket16
;		xref		pocket32

pcx_base	equ		8
pcx_row		equ		12
pcx_col		equ		14
pcx_char	equ		17
pcx_size	equ		19

_plot_xcharacter:
		link		a6,#0
		movem.l		d0-2/a2-3,-(sp)
		lea		positive_plots(pc),a2	; Load address of positive plot function jump table
		bra.b		pcx_common

_plot_xncharacter:
		link		a6,#0
		movem.l		d0-2/a2-3,-(sp)
		lea		negative_plots(pc),a2	; Load address of negative plot function jump table

; This code is reused for positive and negative prints
pcx_common:
		; IS_PRINTABLE
		move.b		pcx_char(a6),d0	; Get char #
		cmpi.b		#' ',d0		; Compare to space (ASCII 32) for printable check
		blt.b		pcx_end		; Below printable range, exit
		cmpi.b		#'~',d0		; Compare to tilde (ASCII 126) for printable check
		bgt.b		pcx_end		; Above printable range, exit

		; GLYPH_START
		ext.w		d0
		subi.w		#' ',d0		; Font table glyph start index

		; Get font size
		move.b		pcx_size(a6),d2
		cmpi.b		#1,d2		; Check size (0,1,2) -> (8, 16, 32)
		blt.b		pcx_sz8		; < 1 (8 Bit)
		beq.b		pcx_sz16	; = 1 (16 bit)
pcx_sz32:
		lsl.w		#7,d0		; 128 bytes per char (index << 7)
;		lea		pocket32,a0	; a0 = font bitmap base address
		moveq		#32,d1		; d1.w = font height
		movea.l		8(a2),a3	; Load plot function address into memory
		bra.b		pcx_plot
pcx_sz16:
		lsl.w		#5,d0		; 32 bytes per char (index << 5)
		lea		pocket16,a0	; a0 = font bitmap base address
		moveq		#16,d1		; d1.w = font height
		movea.l		4(a2),a3	; Load plot function address into memory
		bra.b		pcx_plot
pcx_sz8:
		lsl.w		#3,d0		; 8 bytes per char (index << 3)
		lea		pocket8,a0	; a0 = font bitmap base address
		moveq		#8,d1		; d1.w = font height
		movea.l		(a2),a3		; Load plot function address into memory
pcx_plot:
		adda.w		d0,a0		; a0 = font bitmap pointer

		; Push parameters for bitmap subroutine
		move.w		d1,-(sp)	; Height
		move.l		a0,-(sp)	; Bitmap char pointer
		move.w		pcx_col(a6),-(sp)	; Col
		move.w		pcx_row(a6),-(sp)	; Row
		move.l		pcx_base(a6),-(sp)	; Framebuffer base pointer

		jsr		(a3)		; Plot bitmap
		adda.w		#14,sp		; Super pop

pcx_end:
		movem.l		(sp)+,d0-2/a2-3
		unlk		a6
		rts

		; --- Plot jump tables ---
positive_plots:
		dc.l		_plot_8bit_bitmap
		dc.l		_plot_16bit_bitmap
		dc.l		_plot_32bit_bitmap

negative_plots:
		dc.l		_plot_n8bit_bitmap
		dc.l		_plot_n16bit_bitmap
		dc.l		_plot_n32bit_bitmap

; ==== void plot_xstring(UINT8 *base, int row, int col, char *ch, UINT8 size) ====
; ==== void plot_nxstring(UINT8 *base, int row, int col, char *str, UINT8 size) ====
; PURPOSE: Plots a string, as a sequence of bitmaps from a font table, to the screen.
; INPUT: Address(UINT8*): to the start of the screen
;	 Position(row,col): the coordinates of the top left pixel of the string
;	 ch(c-string): the string to be written to the screen
;	size(UINT8): 0-2 font size (0: 8px, 1: 16px, 2: 32px)
; OUTPUT: None
		xdef		_plot_xstring
		xdef		_plot_nxstring

COLS		equ		640

pstx_base	equ		8		; UINT8* (4 bytes)
pstx_row	equ		12		; int(2 bytes)
pstx_col	equ		14		; int(2 bytes)
pstx_char	equ		16		; char*(4 bytes)
pstx_size	equ		21		; Low byte of 16-bit size parameter

_plot_xstring:
		link		a6,#0
		movem.l		d2-3/a2-3,-(sp)
		lea		_plot_xcharacter,a2	; Use normal char plotter
		bra.b		pstx_common

_plot_nxstring:
		link		a6,#0
		movem.l		d2-3/a2-3,-(sp)
		lea		_plot_xncharacter,a2	; Use negative char plotter

pstx_common:
		move.l		pstx_char(a6),a3	; a3.l = String char pointer

		clr.w		d2
		move.b		pstx_size(a6),d2	; Get size code
		addq.w		#3,d2
		moveq.l		#1,d3
		lsl.w		d2,d3		; d3.w = 8, 16, or 32 (1 << (3 + size))
		move.w		d3,d2		; d2.w = font width & height step

		move.w		#COLS,d3
		sub.w		d2,d3		; d3.w = Font wrap column

		suba.w		#12,sp

		move.l		pstx_base(a6),(sp)	; 0: base	0(sp)
		move.w		pstx_row(a6),4(sp)	; 1: row	4(sp)
		move.w		pstx_col(a6),6(sp)	; 2: col	6(sp)
		clr.b		8(sp)		; Clear top byte for char
		clr.b		10(sp)		; Clear top byte for size
		move.b		pstx_size(a6),11(sp)	; 4: size	11(sp)

pstx_loop:
		move.b		(a3)+,9(sp)	; 3: char	9(sp)
		beq.b		pstx_end	; Stop printing on null terminator
		; More to print
		jsr		(a2)		; Call char plotting routine

		; Advance by font width
		add.w		d2,6(sp)
		cmp.w		6(sp),d3	; col <= (COLS - font_width)
		bge.b		pstx_loop
		
		; Wrap to next line
		move.w		#0,6(sp)
		add.w		d2,4(sp)
		bra.b		pstx_loop

pstx_end:
		adda.w		#12,sp		; Super pop
		movem.l		(sp)+,d2-3/a2-3
		unlk		a6
		rts