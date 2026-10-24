
; =============== SCORE METER ===============
; A saxophone in the top right that is filled
; according to the enjoyment meter level.

		xdef		_emp_left
_emp_left:	incbin		"meter\emp_left.bin"

		xdef		_emp_right
_emp_right:	incbin		"meter\emp_rght.bin"

		xdef		_full_left
_full_left:	incbin		"meter\ful_left.bin"

		xdef		_full_right
_full_right:	incbin		"meter\ful_rght.bin"

; ===============  BACKGROUNDS  ===============
; 640px wide bitmaps to plot in their entirety.
		xdef		_bg
_bg:		incbin		"bg\bg.bin"

; ===============  TARGETS  ===============
