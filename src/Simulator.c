#include <stdio.h>
#include <stdint.h>
#include "Simulator.h"
#include "ErrorSimulator.h"
#include "Exception.h"
#include "CException.h"

AvrOperator avrOperatorTable[256] = {
  [0x0c ... 0x0f] = add,
  [0x1c ... 0x1f] = adc,
  [0x20 ... 0x23] = and,
  [0x70 ... 0x7f] = andi,
  [0x96] = adiw,
  [0x18 ... 0x1b] = sub,
  [0x50 ... 0x5f] = subi,
  [0x08 ... 0x0b] = sbc,
  [0x40 ... 0x4f] = sbci,
  [0x97] = sbiw,
  [0x28] = or,
  [0x60 ... 0x6f] = ori,
  [0x24 ... 0x27] = eor,
  [0xef] = ser,
  [0x9c ... 0x9f] = mul,
  [0x02] = muls,
  [0x94 ... 0x95] = instructionWith1001010,
  [0xfa ... 0xfb] = bst,
  [0xf8 ... 0xf9] = bld,
  [0x03] = mulsu,
  [0x2c ... 0x2f] = mov,
  [0xc0 ... 0xcf] = rjmp,
  [0xf0 ... 0xf3] = brbs,
  [0xf4 ... 0xf7] = brbc,
  [0x00] = nop,
  [0x14 ... 0x17] = cp,
  [0x04 ... 0x07] = cpc,
  [0x30 ... 0x3f] = cpi,
  //[0x10 ... 0x13] = cpse,
  [0xe0 ... 0xef] = ldi,
};

AvrOperator avr10010100Table[16] = {
  [0x0] = com,
  [0x1] = neg,
  [0x2] = swap,
  [0x3] = inc,
  [0x5] = asr,
  [0x6] = lsr,
  [0x7] = ror,
  [0x8] = instructionWith10010100,
  [0xa] = dec,
  [0xc] = jmp, jmp,
  //[0xe] = call, call,
};

AvrOperator avr10010101Table[16] = {
  [0x0] = com,
  [0x1] = neg,
  [0x2] = swap,
  [0x3] = inc,
  [0x5] = asr,
  [0x6] = lsr,
  [0x7] = ror,
  [0x8] = instructionWith10010101,
  [0xa] = dec,
  [0xc] = jmp, jmp,
  //[0xe] = call, call,
};

//AVR SRAM
uint8_t sram[SRAM_SIZE];
uint8_t flashMemory[FLASH_SIZE];

//AVR Register
uint8_t *r = sram;
SregRegister *sreg = (SregRegister*)&sram[0x5f];
uint16_t *xRegPtr = (uint16_t *)&sram[0x26];
uint16_t *yRegPtr = (uint16_t *)&sram[0x28];
uint16_t *zRegPtr = (uint16_t *)&sram[0x30];
uint8_t *sph = &sram[0x5e];
uint8_t *spl = &sram[0x5d];
uint8_t *pc = flashMemory;
uint16_t  *spRegPtr = (uint16_t *)&sram[0x5d];
uint8_t *flash = flashMemory;

int simulateOneInstruction(uint8_t *codePtr)
{
	return avrOperatorTable [*(codePtr + 1)](codePtr);
}

int instructionWith1001010(uint8_t *codePtr)
{
	uint8_t low4bit, is0x95;
	low4bit = *codePtr & 0xf;
  is0x95 = (codePtr[1] & 0x1) | (codePtr[0] & 0x0);

  if(is0x95)
    return avr10010101Table [low4bit](codePtr);
  else
    return avr10010100Table [low4bit](codePtr);
}

int instructionWith10010100(uint8_t *codePtr)
{
	uint8_t isbclr;
	isbclr = *codePtr & 0x80;

  if(isbclr)
    bclr(codePtr);
  else
    bset(codePtr);
}

int instructionWith10010101(uint8_t *codePtr)
{
  uint8_t ins;
	ins = *codePtr & 0xf0;

  if(ins == 0xa0)
    wdr(codePtr);
  else if(ins == 0x80)
    sleep(codePtr);
  else
    Break(codePtr);
}

uint32_t getPc(uint8_t *progCounter)
{
	return (progCounter - flash);
}

uint8_t *getCodePtr(uint32_t pc)
{
	return (flash + pc);
}

void substractStackPointer(int value)
{
  *(uint16_t *)spl -= 2;
}

void push(uint16_t data)
{
  *spRegPtr = data;
  substractStackPointer(2);
}

void pop()
{
  substractStackPointer(-2);
  return *spRegPtr;
}

void initSimulator()
{
  *spRegPtr = 0x8ff;
}

/*int is2wordInstruction(uint8_t *codePtr)
{
  uint16_t sts, first4bit;
  codePtr + 2;

  result = *(uint16_t *)codePtr &



  if(sts(codePtr) | jmp(codePtr) | call(codePtr))
    return 1;
  else
    return 0;
}*/

/**
 * Z:
 *		R7 ¯ • R6 ¯ • R5 ¯ • R4 ¯ • R3 ¯ • R2 ¯ • R1 ¯ • R0 ¯
 */
int is8bitZero(uint8_t data8bit)
{
	if(data8bit == 0)
		return 1;
	else
		return 0;
}

/**
 * C:
 *		R15 ¯ • Rdh7
 */
int is16bitADIWCarry(uint16_t result, uint16_t operand)
{
	result = (~result & operand) >> 15;
	return result;
}

/**
 * Z:
 *		R15 ¯ • R14 ¯ • R13 ¯ • R12 ¯ • R11 ¯ • R10 ¯ • R9 ¯ • R8 ¯ • R7 ¯ • R6 ¯ • R5 ¯ • R4 ¯ • R3 ¯ • R2 ¯ • R1 ¯ • R0 ¯
 */
int is16BitZero(uint16_t data16Bit)
{
	if(data16Bit == 0)
		return 1;
	else
		return 0;
}

/**
 * N:
 *		R15
 */
int is16bitNeg(uint16_t result)
{
	result = result >> 15;
	return result;
}

/**
 * V:
 *		R15 • Rdh7 ¯
 */
int is16bitADIWOverflow(uint16_t result, uint16_t operand)
{
	result = (~operand & result) >> 15;
	return result;
}

/**
 * S:
 *		N ^ V
 */
int is16bitSigned(uint16_t result, uint16_t operand)
{
	uint8_t n, v, s;
	n = is16bitNeg(result);
	v = is16bitADIWOverflow(result, operand);
	s = n ^ v;
	return s;
}

int handleStatusRegForAddImmWordOperation(uint16_t result, uint16_t operand)
{
	sreg->C = is16bitADIWCarry(result, operand);
	sreg->Z = is16BitZero(result);
	sreg->N = is16bitNeg(result);
	sreg->V = is16bitADIWOverflow(result, operand);
	sreg->S = is16bitSigned(result, operand);
	return 0;
}

/**
 * C:
 *		R15 • Rdh7 ¯
 */
int is16bitSBIWCarry(uint16_t result, uint16_t operand)
{
	result = (~operand & result) >> 15;
	return result;
}

/**
 * V:
 *		R15 ¯ • Rdh7
 */
int is16bitSBIWOverflow(uint16_t result, uint16_t operand)
{
	result = (~result & operand) >> 15;
	return result;
}

int handleStatusRegForSubImmWordOperation(uint16_t result, uint16_t operand)
{
	sreg->C = is16bitSBIWCarry(result, operand);
	sreg->Z = is16BitZero(result);
	sreg->N = is16bitNeg(result);
	sreg->V = is16bitSBIWOverflow(result, operand);
	sreg->S = is16bitSigned(result, operand);
	return 0;
}

/**
 * C:
 *    Rd7 • Rr7 + Rr7 • R7 ¯ + R7 ¯ • Rd7
 */
int is8bitAdcAddCarry(uint8_t operand1, uint8_t operand2, uint8_t result)
{
  result = (operand1 & operand2 | operand2 & (~result) | (~result) & operand1) >> 7;
  return result;
}

/**
 * N:
 *    R7
 */
int is8bitNeg(uint8_t result)
{
  result = result >> 7;
  return result;
}

/**
 * V:
 *    Rd7 • Rr7 • R7 ¯ + Rd7 ¯ • Rr7 ¯ • R7
 */
int is8bitOverflow(uint8_t operand1, uint8_t operand2, uint8_t result)
{
  result = (operand1 & operand2 & (~result) | (~operand1) & (~operand2) & result) >> 7;
  return result;
}

/**
 * S:
 *		N ^ V
 */
int is8bitSigned(uint8_t operand1, uint8_t operand2, uint8_t result)
{
	uint8_t n, v, s;
	n = is8bitNeg(result);
	v = is8bitOverflow(operand1, operand2, result);
	s = n ^ v;
	return s;
}

/**
 * H:
 *    Rd3 • Rr3 + Rr3 • R3 ¯ + R3 ¯ • Rd3
 */
int is8bitAdcAddHalfCarry(uint8_t operand1, uint8_t operand2, uint8_t result)
{
  result = ((operand1 & operand2 | operand2 & (~result) | (~result) & operand1) & 0x8) >> 3;
  return result;
}

int handleStatusRegForAddAdcOperation(uint8_t operand1, uint8_t operand2, uint8_t result)
{
	sreg->C = is8bitAdcAddCarry(operand1, operand2, result);
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitOverflow(operand1, operand2, result);
	sreg->S = is8bitSigned(operand1, operand2, result);
	sreg->H = is8bitAdcAddHalfCarry(operand1, operand2, result);
	return 0;
}

/**
 * V:
 *		0
 */
int is8bitAndAndiOrOriEorCbrSbrTstOverflow()
{
	uint8_t v;
	sreg->V = 0;
	v = sreg->V;
	return 0;
}

/**
 * S:
 *		N ^ V
 */
int is8bitAndAndiOrOriEorCbrSbrTstSigned(uint8_t result)
{
	uint8_t n, v, s;
	n = is8bitNeg(result);
	v = is8bitAndAndiOrOriEorCbrSbrTstOverflow();
	s = n ^ v;
	return s;
}

int handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(uint8_t result)
{
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitAndAndiOrOriEorCbrSbrTstOverflow();
	sreg->S = is8bitAndAndiOrOriEorCbrSbrTstSigned(result);
}

/**
 * C:
 *		Rd7 ¯ • Rr7 + Rr7 • R7 + R7 • Rd7 ¯
 */
int is8bitSubSubiSbcSbciCarry(uint8_t operand1, uint8_t operand2, uint8_t result)
{
  result = ((~operand1) & operand2 | operand2 & result | result & (~operand1)) >> 7;
  return result;
}

/**
 * H:
 *		Rd3 ¯ • Rr3 + Rr3 • R3 + R3 • Rd3 ¯
 */
int is8bitSubSubiSbcSbciHalfCarry(uint8_t operand1, uint8_t operand2, uint8_t result)
{
  result = ((((~operand1) & operand2) | (operand2 & result) | (result & (~operand1))) & 0x8) >> 3;
  return result;
}

/**
 * V:
 *    Rd7 • Rr7 ¯ • R7 ¯ + Rd7 ¯ • Rr7 • R7
 */
int is8bitSubSubiSbcSbciOverflow(uint8_t operand1, uint8_t operand2, uint8_t result)
{
	result = (operand1 & (~operand2) & (~result) | (~operand1) & operand2 & result) >> 7;
  return result;
}

/**
 * S:
 *    N ^ V
 */
int is8bitSubSubiSbcSbciSigned(uint8_t operand1, uint8_t operand2, uint8_t result)
{
	uint8_t n, v, s;
	n = is8bitNeg(result);
	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	s = n ^ v;
	return s;
}

int handleStatusRegForSubSubiSbcSbciOperation(uint8_t operand1, uint8_t operand2, uint8_t result)
{
	sreg->C = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	sreg->S = is8bitSubSubiSbcSbciSigned(operand1, operand2, result);
	sreg->H = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);
	return 0;
}

/**
 * C:
 *		1
 */
int is8bitComCarry()
{
	uint8_t c;
	sreg->C = 1;
	c = sreg->C;
	return c;
}

int handleStatusRegForComOperation(uint8_t result)
{
	sreg->C = is8bitComCarry();
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitAndAndiOrOriEorCbrSbrTstOverflow();
	sreg->S = is8bitAndAndiOrOriEorCbrSbrTstSigned(result);
}

/**
 * C:
 *		R7 + R6 + R5 + R4 + R3 + R2 + R1 + R0
 */
int is8bitNegCarry(uint8_t data8bit)
{
	if(data8bit == 0x00)
		return 0;
	else
		return 1;
}

/**
 * V:
 *		R7 • R6 ¯ • R5 ¯ • R4 ¯ • R3 ¯ • R2 ¯ • R1 ¯ • R0 ¯
 */
int is8bitNegOverflow(uint8_t data8bit)
{
	if(data8bit == 0x80)
		return 1;
	else
		return 0;
}

/**
 * S:
 *		N ^ V
 */
int is8bitNegSigned(uint8_t result)
{
	uint8_t n, v, s;
	n = is8bitNeg(result);
	v = is8bitNegOverflow(result);
	s = n ^ v;
	return s;
}

/**
 * H:
 *		R3 + Rd3
 */
int is8bitNegHalfCarry(uint8_t operand1, uint8_t result)
{
	uint8_t h;
	h = ((result | operand1) & 0x8) >> 3;
	return h;
}

int handleStatusRegForNegOperation(uint8_t operand1, uint8_t result)
{
	sreg->C = is8bitNegCarry(result);
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitNegOverflow(result);
	sreg->S = is8bitNegSigned(result);
	sreg->H = is8bitNegHalfCarry(operand1, result);
}

int handleStatusRegForIncOperation(uint8_t result)
{
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitNegOverflow(result);
	sreg->S = is8bitNegSigned(result);
}

/**
 * V:
 *		R7 ¯ • R6 • R5 • R4 • R3 • R2 • R1 • R0
 */
int is8bitDecOverflow(uint8_t data8bit)
{
	if(data8bit == 0x7f)
		return 1;
	else
		return 0;
}

int handleStatusRegForDecOperation(uint8_t result)
{
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitDecOverflow(result);
	sreg->S = is8bitNegSigned(result);
}

int handleStatusRegForClrOperation()
{
	sreg->Z = 1;
	sreg->N = 0;
	sreg->V = 0;
	sreg->S = 0;
}

/**
 * C:
 *		R15
 */
int is16bitMulMulsMulsuCarry(uint16_t result)
{
	uint16_t c;

	c = result >> 15;
	return c;
}

int handleStatusRegForMulMulsMulsuOperation(uint16_t result)
{
	sreg->C = is16bitMulMulsMulsuCarry(result);
	sreg->Z = is16BitZero(result);
}

/**
 * C:
 *		Rd7
 */
int is8bitLslRolCarry(uint8_t operand1)
{
	uint8_t c;

	c = operand1 >> 7;
	return c;
}

/**
 * V:
 *		N ^ C
 */
int is8bitLslRolOverflow(uint8_t operand1, uint8_t result)
{
	uint8_t n, c, v;

	n = is8bitNeg(result);
	c = is8bitLslRolCarry(operand1);
	v = n ^ c;
	return v;
}

/**
 * S:
 *		N ^ V
 */
int is8bitLslRolSigned(uint8_t operand1, uint8_t result)
{
	uint8_t n, v, s;

	n = is8bitNeg(result);
	v = is8bitLslRolOverflow(operand1, result);
	s = n ^ v;
	return s;
}

/**
 * H:
 *		Rd3
 */
int is8bitLslRolHalfCarry(uint8_t operand1)
{
	uint8_t h;

	h = (operand1 & 0x8) >> 3;
	return h;
}

int handleStatusRegForLslRolOperation(uint8_t operand1, uint8_t result)
{
	sreg->C = is8bitLslRolCarry(operand1);
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitLslRolOverflow(operand1, result);
	sreg->S = is8bitLslRolSigned(operand1, result);
	sreg->H = is8bitLslRolHalfCarry(operand1);
}

/**
 * C:
 *		Rd0
 */
int is8bitLsrRorCarry(uint8_t operand1)
{
	uint8_t c;

	c = operand1 & 0x1;
	return c;
}

/**
 * V:
 *		N ^ C
 */
int is8bitLsrOverflow(uint8_t operand1)
{
	uint8_t n, c, v;

	n = 0;
	c = is8bitLsrRorCarry(operand1);
	v = n ^ c;
	return v;
}

/**
 * S:
 *		N ^ V
 */
int is8bitLsrSigned(uint8_t operand1)
{
	uint8_t n, v, s;

	n = 0;
	v = is8bitLsrOverflow(operand1);
	s = n ^ v;
	return s;
}

int handleStatusRegForLsrOperation(uint8_t operand1, uint8_t result)
{
	sreg->C = is8bitLsrRorCarry(operand1);
	sreg->Z = is8bitZero(result);
	sreg->N = 0;
	sreg->V = is8bitLsrOverflow(operand1);
	sreg->S = is8bitLsrSigned(operand1);
}

/**
 * V:
 *		N ^ C
 */
int is8bitRorOverflow(uint8_t operand1, uint8_t result)
{
	uint8_t n, c, v;

	n = is8bitNeg(result);
	c = is8bitLsrRorCarry(operand1);
	v = n ^ c;
	return v;
}

/**
 * S:
 *		N ^ V
 */
int is8bitRorSigned(uint8_t operand1, uint8_t result)
{
	uint8_t n, v, s;

	n = is8bitNeg(result);
	v = is8bitRorOverflow(operand1, result);
	s = n ^ v;
	return s;
}

int handleStatusRegForRorAsrOperation(uint8_t operand1, uint8_t result)
{
	sreg->C = is8bitLsrRorCarry(operand1);
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitRorOverflow(operand1, result);
	sreg->S = is8bitRorSigned(operand1, result);
}

int handleStatusRegForRorBstOperation(uint8_t operand1)
{
	if(operand1 == 0)
		return 0;
	else
		return 1;
}

/**
 * Instruction:
 * 		ADD Rd,Rr
 *		0000 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int add(uint8_t *codePtr)
{
  uint8_t rd, rr;

  rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
  rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

  r[rd] = r[rd] + r[rr];
  handleStatusRegForAddAdcOperation(rd, rr, r[rd]);
  return 0;
}

/**
 * Instruction:
 * 		ADC Rd,Rr
 *		0001 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int adc(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	r[rd] = r[rd] + r[rr] + sreg->C;
	handleStatusRegForAddAdcOperation(rd, rr, r[rd]);
  return 0;
}

/**
 * Instruction:
 * 		AND Rd,Rr
 *		0010 00rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int and(uint8_t *codePtr)
{
  uint8_t rd, rr;

  rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
  rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

  r[rd] = r[rd] & r[rr];
  handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
  return 0;
}

/**
 * Instruction:
 * 		ANDI Rd, K
 *		0111 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *    16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 */
int andi(uint8_t *codePtr)
{
  uint8_t rd, k;

  rd = ((codePtr[0] & 0xf0) >> 4) + 16;
  k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

  r[rd] = r[rd] & k;
  handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
  return 0;
}

/**
 * Instruction:
 * 		ADIW Rd + 1:Rd, K
 *		1001 0110 KKdd KKKK
 * where
 *		0 <= KKKKKK <= 63
 * 		dd is {
 *			00 => 24,
 *			01 => 26,
 *			10 => 28,
 *			11 => 30
 *		}
 */
int adiw(uint8_t *codePtr)
{
  uint8_t rd, k;
  uint16_t *word, before;

  k = (*codePtr & 0xf) | ((*codePtr & 0xc0) >> 2);
  rd = ((*codePtr & 0x30) >> 3) + 24;

  word = (uint16_t*)&r[rd];
  before = *word;
  *word += k;

  handleStatusRegForAddImmWordOperation(*word, before);

  r[rd] = *word;
  r[rd+1] = (*word & 0xff00) >> 8;

  return 0;
}

/**
 * Instruction:
 * 		SUB Rd,Rr
 *		0001 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int sub(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	r[rd] = r[rd] - r[rr];
	handleStatusRegForSubSubiSbcSbciOperation(rd, rr, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		SUBI Rd, K
 *		0111 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *      16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 */
int subi(uint8_t *codePtr)
{
	uint8_t rd, k;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] - k;
	handleStatusRegForSubSubiSbcSbciOperation(rd, k, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		SBC Rd,Rr
 *		0000 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int sbc(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	r[rd] = r[rd] - r[rr] - sreg->C;
	handleStatusRegForSubSubiSbcSbciOperation(rd, rr, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		SBCI Rd, K
 *		0100 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *      16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 */
int sbci(uint8_t *codePtr)
{
	uint8_t rd, k;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] - k - sreg->C;
	handleStatusRegForSubSubiSbcSbciOperation(rd, k, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		SBIW Rd + 1:Rd, K
 *		1001 0111 KKdd KKKK
 * where
 *		0 <= KKKKKK <= 63
 * 		dd is {
 *			00 => 24,
 *			01 => 26,
 *			10 => 28,
 *			11 => 30
 *		}
 */
int sbiw(uint8_t *codePtr)
{
	uint8_t rd, k;
	uint16_t word, before;

	k = (*codePtr & 0xf) | ((*codePtr & 0xc0) >> 2);
	rd = ((*codePtr & 0x30) >> 3) + 24;

	word = (((uint16_t)r[rd+1]) << 8) | r[rd];
	before = word;
	word -= k;

  handleStatusRegForSubImmWordOperation(word, before);

	r[rd] = word;
	r[rd+1] = (word & 0xff00) >> 8;
	return 0;
}

/**
 * Instruction:
 * 		OR Rd,Rr
 *		0010 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int or(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	r[rd] = r[rd] | r[rr];
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		ORI Rd, K
 *		0110 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *      16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 */
int ori(uint8_t *codePtr)
{
	uint8_t rd, k;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] | k;
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		EOR Rd,Rr
 *		0010 01rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int eor(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	r[rd] = (~(r[rd]) & r[rr]) | (r[rd] & ~(r[rr]));
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		COM Rd
 *		1001 010d dddd 0000
 * where
 *		0 <= ddddd <= 31
 */
int com(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = 0xff - r[rd];
	handleStatusRegForComOperation(r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		NEG Rd
 *		1001 010d dddd 0001
 * where
 *		0 <= ddddd <= 31
 */
int neg(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = 0x00 - r[rd];
	handleStatusRegForNegOperation(rd, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		DEC Rd
 *		1001 010d dddd 1010
 * where
 *		0 <= ddddd <= 31
 */
int dec(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = r[rd] - 1;
	handleStatusRegForDecOperation(r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		INC Rd
 *		1001 010d dddd 0011
 * where
 *		0 <= ddddd <= 31
 */
int inc(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = r[rd] + 1;
	handleStatusRegForIncOperation(r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		CBR Rd, K
 *		0111 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *     16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 */
int cbr(uint8_t *codePtr)
{
	uint8_t rd, k;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] & (0xff - k);
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		SBR Rd, K
 *		0110 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *      16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 */
int sbr(uint8_t *codePtr)
{
	uint8_t rd, k;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] | k;
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		TST Rd
 *		0010 00dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 */
int tst(uint8_t *codePtr)
{
  uint16_t rd;

  rd = ((codePtr[1] & 0x3) << 7) | (codePtr[0] & 0xff);

  r[rd] = r[rd] & r[rd];
  handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
  return 0;
}

/**
 * Instruction:
 * 		CLR Rd
 *		0010 01dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 */
int clr(uint8_t *codePtr)
{
  uint16_t rd;

  rd = ((codePtr[1] & 0x3) << 7) | (codePtr[0] & 0xff);

  r[rd] = r[rd] | r[rd];
  handleStatusRegForClrOperation();
  return 0;
}

/**
 * Instruction:
 * 		SER Rd
 *		1110 1111 dddd 1111
 * where
 *      16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 */
int ser(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;

	r[rd] = 0xff;
	return 0;
}

/**
 * Instruction:
 * 		MUL Rd,Rr
 *		1001 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int mul(uint8_t *codePtr)
{
	uint8_t rd, rr;
	uint16_t result;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	result = r[rd] * r[rr];
	r[0] = result;
	r[1] = (result & 0xff00) >> 8;
	handleStatusRegForMulMulsMulsuOperation(result);
	return 0;
}

/**
 * Instruction:
 * 		MULS Rd, Rr
 *		0000 0010 dddd rrrr
 * where
 *      16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 *
 *      16 <= rrrr <= 31
 * 		rrrr is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 */
int muls(uint8_t *codePtr)
{
	uint8_t rd, rr;
	int16_t result;

	rd = ((*codePtr & 0xf0) >> 4) + 16;
	rr = (*codePtr & 0x0f) + 16;

	result = (int8_t)r[rd] * (int8_t)r[rr];

	r[0] = result;
	r[1] = (result & 0xff00) >> 8;
	handleStatusRegForMulMulsMulsuOperation(result);
	return 0;
}

/**
 * Instruction:
 * 		MULSU Rd, Rr
 *		0000 0010 0ddd 0rrr
 * where
 *      16 <= ddd <= 23
 * 		ddd is {
 *			000 => 16,
 *			001 => 17,
 *			010 => 18,
 *			011 => 19,
 *			100 => 20,
 *			101 => 21,
 *			110 => 22,
 *			111 => 23,
 *		}
 *
 *      16 <= rrr <= 23
 * 		rrr is {
 *			000 => 16,
 *			001 => 17,
 *			010 => 18,
 *			011 => 19,
 *			100 => 20,
 *			101 => 21,
 *			110 => 22,
 *			111 => 23,
 *		}
 */
int mulsu(uint8_t *codePtr)
{
	uint8_t rd, rr;
	uint16_t result;

	rd = ((*codePtr & 0x70) >> 4) + 16;
	rr = (*codePtr & 0x07) + 16;

	result = (int8_t)r[rd] * r[rr];

	r[0] = result;
	r[1] = (result & 0xff00) >> 8;
	handleStatusRegForMulMulsMulsuOperation(result);
	return 0;
}

/**
 * Instruction:
 * 		LSL Rd
 *		0000 11dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 */
int lsl(uint8_t *codePtr)
{
	uint8_t rd;
	uint16_t temp;

	rd = ((codePtr[1] & 0x3) << 7) | (codePtr[0] & 0xff);

	temp = ((uint16_t)r[rd] << 1);
	r[rd] = temp;
	handleStatusRegForLslRolOperation(rd, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		LSR Rd
 *		1001 010d dddd 0110
 * where
 *		0 <= ddddd <= 31
 */
int lsr(uint8_t *codePtr)
{
	uint8_t rd, temp;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	temp = r[rd];
	r[rd] = r[rd] >> 1;
	handleStatusRegForLsrOperation(rd, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		ROL Rd
 *		0001 11dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 */
int rol(uint8_t *codePtr)
{
	uint8_t rd;
	uint16_t temp;

	rd = ((codePtr[1] & 0x3) << 7) | (codePtr[0] & 0xff);

	temp = ((uint16_t)r[rd] << 1);
	r[rd] = temp + sreg->C;
	handleStatusRegForLslRolOperation(rd, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		ROR Rd
 *		1001 010d dddd 0111
 * where
 *		0 <= ddddd <= 31
 */
int ror(uint8_t *codePtr)
{
	uint8_t rd, temp;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	temp = r[rd];
	r[rd] = (r[rd] >> 1) + (sreg->C << 7);
	sreg->C = temp & 0x1;
	handleStatusRegForRorAsrOperation(rd, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		ASR Rd
 *		1001 010d dddd 0101
 * where
 *		0 <= ddddd <= 31
 */
int asr(uint8_t *codePtr)
{
	uint8_t rd, temp;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	temp = r[rd];
	r[rd] = (r[rd] >> 1) + (r[rd] & 0x80) ;
	handleStatusRegForRorAsrOperation(rd, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		SWAP Rd
 *		1001 010d dddd 0010
 * where
 *		0 <= ddddd <= 31
 */
int swap(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = ((r[rd] & 0x0f) << 4) + ((r[rd] & 0xf0) >> 4);
	return 0;
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 */
int bset(uint8_t *codePtr)
{
	uint8_t s;

	s = (*codePtr & 0x70) >> 4;

	switch(s)
	{
		case 0b000:
			sreg->C = 1;
			break;
		case 0b001:
			sreg->Z = 1;
			break;
		case 0b010:
			sreg->N = 1;
			break;
		case 0b011:
			sreg->V = 1;
			break;
		case 0b100:
			sreg->S = 1;
			break;
		case 0b101:
			sreg->H = 1;
			break;
		case 0b110:
			sreg->T = 1;
			break;
		case 0b111:
			sreg->I = 1;
			break;
		default:
			printf("error!");
	}
	return 0;
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 */
int bclr(uint8_t *codePtr)
{
	uint8_t s;

	s = (*codePtr & 0x70) >> 4;

	switch(s)
	{
		case 0b000:
			sreg->C = 0;
			break;
		case 0b001:
			sreg->Z = 0;
			break;
		case 0b010:
			sreg->N = 0;
			break;
		case 0b011:
			sreg->V = 0;
			break;
		case 0b100:
			sreg->S = 0;
			break;
		case 0b101:
			sreg->H = 0;
			break;
		case 0b110:
			sreg->T = 0;
			break;
		case 0b111:
			sreg->I = 0;
			break;
		default:
			printf("error!");
	}
	return 0;
}

/**
 * Instruction:
 * 		BST Rd, b
 *		1111 101d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 */
int bst(uint8_t *codePtr)
{
	uint8_t rd, b;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	b = *codePtr & 0x7;

	sreg->T = (r[rd] & (1 << b)) >> b;

	handleStatusRegForRorBstOperation(rd);
	return 0;
}

/**
 * Instruction:
 * 		BLD Rd, b
 *		1111 100d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 */
int bld(uint8_t *codePtr)
{
	uint8_t rd, b;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	b = *codePtr & 0x7;

  r[rd] = (r[rd] & ~(1 << b)) + (sreg->T << b);
	return 0;
}

/**
 * Instruction:
 * 		SEC None
 *		1001 0100 0000 1000
 */
int sec(uint8_t *codePtr)
{
	sreg->C = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLC None
 *		1001 0100 1000 1000
 */
int clc(uint8_t *codePtr)
{
	sreg->C = 0;
	return 0;
}

/**
 * Instruction:
 * 		SEN None
 *		1001 0100 0010 1000
 */
int sen(uint8_t *codePtr)
{
	sreg->N = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLN None
 *		1001 0100 1010 1000
 */
int cln(uint8_t *codePtr)
{
	sreg->N = 0;
	return 0;
}

/**
 * Instruction:
 * 		SEZ None
 *		1001 0100 0001 1000
 */
int sez(uint8_t *codePtr)
{
	sreg->Z = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLZ None
 *		1001 0100 1001 1000
 */
int clz(uint8_t *codePtr)
{
	sreg->Z = 0;
	return 0;
}

/**
 * Instruction:
 * 		SEI None
 *		1001 0100 0111 1000
 */
int sei(uint8_t *codePtr)
{
	sreg->I = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLI None
 *		1001 0100 1111 1000
 */
int cli(uint8_t *codePtr)
{
	sreg->I = 0;
	return 0;
}

/**
 * Instruction:
 * 		SES None
 *		1001 0100 0100 1000
 */
int ses(uint8_t *codePtr)
{
	sreg->S = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLS None
 *		1001 0100 1100 1000
 */
int cls(uint8_t *codePtr)
{
	sreg->S = 0;
	return 0;
}

/**
 * Instruction:
 * 		SEV None
 *		1001 0100 0011 1000
 */
int sev(uint8_t *codePtr)
{
	sreg->V = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLV None
 *		1001 0100 1011 1000
 */
int clv(uint8_t *codePtr)
{
	sreg->V = 0;
	return 0;
}

/**
 * Instruction:
 * 		SET None
 *		1001 0100 0110 1000
 */
int set(uint8_t *codePtr)
{
	sreg->T = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLT None
 *		1001 0100 1110 1000
 */
int clt(uint8_t *codePtr)
{
	sreg->T = 0;
	return 0;
}

/**
 * Instruction:
 * 		SEH None
 *		1001 0100 0101 1000
 */
int seh(uint8_t *codePtr)
{
	sreg->H = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLH None
 *		1001 0100 1101 1000
 */
int clh(uint8_t *codePtr)
{
	sreg->H = 0;
	return 0;
}

/**
 * Instruction:
 * 		MOV Rd, Rr
 *		0010 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int mov(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

  r[rd] = r[rr];
	return 0;
}

/**
 * Instruction:
 * 		RJMP k
 *		1100 kkkk kkkk kkkk
 * where
 *		-2K <= kkkkkkkkkkkk <= 2K
 */
int rjmp(uint8_t *codePtr)
{
	int k;

	k = ((codePtr[1] & 0xf) << 8) | (codePtr[0] & 0xff);

  if(k & 0x800)
    k |= 0xfffff000;

	return (k+1) * 2;
}

/**
 * Instruction:
 * 		JMP k
 *			1001 010k kkkk 110k
 *			kkkk kkkk kkkk kkkk
 * where
 *			0 <= kkkkkkkkkkkk <= 4M
 */
int jmp(uint8_t *codePtr)
{
	uint32_t k, k1, k2;
	k = *(uint32_t *)codePtr;

	k = ((k & 0xffff0000) >> 16) | ((k & 0x1f0) << 13) | ((k & 0x1) << 16);

	return getCodePtr(k*2) - codePtr;
}

/**
 * Instruction:
 * 		BRBS s, k
 *			1111 00kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 */
int brbs(uint8_t *codePtr)
{
	uint8_t k, s;

	k = ((codePtr[0] & 0xf8) >> 3) | ((codePtr[1] & 0x3) << 5);
  s = (*codePtr & 0x7);

  switch(s)
	{
		case 0b000:
			if(sreg->C == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b001:
			if(sreg->Z == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b010:
			if(sreg->N == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b011:
			if(sreg->V == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b100:
			if(sreg->S == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b101:
			if(sreg->H == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b110:
			if(sreg->T == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b111:
			if(sreg->I == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		default:
			printf("error!");
  }
}

/**
 * Instruction:
 * 		BRBC s, k
 *			1111 01kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 */
int brbc(uint8_t *codePtr)
{
	uint8_t k, s;

	k = ((codePtr[0] & 0xf8) >> 3) | ((codePtr[1] & 0x3) << 5);
  s = (*codePtr & 0x7);

  switch(s)
	{
		case 0b000:
			if(sreg->C == 0)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b001:
			if(sreg->Z == 0)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b010:
			if(sreg->N == 0)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b011:
			if(sreg->V == 0)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b100:
			if(sreg->S == 0)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b101:
			if(sreg->H == 0)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b110:
			if(sreg->T == 0)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b111:
			if(sreg->I == 0)
        return ((k+1) * 2);
      else
        return 2;
			break;
		default:
			printf("error!");
  }
}

/**
 * Instruction:
 * 		NOP None
 *			0000 0000 0000 0000
 *       0    0    0    0
 */
int nop(uint8_t *codePtr)
{
  return 2;
}

/**
 * Instruction:
 * 		WDR None
 *			1001 0101 1010 1000
 *       9    5    a    8
 */
int wdr(uint8_t *codePtr)
{
  throwSimpleError(WATCHDOG_EXCEPTION, NULL);
}

/**
 * Instruction:
 * 		BREAK None
 *			1001 0101 1001 1000
 *       9    5    9    8
 */
int Break(uint8_t *codePtr)
{
  throwSimpleError(BREAK_EXCEPTION, NULL);
}

/**
 * Instruction:
 * 		SLEEP None
 *			1001 0101 1001 1000
 *       9    5    9    8
 */
int sleep(uint8_t *codePtr)
{
  throwSimpleError(SLEEP_EXCEPTION, NULL);
}

/**
 * Instruction:
 * 		CP Rd, Rr
 *			0001 01rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *      0 <= rrrrr <= 31
 */
int cp(uint8_t *codePtr)
{
	uint8_t rd, rr, result;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	result = r[rd] - r[rr];
	handleStatusRegForSubSubiSbcSbciOperation(rd, rr, result);
	return 0;
}

/**
 * Instruction:
 * 		CPC Rd, Rr
 *			0000 01rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *      0 <= rrrrr <= 31
 */
int cpc(uint8_t *codePtr)
{
	uint8_t rd, rr, result;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	result = r[rd] - r[rr] - sreg->C;
	handleStatusRegForSubSubiSbcSbciOperation(rd, rr, result);
	return 0;
}

/**
 * Instruction:
 * 		CPI Rd, K
 *		0011 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *    16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 */
int cpi(uint8_t *codePtr)
{
	uint8_t rd, k;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] - k;
	handleStatusRegForSubSubiSbcSbciOperation(rd, k, r[rd]);
	return 0;
}

/**
 * Instruction:
 * 		LDI Rd, K
 *		1110 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *    16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17,
 *			0010 => 18,
 *			0011 => 19,
 *			0100 => 20,
 *			0101 => 21,
 *			0110 => 22,
 *			0111 => 23,
 *			1000 => 24,
 *			1001 => 25,
 *			1010 => 26,
 *			1011 => 27,
 *			1100 => 28,
 *			1101 => 29,
 *			1110 => 30,
 *			1111 => 31
 *		}
 */
int ldi(uint8_t *codePtr)
{
	uint8_t rd, k;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = k;
	return 0;
}
