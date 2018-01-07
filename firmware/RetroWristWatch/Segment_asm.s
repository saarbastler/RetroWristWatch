
/*
 * Segment_asm.s
 *
 * Created: 04.08.2016 18:43:21
 *  Author: Joerg
 */ 

 #include <avr/interrupt.h>

 #include "Device.h"

.extern Timer1_tickerL
.extern Timer1_tickerH

.extern Segment_digit
.extern Segment_digitIdx
.extern Segment_digitFlags

.comm divider, 1

.global TIMER1_OVF_vect

.func TIMER1_OVF_vect
TIMER1_OVF_vect:
				push 	r16
				in 		r16, _SFR_IO_ADDR(SREG)
				push	r16

				lds		r16, Timer1_ticker
				inc		r16
				sts		Timer1_ticker, r16
        brne  noTick
      
				lds		r16, Timer1_ticker+1
				inc		r16
				sts		Timer1_ticker+1, r16

noTick: lds   r16, Segment_digitFlags
        tst   r16
        breq  jmpNoDisplay
        lds   r16, divider
        inc   r16
        andi  r16, 1
        sts   divider, r16
        breq  display
jmpNoDisplay:
        jmp  noDisplay

display:
        ; Digits off
        cbi   _SFR_IO_ADDR(DIGIT1_PORT), DIGIT1
        cbi   _SFR_IO_ADDR(DIGIT2_PORT), DIGIT2
        cbi   _SFR_IO_ADDR(DIGIT3_PORT), DIGIT3
        cbi   _SFR_IO_ADDR(DIGIT4_PORT), DIGIT4

        push  r30
        push  r31

        ldi   ZL, lo8(Segment_digit)
        ldi   ZH, hi8(Segment_digit)
        lds   r16, Segment_digitIdx
        inc   r16
        andi  r16, 3
        sts   Segment_digitIdx, r16
        add   ZL, r16
        ldi   r16, 0
        adc   ZH, r16
        ld    r16, Z

        sbrs  r16, 0
        cbi    _SFR_IO_ADDR(SEGMENT_A_PORT), SEGMENT_A
        sbrc  r16, 0
        sbi    _SFR_IO_ADDR(SEGMENT_A_PORT), SEGMENT_A
        sbrs  r16, 1
        cbi    _SFR_IO_ADDR(SEGMENT_B_PORT), SEGMENT_B
        sbrc  r16, 1
        sbi    _SFR_IO_ADDR(SEGMENT_B_PORT), SEGMENT_B
        sbrs  r16, 2
        cbi    _SFR_IO_ADDR(SEGMENT_C_PORT), SEGMENT_C
        sbrc  r16, 2
        sbi    _SFR_IO_ADDR(SEGMENT_C_PORT), SEGMENT_C
        sbrs  r16, 3
        cbi    _SFR_IO_ADDR(SEGMENT_D_PORT), SEGMENT_D
        sbrc  r16, 3
        sbi    _SFR_IO_ADDR(SEGMENT_D_PORT), SEGMENT_D
        sbrs  r16, 4
        cbi    _SFR_IO_ADDR(SEGMENT_E_PORT), SEGMENT_E
        sbrc  r16, 4
        sbi    _SFR_IO_ADDR(SEGMENT_E_PORT), SEGMENT_E
        sbrs  r16, 5
        cbi    _SFR_IO_ADDR(SEGMENT_F_PORT), SEGMENT_F
        sbrc  r16, 5
        sbi    _SFR_IO_ADDR(SEGMENT_F_PORT), SEGMENT_F
        sbrs  r16, 6
        cbi    _SFR_IO_ADDR(SEGMENT_G_PORT), SEGMENT_G
        sbrc  r16, 6
        sbi    _SFR_IO_ADDR(SEGMENT_G_PORT), SEGMENT_G
        sbrs  r16, 7
        cbi    _SFR_IO_ADDR(SEGMENT_DP_PORT), SEGMENT_DP
        sbrc  r16, 7
        sbi    _SFR_IO_ADDR(SEGMENT_DP_PORT), SEGMENT_DP

        lds   r16, Segment_digitIdx
        and   r16,r16
        brne  tDigit2
        sbi   _SFR_IO_ADDR(DIGIT1_PORT), DIGIT1
        rjmp  digitDone
tDigit2:
        dec   r16
        brne  tDigit3
        sbi   _SFR_IO_ADDR(DIGIT2_PORT), DIGIT2
        rjmp  digitDone
tDigit3:
        dec   r16
        brne  tDigit4
        sbi   _SFR_IO_ADDR(DIGIT3_PORT), DIGIT3
        rjmp  digitDone
tDigit4:
        sbi   _SFR_IO_ADDR(DIGIT4_PORT), DIGIT4
digitDone:
        pop   r31
        pop   r30

noDisplay:
				pop		r16
				out 	_SFR_IO_ADDR(SREG), r16
				pop 	r16
				reti
.endfunc
.end
