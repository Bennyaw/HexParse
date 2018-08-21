#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "unity.h"
#include "Exception.h"
#include "CException.h"
#include "HexParse.h"
#include "HexParseAndSimulator.h"
#include "Simulator.h"
#include "ErrorSimulator.h"

#define k 1024
#define SRAM_SIZE   0x900

uint8_t flashMemory[256*k];
extern uint8_t sram[SRAM_SIZE];

void setUp(void){
  initHexParser();
}

void tearDown(void)
{
}

/*-----oooooo.hex---------------
 *:020000020000FC
 *:0800000002E014E0010F9895E5
 *:00000001FF
 *-----------------------------*/

 /**--------simulate -----------
  *                opcode
  * ldi r16 $2     E0 02
  * ldi r17 $4     E0 14
  * add r16,r17    0F 01
  * break          95 98
  */
void test_HexParseAndSimulator_add_r16_r17_(void){


  CEXCEPTION_T e;
  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/oooooo.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }

  while((hexLineRead = readFile(fp)) != NULL)
  {
    hexParse(hexLineRead,flashMemory);
  }

  r[16] = 2;
  r[17] = 4;

 Try{
//   simulate(flashMemory);
   TEST_FAIL_MESSAGE("Expect Break exception. But no exception thrown.");
 }
 Catch(e){
  TEST_ASSERT_EQUAL(BREAK_EXCEPTION, e->errorCode);
  TEST_ASSERT_EQUAL(6,r[16]);
  TEST_ASSERT_EQUAL(4,r[17]);

  TEST_ASSERT_EQUAL(0, sreg->C);
  TEST_ASSERT_EQUAL(0, sreg->Z);
  TEST_ASSERT_EQUAL(0, sreg->N);
  TEST_ASSERT_EQUAL(0, sreg->V);
  TEST_ASSERT_EQUAL(0, sreg->S);
  TEST_ASSERT_EQUAL(0, sreg->H);
}

}

/**----------buublesort------------
.include "8515def.inc"

.equ	SIZE	=60		;data block size
.equ	TABLE_L	=$60		;Low SRAM address of first data element
.equ	TABLE_H	=$00		;High SRAM address of first data element

rjmp	RESET		;Reset Handle

;***************************************************************************
;*
;* "bubble"
;*
;* This subroutine bubble sorts the number of bytes found in "cnt1" + 1
;* with the last element in SRAM at location "last".
;* This implementation sorts the data with the highest element at the
;* lowest SRAM address. The sort order can be reversed by changing the
;* "brlo" statement to "brsh". Signed sort can be obtained by using "brlt"
;* or "brge"
;*
;* Number of words	:13 + return
;* Number of cycles	:6*(SIZE-1)+10*(SIZE(SIZE-1))+return (Min)
;*			 6*(SIZE-1)+13*(SIZE(SIZE-1))+return (Max)
;* Low registers used	:3 (A,B,cnt2)
;* High registers used	:3 (cnt1,endL,endH)
;* Pointers used	:Z
;*
;***************************************************************************

;***** Subroutine Register Variables

.def	A	=r13		;first value to be compared
.def	B	=r14		;second value to be compared
.def	cnt2	=r15		;inner loop counter
.def	cnt1	=r16		;outer loop counter
.def	endL	=r17		;end of data array low address
.def	endH	=r18		;end of data array high address

;***** Code

bubble:
mov	ZL,endL
mov	ZH,endH		;init Z pointer
mov	cnt2,cnt1	;counter2 <- counter1
i_loop:	ld	A,Z		;get first byte, A (n)
ld	B,-Z		;decrement Z and get second byte, B (n-1)
cp	A,B		;compare A with B
brlo	L1		;if A not lower
st	Z,A		;    store swapped
std	Z+1,B
L1:	dec	cnt2
brne	i_loop		;end inner loop
dec	cnt1
brne	bubble		;end outer loop
ret


;***************************************************************************
;*
;* Test Program
;*
;* This program copies 60 bytes of data from Program memory to SRAM. It
;* then calls "bubble" to get the data sorted.
;*
;***************************************************************************

RESET:

;***** Main program Register variables

.def	temp	=r16

;***** Code

ldi	temp,low(RAMEND)
out	SPL,temp
ldi	temp,high(RAMEND)
out	SPH,temp	;init Stack Pointer

;***** Memory fill

clr	ZH
ldi	ZL,tableend*2+1	;Z-pointer <- ROM table end + 1
ldi	YL,low(256*TABLE_H+TABLE_L+SIZE)
ldi	YH,high(256*TABLE_H+TABLE_L+SIZE)
      ;Y pointer <- SRAM table end + 1
loop:	lpm			;get ROM constant
st	-Y,r0		;store in SRAM and decrement Y-pointer
sbiw	ZL,1		;decrement Z-pointer
cpi	YL,TABLE_L	;if not done
brne	loop		;    loop more
cpi	YH,TABLE_H
brne	loop

;***** Sort data

sort:	ldi	endL,low(TABLE_H*256+TABLE_L+SIZE-1)
ldi	endH,high(TABLE_H*256+TABLE_L+SIZE-1)
      ;Z <- end of array address
ldi	cnt1,SIZE-1	;cnt1 <- size of array - 1
rcall	bubble

forever:rjmp	forever



;***** 60 ROM Constants

table:
.db	120,196
.db	78,216
.db	78,100
.db	43,39
.db	241,43
.db	62,172
.db	109,69
.db	48,184
.db	215,231
.db	63,133
.db	216,8
.db	121,126
.db	188,98
.db	168,205
.db	157,172
.db	108,233
.db	80,255
.db	252,102
.db	198,0
.db	171,239
.db	107,114
.db	172,170
.db	17,45
.db	42,55
.db	34,174
.db	229,250
.db	12,179
.db	187,243
.db	44,231
tableend:
.db	76,48

 */


/*---------testBubbleSort.hex--------------
:020000020000FC
:100000000EC0E12FF22FF02ED080E290DE1410F01F
:10001000D082E182FA94C1F70A9599F708950FEF1B
:100020000DBF08E00EBFFF27E1E8CCE9D0E0C8959E
:100030000A923197C036D9F7D030C9F71BE920E0D2
:100040000BE3DFDF989578C44ED84E642B27F12B55
:100050003EAC6D4530B8D7E73F85D808797EBC62A5
:10006000A8CD9DAC6CE950FFFC66C600ABEF6B728F
:10007000ACAA112D2A3722AEE5FA0CB3BBF32CE75C
:020080004C3002
:00000001FF
*/
void test_HexParseAndSimulator_bubblesort(void)
{
  CEXCEPTION_T e;
  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/testBubbleSort.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }

  while((hexLineRead = readFile(fp)) != NULL)
  {
    hexParse(hexLineRead,flashMemory);
  }
  uint8_t expectedData[] ={
    255,252,250,243,241,239,233,231,231,229,
    216,216,215,205,198,196,188,187,184,179,
    174,172,172,172,171,170,168,157,133,126,
    121,120,114,109,108,107,102,100,98,80,
    78,78,76,69,63,62,55,48,48,45,
    44,43,43,42,39,34,17,12,8,0
  };

 Try{
   simulate(flashMemory);
   TEST_FAIL_MESSAGE("Expect Break exception. But no exception thrown.");
 }
 Catch(e){
  TEST_ASSERT_EQUAL(BREAK_EXCEPTION, e->errorCode);
  TEST_ASSERT_EQUAL_MEMORY(expectedData,&sram[0x0000],60);
}
}
