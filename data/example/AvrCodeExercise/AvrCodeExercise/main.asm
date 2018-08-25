;
; AvrCodeExercise.asm
;
; Created: 25/8/2018 5:59:28 PM
; Author : frask
;


; Replace with your application code
start:
    ldi		r16, $70
	ser		r17
	ldi		r24, $30
	clr		r25
	sub		r17, r24
	add		r16, r17
	brvs	overflow1
back1:
	ldi		r17, $65
	adc		r16, r17		
	brvs	overflow2
back2:
	adiw	r24:r25, $35
	sbc		r16, r24
	brvc	noUnderflow1	
back3:
	ldi		r18, 0b00100100
	sbrs	r18, 5
	jmp		forever
	sbrc	r18, 4 
	rjmp	next
	jmp		there
next:
	ser		r19
	mov		r0, r19
	dec		r0
	mov		r1, r19
	call	justCalling
	ldi		r21, $f0
	bst		r21, 6
	brts	tFlagSet
back4:
	brtc	forever
	ses
	brlt	lessThan
back5:
	brge	forever

	; End of program
	break
forever:
	rjmp	forever

justCalling:
	cp		r0, r1
	brbs	SREG_Z, getBack	
	rcall	makeR0andR1TheSame
	jmp		justCalling
getBack:
	ret

overflow1:
	rjmp	back1	
overflow2:
	rjmp	back2	
noUnderflow1:
	rjmp	back3
tFlagSet:
	jmp		back4
lessThan:
	rjmp	back5
there:
	rjmp	next

makeR0andR1TheSame:
	ser		r20
	andi	r20, $1
	add		r0, r20
	ret