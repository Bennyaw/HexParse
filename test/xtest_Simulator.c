#include <stdio.h>
#include "unity.h"
#include "Simulator.h"
#include "ErrorSimulator.h"
#include "Exception.h"
#include "CException.h"

void setUp(void){}

void tearDown(void){}



/**
 * Z:
 *		R7 ¯ • R6 ¯ • R5 ¯ • R4 ¯ • R3 ¯ • R2 ¯ • R1 ¯ • R0 ¯
 */
void test_is8bitZero_given_data8bit_is_0b00010100(void)
{
	uint8_t data8bit, z;
	data8bit = 0b00010100;

	z = is8bitZero(data8bit);

	TEST_ASSERT_EQUAL(0, z);
}

void test_is8bitZero_given_data8bit_is_0b00000000(void)
{
	uint8_t data8bit, z;
	data8bit = 0b00000000;

	z = is8bitZero(data8bit);

	TEST_ASSERT_EQUAL(1, z);
}

/**
 * C:
 *		R15 ¯ • Rdh7
 */
void test_is16bitADIWCarry_given_result_is_0xff4d_operand_is_0x46f6(void)
{
	uint8_t c;
	uint16_t result, operand;

	result = 0xff4d;
	operand = 0x46f6;

	c = is16bitADIWCarry(result, operand);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitADIWCarry_given_result_is_0xff4d_operand_is_0xc6f6(void)
{
	uint8_t c;
	uint16_t result, operand;

	result = 0xff4d;
	operand = 0xc6f6;

	c = is16bitADIWCarry(result, operand);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitADIWCarry_given_result_is_0x4f4d_operand_is_0x46f6(void)
{
	uint8_t c;
	uint16_t result, operand;

	result = 0x4f4d;
	operand = 0x46f6;

	c = is16bitADIWCarry(result, operand);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitADIWCarry_given_result_is_0x4f4d_operand_is_0xf6f6(void)
{
	uint8_t c;
	uint16_t result, operand;

	result = 0x4f4d;
	operand = 0xf6f6;

	c = is16bitADIWCarry(result, operand);

	TEST_ASSERT_EQUAL(1, c);
}

/**
 * Z:
 *		R15 ¯ • R14 ¯ • R13 ¯ • R12 ¯ • R11 ¯ • R10 ¯ • R9 ¯ • R8 ¯ • R7 ¯ • R6 ¯ • R5 ¯ • R4 ¯ • R3 ¯ • R2 ¯ • R1 ¯ • R0 ¯
 */
void test_is16bitZero_given_data8bit_is_0x6f94(void)
{
	uint16_t data16Bit, z;
	data16Bit = 0x6f94;

	z = is16BitZero(data16Bit);

	TEST_ASSERT_EQUAL(0, z);
}

void test_is16bitZero_given_data8bit_is_0x0000(void)
{
	uint16_t data16Bit, z;
	data16Bit = 0x0000;

	z = is16BitZero(data16Bit);

	TEST_ASSERT_EQUAL(1, z);
}

/**
 * N:
 *		R15
 */
void test_is16bitNeg_given_result_is_0x4f4d(void)
{
	uint8_t n;
	uint16_t result;

	result = 0x4f4d;

	n = is16bitNeg(result);

	TEST_ASSERT_EQUAL(0, n);
}

void test_is16bitNeg_given_result_is_0xff4d(void)
{
	uint8_t n;
	uint16_t result;

	result = 0xff4d;

	n = is16bitNeg(result);

	TEST_ASSERT_EQUAL(1, n);
}

/**
 * V:
 *		R15 • Rdh7 ¯
 */
void test_is16bitADIWOverflow_given_result_is_0x4f4d_operand_is_0xf6f6(void)
{
	uint8_t v;
	uint16_t result, operand;

	result = 0x4f4d;
	operand = 0xf6f6;

	v = is16bitADIWOverflow(result, operand);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitADIWOverflow_given_result_is_0x4f4d_operand_is_0x46f6(void)
{
	uint8_t v;
	uint16_t result, operand;

	result = 0x4f4d;
	operand = 0x46f6;

	v = is16bitADIWOverflow(result, operand);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitADIWOverflow_given_result_is_0xff4d_operand_is_0xf6f6(void)
{
	uint8_t v;
	uint16_t result, operand;

	result = 0xff4d;
	operand = 0xf6f6;

	v = is16bitADIWOverflow(result, operand);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitADIWOverflow_given_result_is_0xff4d_operand_is_0x46f6(void)
{
	uint8_t v;
	uint16_t result, operand;

	result = 0xff4d;
	operand = 0x46f6;

	v = is16bitADIWOverflow(result, operand);

	TEST_ASSERT_EQUAL(1, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is16bitSigned_given_n_is_0_v_is_0(void)
{
	uint8_t s;
	uint16_t result, operand;

	result = 0x4f4d;
	operand = 0xf6f6;

	s = is16bitSigned(result, operand);

	TEST_ASSERT_EQUAL(0, s);
}

void test_is16bitSigned_given_n_is_1_v_is_1(void)
{
	uint8_t s;
	uint16_t result, operand;

	result = 0xff4d;
	operand = 0x46f6;

	s = is16bitSigned(result, operand);

	TEST_ASSERT_EQUAL(0, s);
}

void test_handleStatusRegForAddImmWordOperation_given_result_is_0x54d6_operand_is_0xdd82(void)
{
	uint16_t result, operand;

	result = 0x54d6;
	operand = 0xdd82;

	handleStatusRegForAddImmWordOperation(result, operand);

	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
}

/**
 * C:
 *		R15 • Rdh7¯
 */
void test_is16bitSBIWCarry_given_result_is_0x4f4d_operand_is_0xf6f6(void)
{
	uint8_t c;
	uint16_t result, operand;

	result = 0x4f4d;
	operand = 0xf6f6;

	c = is16bitSBIWCarry(result, operand);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitSBIWCarry_given_result_is_0x4f4d_operand_is_0x46f6(void)
{
	uint8_t c;
	uint16_t result, operand;

	result = 0x4f4d;
	operand = 0x46f6;

	c = is16bitSBIWCarry(result, operand);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitSBIWCarry_given_result_is_0xff4d_operand_is_0xf6f6(void)
{
	uint8_t c;
	uint16_t result, operand;

	result = 0xff4d;
	operand = 0xf6f6;

	c = is16bitSBIWCarry(result, operand);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitSBIWCarry_given_result_is_0xff4d_operand_is_0x46f6(void)
{
	uint8_t c;
	uint16_t result, operand;

	result = 0xff4d;
	operand = 0x46f6;

	c = is16bitSBIWCarry(result, operand);

	TEST_ASSERT_EQUAL(1, c);
}

/**
 * V:
 *		R15 ¯ • Rdh7
 */
void test_is16bitSBIWOverflow_given_result_is_0xff4d_operand_is_0x46f6(void)
{
	uint8_t v;
	uint16_t result, operand;

	result = 0xff4d;
	operand = 0x46f6;

	v = is16bitSBIWOverflow(result, operand);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitSBIWOverflow_given_result_is_0xff4d_operand_is_0xf6f6(void)
{
	uint8_t v;
	uint16_t result, operand;

	result = 0xff4d;
	operand = 0xf6f6;

	v = is16bitSBIWOverflow(result, operand);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitSBIWOverflow_given_result_is_0x4f4d_operand_is_0x46f6(void)
{
	uint8_t v;
	uint16_t result, operand;

	result = 0x4f4d;
	operand = 0x46f6;

	v = is16bitSBIWOverflow(result, operand);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitSBIWOverflow_given_result_is_0x4f4d_operand_is_0xf6f6(void)
{
	uint8_t v;
	uint16_t result, operand;

	result = 0x4f4d;
	operand = 0xf6f6;

	v = is16bitSBIWOverflow(result, operand);

	TEST_ASSERT_EQUAL(1, v);
}

void test_handleStatusRegForSubImmWordOperation_given_result_is_0x83f5_operand_is_0x98a2(void)
{
	uint16_t result, operand;

	result = 0x83f5;
	operand = 0x98a2;

	handleStatusRegForAddImmWordOperation(result, operand);

	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * C:
 *    Rd7 • Rr7 + Rr7 • R7 ¯ + R7 ¯ • Rd7
 */
void test_is8bitAdcAddCarry_given_operand1_is_0x00_operand2_is_0x01_result_is_0x09(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0x01;
	result = 0x01;

	c = is8bitAdcAddCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0x7f_operand2_is_0x7a_result_is_0xf9(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x7f;
	operand2 = 0x7a;
	result = 0xf9;

	c = is8bitAdcAddCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0x7c_operand2_is_0x9e_result_is_0x1a(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x7c;
	operand2 = 0x9e;
	result = 0x1a;

	c = is8bitAdcAddCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0x00_operand2_is_0xf1_result_is_0xf1(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0xf1;
	result = 0xf1;

	c = is8bitAdcAddCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0xf6_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0x11;
	result = 0x07;

	c = is8bitAdcAddCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0x86_operand2_is_0x11_result_is_0x97(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x86;
	operand2 = 0x11;
	result = 0x97;

	c = is8bitAdcAddCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x2b(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x2b;

	c = is8bitAdcAddCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0xf6_operand2_is_0xa1_result_is_0x97(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0xa1;
	result = 0x97;

	c = is8bitAdcAddCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, c);
}

/**
 * N:
 *    R7
 */
void test_is8bitNeg_given_result_is_0x17(void)
{
	uint8_t n;
	uint8_t result;

	result = 0x17;

	n = is8bitNeg(result);

	TEST_ASSERT_EQUAL(0, n);
}

void test_is8bitNeg_given_result_is_0xa7(void)
{
	uint8_t n;
	uint8_t result;

	result = 0xa7;

	n = is8bitNeg(result);

	TEST_ASSERT_EQUAL(1, n);
}

/**
 * V:
 *    Rd7 • Rr7 • R7 ¯ + Rd7 ¯ • Rr7 ¯ • R7
 */
void test_is8bitOverflow_given_operand1_is_0x06_operand2_is_0x71_result_is_0x77(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x06;
	operand2 = 0x71;
	result = 0x77;

	v = is8bitOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitOverflow_given_operand1_is_0x7f_operand2_is_0x7a_result_is_0xf9(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x7f;
	operand2 = 0x7a;
	result = 0xf9;

	v = is8bitOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitOverflow_given_operand1_is_0x7c_operand2_is_0x9e_result_is_0x1a(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x7c;
	operand2 = 0x9e;
	result = 0x1a;

	v = is8bitOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitOverflow_given_operand1_is_0x00_operand2_is_0xf1_result_is_0xf1(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0xf1;
	result = 0xf1;

	v = is8bitOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitOverflow_given_operand1_is_0xf6_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0x11;
	result = 0x07;

	v = is8bitOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitOverflow_given_operand1_is_0x86_operand2_is_0x11_result_is_0x97(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x86;
	operand2 = 0x11;
	result = 0x97;

	v = is8bitOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitOverflow_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x2b(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x2b;

	v = is8bitOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitOverflow_given_operand1_is_0xf6_operand2_is_0xa1_result_is_0x97(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0xa1;
	result = 0x97;

	v = is8bitOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitSigned_given_n_is_0_v_is_0(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0x11;
	result = 0x07;

	s = is8bitSigned(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitSigned_given_n_is_0_v_is_1(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;

	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x2b;

	s = is8bitSigned(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, s);
}

void test_is8bitSigned_given_n_is_1_v_is_0(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0xa1;
	result = 0x97;

	s = is8bitSigned(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, s);
}

void test_is8bitSigned_given_n_is_1_v_is_1(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;

	operand1 = 0x7f;
	operand2 = 0x7a;
	result = 0xf9;

	s = is8bitSigned(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, s);
}

/**
 * H:
 *    Rd3 • Rr3 + Rr3 • R3 ¯ + R3 ¯ • Rd3
 */
void test_is8bitAdcAddHalfCarry_given_operand1_is_0x00_operand2_is_0x01_result_is_0x01(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0x01;
	result = 0x01;

	h = is8bitAdcAddHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0x77_operand2_is_0x71_result_is_0xf9(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x77;
	operand2 = 0x71;
	result = 0xf9;

	h = is8bitAdcAddHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0x70_operand2_is_0x9e_result_is_0x13(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x70;
	operand2 = 0x9e;
	result = 0x13;

	h = is8bitAdcAddHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0x00_operand2_is_0xff_result_is_0xff(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0xff;
	result = 0xff;

	h = is8bitAdcAddHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0xfc_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0xfc;
	operand2 = 0x11;
	result = 0x07;

	h = is8bitAdcAddHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0x8a_operand2_is_0x11_result_is_0x98(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x8a;
	operand2 = 0x11;
	result = 0x98;

	h = is8bitAdcAddHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x22(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x22;

	h = is8bitAdcAddHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0xff_operand2_is_0xab_result_is_0x98(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0xff;
	operand2 = 0xab;
	result = 0x98;

	h = is8bitAdcAddHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, h);
}

void test_handleStatusRegForAddAdcOperation_given_operand1_is_0xff_operand2_is_0xab_result_is_0x98(void)
{
	uint8_t result, operand1, operand2;

	operand1 = 0xff;
	operand2 = 0xab;
	result = 0x98;

	handleStatusRegForAddAdcOperation(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
	TEST_ASSERT_EQUAL(1, sreg->H);
}

/**
 * V:
 *		0
 */
void test_is8bitAndAndiOrOriEorCbrSbrTstOverflow_given_v_is_0(void)
{
	uint8_t v;
	sreg->V = 0;

	v = is8bitAndAndiOrOriEorCbrSbrTstOverflow();

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitAndAndiOrOriEorCbrSbrTstOverflow_given_v_is_1(void)
{
	uint8_t v;
	sreg->V = 1;

	v = is8bitAndAndiOrOriEorCbrSbrTstOverflow();

	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitAndAndiOrOriEorCbrSbrTstSigned_given_n_is_0_v_is_0(void)
{
	uint8_t s;
	uint16_t result;

	result = 0x07;

	s = is8bitAndAndiOrOriEorCbrSbrTstSigned(result);

	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitAndAndiOrOriEorCbrSbrTstSigned_given_n_is_1_v_is_0(void)
{
	uint8_t s;
	uint16_t result;

	result = 0x97;

	s = is8bitAndAndiOrOriEorCbrSbrTstSigned(result);

	TEST_ASSERT_EQUAL(1, s);
}

void test_handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation_given_result_is_0xab(void)
{
	uint8_t result;

	result = 0xab;

	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(result);

	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * C:
 *    Rd7 ¯ • Rr7 + Rr7 • R7 + R7 • Rd7 ¯
 */
void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x00_operand2_is_0x01_result_is_0x09(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0x01;
	result = 0x01;

	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x7f_operand2_is_0x7a_result_is_0xf9(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x7f;
	operand2 = 0x7a;
	result = 0xf9;

	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x7c_operand2_is_0x9e_result_is_0x1a(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x7c;
	operand2 = 0x9e;
	result = 0x1a;

	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x00_operand2_is_0xf1_result_is_0xf1(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0xf1;
	result = 0xf1;

	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0xf6_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0x11;
	result = 0x07;

	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x86_operand2_is_0x11_result_is_0x97(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x86;
	operand2 = 0x11;
	result = 0x97;

	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x2b(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x2b;

	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0xf6_operand2_is_0xa1_result_is_0x97(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0xa1;
	result = 0x97;

	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, c);
}

/**
 * H:
 *		Rd3 ¯ • Rr3 + Rr3 • R3 + R3 • Rd3 ¯
 */
void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x00_operand2_is_0x01_result_is_0x01(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0x01;
	result = 0x01;

	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x77_operand2_is_0x71_result_is_0xf9(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x77;
	operand2 = 0x71;
	result = 0xf9;

	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x70_operand2_is_0x9e_result_is_0x13(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x70;
	operand2 = 0x9e;
	result = 0x13;

	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x00_operand2_is_0xff_result_is_0xff(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0xff;
	result = 0xff;

	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0xfc_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0xfc;
	operand2 = 0x11;
	result = 0x07;

	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x8a_operand2_is_0x11_result_is_0x98(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x8a;
	operand2 = 0x11;
	result = 0x98;

	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x22(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x22;

	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0xff_operand2_is_0xab_result_is_0x98(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;

	operand1 = 0xff;
	operand2 = 0xab;
	result = 0x98;

	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, h);
}

/**
 * V:
 *    Rd7 • Rr7 ¯ • R7 ¯ + Rd7 ¯ • Rr7 • R7
 */
void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x06_operand2_is_0x71_result_is_0x77(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x06;
	operand2 = 0x71;
	result = 0x77;

	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x7f_operand2_is_0x7a_result_is_0xf9(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x7f;
	operand2 = 0x7a;
	result = 0xf9;

	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x7c_operand2_is_0x9e_result_is_0x1a(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x7c;
	operand2 = 0x9e;
	result = 0x1a;

	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x00_operand2_is_0xf1_result_is_0xf1(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0xf1;
	result = 0xf1;

	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0xf6_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0x11;
	result = 0x07;

	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x86_operand2_is_0x11_result_is_0x97(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x86;
	operand2 = 0x11;
	result = 0x97;

	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x2b(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x2b;

	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0xf6_operand2_is_0xa1_result_is_0x97(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0xa1;
	result = 0x97;

	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitSubSubiSbcSbciSigned_given_n_is_0_v_is_0(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;

	operand1 = 0x16;
	operand2 = 0x11;
	result = 0x07;

	s = is8bitSubSubiSbcSbciSigned(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitSubSubiSbcSbciSigned_given_n_is_0_v_is_1(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;

	operand1 = 0x9f;
	operand2 = 0x4c;
	result = 0x5b;

	s = is8bitSubSubiSbcSbciSigned(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, s);
}

void test_is8bitSubSubiSbcSbciSigned_given_n_is_1_v_is_0(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;

	operand1 = 0xf6;
	operand2 = 0x01;
	result = 0x97;

	s = is8bitSubSubiSbcSbciSigned(operand1, operand2, result);

	TEST_ASSERT_EQUAL(1, s);
}

void test_is8bitSubSubiSbcSbciSigned_given_n_is_1_v_is_1(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;

	operand1 = 0x7f;
	operand2 = 0x8a;
	result = 0xf9;

	s = is8bitSubSubiSbcSbciSigned(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, s);
}

void test_handleStatusRegForSubSubiSbcSbciOperation_given_operand1_is_0xaf_operand2_is_0x6b_result_is_0x11(void)
{
	uint8_t result, operand1, operand2;

	operand1 = 0xaf;
	operand2 = 0x6b;
	result = 0x11;

	handleStatusRegForSubSubiSbcSbciOperation(operand1, operand2, result);

	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
	TEST_ASSERT_EQUAL(0, sreg->H);
}

/**
 * C:
 *		1
 */
void test_is8bitComCarry_given_c_is_0(void)
{
	uint8_t c;
	//sreg->C = 0;

	c = is8bitComCarry();

	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitComCarry_given_c_is_1(void)
{
	uint8_t c;
	sreg->C = 1;

	c = is8bitComCarry();

	TEST_ASSERT_EQUAL(1, c);
}

void test_handleStatusRegForComOperation_given_operand1_is_0x00_operand2_is_0x00_result_is_0x00(void)
{
	uint8_t result, operand1, operand2;

	operand1 = 0x00;
	operand2 = 0x00;
	result = 0x00;

	handleStatusRegForComOperation(result);

	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(1, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
}

/**
 * C:
 *		R7 + R6 + R5 + R4 + R3 + R2 + R1 + R0
 */
void test_is8bitNegCarry_given_data8bit_is_0b00010100(void)
{
  uint8_t data8bit, c;
	data8bit = 0x14;

	c = is8bitNegCarry(data8bit);

	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitNegCarry_given_data8bit_is_0b00000000(void)
{
	uint8_t data8bit, c;
	data8bit = 0x0;

	c = is8bitNegCarry(data8bit);

	TEST_ASSERT_EQUAL(0, c);
}

/**
 * V:
 *		R7 • R6 ¯ • R5 ¯ • R4 ¯ • R3 ¯ • R2 ¯ • R1 ¯ • R0 ¯
 */
void test_is8bitNegOverflow_given_data8bit_is_0b10000000(void)
{
  uint8_t data8bit, v;
	data8bit = 0x80;

	v = is8bitNegOverflow(data8bit);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitNegOverflow_given_data8bit_is_0b10011111(void)
{
	uint8_t data8bit, v;
	data8bit = 0x9f;

	v = is8bitNegOverflow(data8bit);

	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitNegSigned_given_n_is_0_v_is_0(void)
{
	uint8_t s;
	uint16_t result;

	result = 0x07;

	s = is8bitNegSigned(result);

	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitNegSigned_given_n_is_1_v_is_1(void)
{
	uint8_t s;
	uint16_t result;

	result = 0x80;

	s = is8bitNegSigned(result);

	TEST_ASSERT_EQUAL(0, s);
}

/**
 * H:
 *		R3 + Rd3 ¯
 */
void test_is8bitNegHalfCarry_given_operand1_is_0xa3_result_is_0x80(void)
{
	uint8_t h;
	uint16_t result, operand1;

	operand1 = 0xa3;
	result = 0x80;

	h = is8bitNegHalfCarry(operand1, result);

	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitNegHalfCarry_given_operand1_is_0xa3_result_is_0x88(void)
{
	uint8_t h;
	uint16_t result, operand1;

	operand1 = 0xa3;
	result = 0x88;

	h = is8bitNegHalfCarry(operand1, result);

	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitNegHalfCarry_given_operand1_is_0xaf_result_is_0x81(void)
{
	uint8_t h;
	uint16_t result, operand1;

	operand1 = 0xaf;
	result = 0x81;

	h = is8bitNegHalfCarry(operand1, result);

	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitNegHalfCarry_given_operand1_is_0xaf_result_is_0x8a(void)
{
	uint8_t h;
	uint16_t result, operand1;

	operand1 = 0xaf;
	result = 0x8a;

	h = is8bitNegHalfCarry(operand1, result);

	TEST_ASSERT_EQUAL(1, h);
}

void test_handleStatusRegForNegOperation_given_operand1_is_0x81_result_is_0x94(void)
{
	uint8_t result, operand1;

	operand1 = 0x81;
	result = 0x94;

	handleStatusRegForNegOperation(operand1, result);

	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
	TEST_ASSERT_EQUAL(0, sreg->H);
}

void test_handleStatusRegForIncOperation_given_operand1_is_0x81_result_is_0x94(void)
{
	uint8_t result;

	result = 0x94;

	handleStatusRegForIncOperation(result);

	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * V:
 *		R7 ¯ • R6 • R5 • R4 • R3 • R2 • R1 • R0
 */
void test_is8bitDecOverflow_given_data8bit_is_0b01111111(void)
{
	uint8_t data8bit, v;
	data8bit = 0x7f;

	v = is8bitDecOverflow(data8bit);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitDecOverflow_given_data8bit_is_0b10011100(void)
{
	uint8_t data8bit, v;
	data8bit = 0x9c;

	v = is8bitDecOverflow(data8bit);

	TEST_ASSERT_EQUAL(0, v);
}

void test_handleStatusRegForClrOperation(void)
{
	handleStatusRegForClrOperation();

	TEST_ASSERT_EQUAL(1, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
}

/**
 * C:
 *		R15
 */
void test_is16bitMulMulsMulsuCarry_given_result_is_0x6f73(void)
{
	uint16_t result, c;
	result = 0x6f73;

	c = is16bitMulMulsMulsuCarry(result);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitMulMulsMulsuCarry_given_result_is_0xff73(void)
{
	uint16_t result, c;
	result = 0xff73;

	c = is16bitMulMulsMulsuCarry(result);

	TEST_ASSERT_EQUAL(1, c);
}

void test_handleStatusRegForMulMulsMulsuOperation_given_reult_is_0x1234(void)
{
	uint16_t result;
	result = 0x1234;

	handleStatusRegForMulMulsMulsuOperation(result);

	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
}

/**
 * C:
 *		Rd7
 */
void test_is8bitLslRolCarry_given_operand1_is_0x6f(void)
{
	uint8_t operand1, c;
	operand1 = 0x6f;

	c = is8bitLslRolCarry(operand1);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitLslRolCarry_given_operand1_is_0xf6(void)
{
	uint8_t operand1, c;
	operand1 = 0xf6;

	c = is8bitLslRolCarry(operand1);

	TEST_ASSERT_EQUAL(1, c);
}

/**
 * V:
 *		N ^ C
 */
void test_is8bitLslRolOverflow_given_operand1_is_0x6f_result_is_0x47(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x6f;
	result = 0x47;

	v = is8bitLslRolOverflow(operand1, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitLslRolOverflow_given_operand1_is_0x6f_result_is_0x89(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x6f;
	result = 0x89;

	v = is8bitLslRolOverflow(operand1, result);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitLslRolOverflow_given_operand1_is_0x92_result_is_0x47(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x92;
	result = 0x47;

	v = is8bitLslRolOverflow(operand1, result);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitLslRolOverflow_given_operand1_is_0xff_result_is_0x96(void)
{
	uint8_t operand1, result, v;
	operand1 = 0xff;
	result = 0x96;

	v = is8bitLslRolOverflow(operand1, result);

	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitLslRolSigned_given_operand1_is_0x6f_result_is_0x47(void)
{
	uint8_t operand1, result, s;
	operand1 = 0x6f;
	result = 0x47;

	s = is8bitLslRolSigned(operand1, result);

	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitLslRolSigned_given_operand1_is_0x6f_result_is_0x89(void)
{
	uint8_t operand1, result, s;
	operand1 = 0x6f;
	result = 0x89;

	s = is8bitLslRolSigned(operand1, result);

	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitLslRolSigned_given_operand1_is_0x92_result_is_0x47(void)
{
	uint8_t operand1, result, s;
	operand1 = 0x92;
	result = 0x47;

	s = is8bitLslRolSigned(operand1, result);

	TEST_ASSERT_EQUAL(1, s);
}

void test_is8bitLslRolSigned_given_operand1_is_0xff_result_is_0x96(void)
{
	uint8_t operand1, result, s;
	operand1 = 0xff;
	result = 0x96;

	s = is8bitLslRolSigned(operand1, result);

	TEST_ASSERT_EQUAL(1, s);
}

/**
 * H:
 *		Rd3
 */
void test_is8bitLslRolHalfCarry_given_operand1_is_0x34(void)
{
	uint8_t operand1, h;
	operand1 = 0x34;

	h = is8bitLslRolHalfCarry(operand1);

	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitLslRolHalfCarry_given_operand1_is_0xff(void)
{
	uint8_t operand1, h;
	operand1 = 0xff;

	h = is8bitLslRolHalfCarry(operand1);

	TEST_ASSERT_EQUAL(1, h);
}

void test_handleStatusRegForLslRolOperation_given_operand1_is_0x87_result_is_0x43(void)
{
	uint8_t operand1, result;
	operand1 = 0x87;
	result = 0x43;

	handleStatusRegForLslRolOperation(operand1, result);

	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
	TEST_ASSERT_EQUAL(0, sreg->H);
}

/**
 * C:
 *		Rd0
 */
void test_is8bitLsrRorCarry_given_operand1_is_0x68(void)
{
	uint8_t operand1, c;
	operand1 = 0x68;

	c = is8bitLsrRorCarry(operand1);

	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitLsrRorCarry_given_operand1_is_0xf9(void)
{
	uint8_t operand1, c;
	operand1 = 0xf9;

	c = is8bitLsrRorCarry(operand1);

	TEST_ASSERT_EQUAL(1, c);
}

/**
 * V:
 *		N ^ C
 */
void test_is8bitLsrOverflow_given_operand1_is_0x68(void)
{
	uint8_t operand1, v;
	operand1 = 0x68;

	v = is8bitLsrOverflow(operand1);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitLsrOverflow_given_operand1_is_0x6f_operand1_is_0x89(void)
{
	uint8_t operand1, v;
	operand1 = 0x89;

	v = is8bitLsrOverflow(operand1);

	TEST_ASSERT_EQUAL(1, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitLsrSigned_given_operand1_is_0x68(void)
{
	uint8_t operand1, s;
	operand1 = 0x68;

	s = is8bitLsrSigned(operand1);

	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitLsrSigned_given_operand1_is_0x6f_operand1_is_0x89(void)
{
	uint8_t operand1, s;
	operand1 = 0x89;

	s = is8bitLsrSigned(operand1);

	TEST_ASSERT_EQUAL(1, s);
}

void test_handleStatusRegForLsrOperation_given_operand1_is_0x87_result_is_0x43(void)
{
	uint8_t operand1, result;
	operand1 = 0x87;
	result = 0x43;

	handleStatusRegForLsrOperation(operand1, result);

	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * V:
 *		N ^ C
 */
void test_is8bitRorOverflow_given_operand1_is_0x67_result_is_0x04(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x68;
	result = 0x04;

	v = is8bitRorOverflow(operand1, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitRorOverflow_given_operand1_is_0x67_result_is_0x94(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x68;
	result = 0x94;

	v = is8bitRorOverflow(operand1, result);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitRorOverflow_given_operand1_is_0x89_result_is_0x04(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x89;
	result = 0x04;

	v = is8bitRorOverflow(operand1, result);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitRorOverflow_given_operand1_is_0x89_result_is_0x94(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x89;
	result = 0x94;

	v = is8bitRorOverflow(operand1, result);

	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitRorSigned_given_operand1_is_0x68_result_is_0x04(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x68;
	result = 0x04;

	v = is8bitRorSigned(operand1, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitRorSigned_given_operand1_is_0x67_result_is_0x04(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x67;
	result = 0x04;

	v = is8bitRorSigned(operand1, result);

	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitRorSigned_given_operand1_is_0x88_result_is_0x94(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x88;
	result = 0x94;

	v = is8bitRorSigned(operand1, result);

	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitRorSigned_given_operand1_is_0x87_result_is_0x94(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x87;
	result = 0x94;

	v = is8bitRorSigned(operand1, result);

	TEST_ASSERT_EQUAL(1, v);
}

void test_handleStatusRegForRorAsrOperation_given_operand1_is_0x87_result_is_0x43(void)
{
	uint8_t operand1, result;
	operand1 = 0x87;
	result = 0x43;

	handleStatusRegForLsrOperation(operand1, result);

	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
}

void test_handleStatusRegForRorBstOperation_given_operand1_is_0b010(void)
{
	uint8_t operand1, t;
	operand1 = 0b010;

	t = handleStatusRegForRorBstOperation(operand1);

	TEST_ASSERT_EQUAL(1, t);
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
 *
 * Simulate adiw R27,26, 13
 * 		KKKKKK = 13 = 0xd = b'001101'
 *		    dd = b'01' ==> 26
 *		1001 0110 0001 1101
 *		  9    6    1    d
 */
void test_AvrOperatorTable_given_adiw_r27_r26_13(void)
{
	int k;
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x1d, 0x96,
	};
	uint8_t *progCounter = codeMemory;

	r[26] = 24;
	r[27] = 10;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(37, r[26]);
	TEST_ASSERT_EQUAL(10, r[27]);
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
 *
 * Simulate adiw R31,30, 2
 * 		KKKKKK = 2 = 0x2 = b'000010'
 *		    dd = b'11' ==> 30
 *		1001 0110 0011 0010
 *		  9    6    3    2
 */
void test_AvrOperatorTable_given_adiw_r31_r30_2(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x32, 0x96,
	};
	uint8_t *progCounter = codeMemory;

	r[30] = 0xff;
	r[31] = 0x10;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x01, r[30]);
	TEST_ASSERT_EQUAL(0x11, r[31]);
}

/**
 * Instruction:
 * 		ADD Rd,Rr
 *		0000 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate add R4,R7
 * 		ddddd = 4 = b'00100'
 * 		rrrrr = 7 = b'00111'
 *		0000 1100 0100 0111
 *		  0    c    4    7
 */
void test_AvrOperatorTable_given_add_r4_r7(void)
{
    uint8_t codeMemory[] = {
		0x47, 0x0c,
	};
	uint8_t *progCounter = codeMemory;

	r[7] = 23;
	r[4] = 5;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(23, r[7]);
	TEST_ASSERT_EQUAL(28, r[4]);
}

/**
 * Instruction:
 * 		AND Rd,Rr
 *		0010 00rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate and R2,R3
 * 		ddddd = 2 = b'00010'
 * 		rrrrr = 3 = b'00011'
 *		0010 0000 0010 0011
 *		  2    0    2    3
 */
void test_AvrOperatorTable_given_and_r2_r3(void)
{
	uint8_t codeMemory[] = {
		0x23, 0x20,
	};
	uint8_t *progCounter = codeMemory;

	r[2] = 0x5;
	r[3] = 0xf;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x5, r[2]);
	TEST_ASSERT_EQUAL(0xf, r[3]);
}

/**
 * Instruction:
 * 		AND Rd,Rr
 *		0010 00rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate and R4,R31
 * 		ddddd = 4  = b'00100'
 * 		rrrrr = 31 = b'11111'
 *		0010 0010 0100 1111
 *		  2    2    4    f
 */
void test_AvrOperatorTable_given_and_r4_r31(void)
{
	uint8_t codeMemory[] = {
		0x4f, 0x22,
	};
	uint8_t *progCounter = codeMemory;

	r[4]  = 0x75;
	r[31] = 0x78;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x70, r[4]);
	TEST_ASSERT_EQUAL(0x78, r[31]);
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
 *
 * Simulate andi R20, 42
 * 		KKKKKKKK = 42 = b'00101010'
 *		    dddd = b'0100' ==> 20
 *		0111 0010 0100 1010
 *		  7    2    4    a
 */
void test_AvrOperatorTable_given_andi_r20_42(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x4a, 0x72,
	};
	uint8_t *progCounter = codeMemory;

	r[20] = 0xf0;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x20, r[20]);
}

/**
 * Instruction:
 * 		ADC Rd,Rr
 *		0001 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate add R5,R9
 * 		ddddd = 5 = b'00101'
 * 		rrrrr = 9 = b'01001'
 *		0001 1100 0101 1001
 *		  0    c    5    9
 */
void test_AvrOperatorTable_given_adc_r5_r9(void)
{
    uint8_t codeMemory[] = {
		0x59, 0x1c,
	};
	uint8_t *progCounter = codeMemory;

	sreg->C = 1;

	r[5] = 0x7d;
	r[9] = 0x1c;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x9a, r[5]);
	TEST_ASSERT_EQUAL(0x1c, r[9]);
}

/**
 * Instruction:
 * 		SUB Rd,Rr
 *		0001 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate sub R15,R21
 * 		ddddd = 15 = b'01111'
 * 		rrrrr = 21 = b'10101'
 *		0001 1010 1111 0101
 *		  1     a     f     5
 */
void test_AvrOperatorTable_given_sub_r15_r21(void)
{
    uint8_t codeMemory[] = {
		0xf5, 0x1a,
	};
	uint8_t *progCounter = codeMemory;

	r[15] = 23;
	r[21] = 5;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(18, r[15]);
	TEST_ASSERT_EQUAL(5, r[21]);
}

/**
 * Instruction:
 * 		SUBI Rd, K
 *		0101 KKKK dddd KKKK
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
 *
 * Simulate subi R25, 201
 * 		KKKKKKKK = 201 = b'11001001'
 *		    dddd = b'1001' ==> 25
 *		0101 1100 1001 1001
 *		  5     c     9     9
 */
void test_AvrOperatorTable_given_subi_r25_201(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x99, 0x5c,
	};
	uint8_t *progCounter = codeMemory;

	r[25] = 0xf0;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x27, r[25]);
}

/**
 * Instruction:
 * 		SBC Rd,Rr
 *		0000 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate sbc R9,R16
 * 		ddddd = 9 = b'01001'
 * 		rrrrr = 16 = b'10000'
 *		0000 1010 1001 0000
 *		  0     a     9    0
 */
void test_AvrOperatorTable_given_sbc_r9_r16(void)
{
  uint8_t codeMemory[] = {
		0x90, 0x0a,
	};
	uint8_t *progCounter = codeMemory;

	sreg->C = 1;

	r[9]  = 0x9c;
	r[16] = 0x52;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x49, r[9]);
	TEST_ASSERT_EQUAL(0x52, r[16]);
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
 *
 * Simulate sbci R29, 155
 * 		KKKKKKKK = 155 = b'10011011'
 *		    dddd = b'1101' ==> 29
 *		0100 1001 1101 1011
 *		  4     9     d     b
 */
void test_AvrOperatorTable_given_sbci_r29_155(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0xdb, 0x49,
	};
	uint8_t *progCounter = codeMemory;

	sreg->C = 1;

	r[29] = 0xff;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x63, r[29]);
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
 *
 * Simulate sbiw R29,28, 54
 * 		KKKKKK = 54 = 0x36 = b'110110'
 *		    dd = b'10' ==> 28
 *		1001 0111 1110 0110
 *		  9     7     e     6
 */
void test_AvrOperatorTable_given_sbiw_r29_r28_54(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0xe6, 0x97,
	};
	uint8_t *progCounter = codeMemory;

	r[28] = 0x28;
	r[29] = 0x10;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0xf2, r[28]);
	TEST_ASSERT_EQUAL(0xf, r[29]);
}

/**
 * Instruction:
 * 		OR Rd,Rr
 *		0010 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate or R2,R8
 * 		ddddd = 2 = b'00010'
 * 		rrrrr = 8 = b'01000'
 *		0010 1000 0010 1000
 *		  2     8     2     8
 */
void test_AvrOperatorTable_given_or_r2_r8(void)
{
    uint8_t codeMemory[] = {
		0x28, 0x28,
	};
	uint8_t *progCounter = codeMemory;

	r[2] = 0xe;
	r[8] = 0x18;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x1e, r[2]);
	TEST_ASSERT_EQUAL(0x18, r[8]);
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
 *
 * Simulate ori R23, 78
 * 		KKKKKKKK = 78 = b'01001110'
 *		    dddd = b'0111' ==> 23
 *		0110 0100 0111 1110
 *		  6     4     7     e
 */
void test_AvrOperatorTable_given_ori_r23_78(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x7e, 0x64,
	};
	uint8_t *progCounter = codeMemory;

	r[23] = 0x63;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x6f, r[23]);
}

/**
 * Instruction:
 * 		EOR Rd,Rr
 *		0010 01rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate eor R16,R10
 * 		ddddd = 16 = b'10000'
 * 		rrrrr = 10 = b'01010'
 *		0010 0101 0000 1010
 *		  2     5     0     a
 */
void test_AvrOperatorTable_given_eor_r16_r10(void)
{
    uint8_t codeMemory[] = {
		0x0a, 0x25,
	};
	uint8_t *progCounter = codeMemory;

	r[16] = 0x47;
	r[10] = 0xe3;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0xa4, r[16]);
	TEST_ASSERT_EQUAL(0xe3, r[10]);
}

/**
 * Instruction:
 * 		COM Rd
 *		1001 010d dddd 0000
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate com R9
 * 		ddddd = 9 = b'01001'
 *		1001 0100 1001 0000
 *		  9     4      9      0
 */
void test_AvrOperatorTable_given_com_r9(void)
{
    uint8_t codeMemory[] = {
		0x90, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	r[9] = 0x66;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x99, r[9]);
}

/**
 * Instruction:
 * 		NEG Rd
 *		1001 010d dddd 0001
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate neg R9
 * 		ddddd = 9 = b'01001'
 *		1001 0100 1001 0001
 *		  9     4      9      1
 */
void test_AvrOperatorTable_given_neg_r9(void)
{
    uint8_t codeMemory[] = {
		0x91, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	r[9] = 0x1;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0xff, r[9]);
}

/**
 * Instruction:
 * 		DEC Rd
 *		1001 010d dddd 1010
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate dec R16
 * 		ddddd = 16 = b'10000'
 *		1001 0101 0000 1010
 *		  9     5     0      a
 */
void test_AvrOperatorTable_given_dec_r16(void)
{
    uint8_t codeMemory[] = {
		0x0a, 0x95,
	};
	uint8_t *progCounter = codeMemory;

	r[16] = 0x9b;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x9a, r[16]);
}

/**
 * Instruction:
 * 		INC Rd
 *		1001 010d dddd 0011
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate inc R16
 * 		ddddd = 16 = b'10000'
 *		1001 0101 0000 0011
 *		  9     5      0      3
 */
void test_AvrOperatorTable_given_inc_r16(void)
{
    uint8_t codeMemory[] = {
		0x03, 0x95,
	};
	uint8_t *progCounter = codeMemory;

	r[16] = 0x9b;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x9c, r[16]);
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
 *
 * Simulate sbr R31, 240
 * 		KKKKKKKK = 240 = b'11110000'
 *		    dddd = b'1111' ==> 31
 *		0110 1111 1111 0000
 *		  6     f      f     0
 */
void test_AvrOperatorTable_given_sbr_r31_240(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0xf0, 0x6f,
	};
	uint8_t *progCounter = codeMemory;

	r[31] = 0xe6;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0xf6, r[31]);
}

/**
 * Instruction:
 * 		TST Rd
 *		0010 00dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 *
 * Simulate tst R18
 * 		dddddddddd = 18 = b'0000010010'
 *		0010 0000 0001 0010
 *		  2     0     1      2
 */
void test_AvrOperatorTable_given_tst_r18(void)
{
	uint8_t codeMemory[] = {
		0x12, 0x20,
	};
	uint8_t *progCounter = codeMemory;

	r[18] = 0x3b;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x3b, r[18]);
}

/**
 * Instruction:
 * 		CLR Rd
 *		0010 01dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 *
 * Simulate clr R3
 * 		dddddddddd = 3 = b'0000000011'
 *		0010 0100 0000 0011
 *		  2      4     0     3
 */
void test_AvrOperatorTable_given_clr_r3(void)
{
	uint8_t codeMemory[] = {
		0x03, 0x24,
	};
	uint8_t *progCounter = codeMemory;

	r[3] = 0x7c;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x7c, r[3]);
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
 *
 * Simulate ser R22
 *		    dddd = b'0110' ==> 22
 *		1110 1111 0110 1111
 *		  e     f      6     f
 */
void test_AvrOperatorTable_given_ser_r22(void)
{
	uint8_t codeMemory[] = {
		0x6f, 0xef,
	};
	uint8_t *progCounter = codeMemory;

	r[22] = 0x87;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0xff, r[22]);
}

/**
 * Instruction:
 * 		MUL Rd,Rr
 *		1001 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate mul R4,R5
 * 		ddddd = 4 = b'00100'
 * 		rrrrr = 5 = b'00101'
 *		1001 1100 0100 0101
 *		  9      c     4     5
 */
void test_AvrOperatorTable_given_mul_r4_r5(void)
{
    uint8_t codeMemory[] = {
		0x45, 0x9c,
	};
	uint8_t *progCounter = codeMemory;

	r[4] = 0x34;
	r[5] = 0x6d;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x16, r[1]);
	TEST_ASSERT_EQUAL(0x24, r[0]);
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
 *
 * Simulate muls R21, R20
 *		    dddd = b'0101' ==> 21
 *		    rrrr = b'0100' ==> 20
 *		0000 0010 0101 0100
 *		  0      2      5     4
 */
void test_AvrOperatorTable_given_muls_r21_r20(void)
{
	uint8_t codeMemory[] = {
		0x54, 0x02,
	};
	uint8_t *progCounter = codeMemory;

	r[21] = -32;
	r[20] = -10;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x1, r[1]);
	TEST_ASSERT_EQUAL(0x40, r[0]);
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
 *
 * Simulate mulsu R23, R20
 *		    ddd = b'111' ==> 21
 *		    rrr = b'100' ==> 20
 *		0000 0010 0111 0100
 *		  0      2      7     4
 */
void test_AvrOperatorTable_given_mulsu_r23_r20(void)
{
	uint8_t codeMemory[] = {
		0x74, 0x02,
	};
	uint8_t *progCounter = codeMemory;

	r[23] = -32;
	r[20] = 10;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0xfe, r[1]);
	TEST_ASSERT_EQUAL(0xc0, r[0]);
}

/**
 * Instruction:
 * 		LSL Rd
 *		0000 11dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 *
 * Simulate lsl R0
 * 		dddddddddd = 0 = b'0000000000'
 *		0000 1100 0000 0000
 *		  0      c     0      0
 */
void test_AvrOperatorTable_given_lsl_r0(void)
{
	uint8_t codeMemory[] = {
		0x00, 0x0c,
	};
	uint8_t *progCounter = codeMemory;

	r[0] = 0x97;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x2e, r[0]);
}

/**
 * Instruction:
 * 		LSR Rd
 *		1001 010d dddd 0110
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate lsr R1
 * 		ddddd = 1 = b'00001'
 *		1001 0100 0001 0110
 *		  9      4     1      6
 */
void test_AvrOperatorTable_given_lsr_r1(void)
{
	uint8_t codeMemory[] = {
		0x16, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	r[1] = 0x91;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x48, r[1]);
	TEST_ASSERT_EQUAL(1, sreg->C);
}

/**
 * Instruction:
 * 		ROR Rd
 *		1001 010d dddd 0111
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ror R3
 * 		ddddd = 3 = b'00011'
 *		1001 0100 0011 0111
 *		  9      4     3      7
 */
void test_AvrOperatorTable_given_ror_r3(void)
{
	uint8_t codeMemory[] = {
		0x37, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	r[3] = 0x90;

	sreg->C = 1;
	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0xc8, r[3]);
}

/**
 * Instruction:
 * 		ASR Rd
 *		1001 010d dddd 0101
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ror R3
 * 		ddddd = 4 = b'00100'
 *		1001 0100 0100 0101
 *		  9      4     4      5
 */
void test_AvrOperatorTable_given_asr_r4(void)
{
	uint8_t codeMemory[] = {
		0x45, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	r[4] = 0x90;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0xc8 ,r[4]);
}

/**
 * Instruction:
 * 		SWAP Rd
 *		1001 010d dddd 0010
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ror R7
 * 		ddddd = 7 = b'00111'
 *		1001 0100 0111 0010
 *		  9      4     7      2
 */
void test_AvrOperatorTable_given_swap_r7(void)
{
	uint8_t codeMemory[] = {
		0x72, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	r[7] = 0x94;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x49, r[7]);
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bset 3
 * 		sss = 3 = b'011'
 *		1001 0100 0011 1000
 *		  9      4     3      8
 */
void test_AvrOperatorTable_given_bset_3(void)
{
	uint8_t codeMemory[] = {
		0x38, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->V);
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bset 7
 * 		sss = 3 = b'011'
 *		1001 0100 0111 1000
 *		  9      4     7      8
 */
void test_AvrOperatorTable_given_bset_7(void)
{
	uint8_t codeMemory[] = {
		0x78, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->I);
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bclr 3
 * 		sss = 3 = b'011'
 *		1001 0100 1011 1000
 *		  9      4     b      8
 */
void test_AvrOperatorTable_given_bclr_3(void)
{
	uint8_t codeMemory[] = {
		0xb8, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->V);
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bclr 7
 * 		sss = 7 = b'111'
 *		1001 0100 1111 1000
 *		  9      4     f      8
 */
void test_AvrOperatorTable_given_bclr_7(void)
{
	uint8_t codeMemory[] = {
		0xf8, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->I);
}

/**
 * Instruction:
 * 		BST Rd, b
 *		1111 101d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 *
 * Simulate bst R6, 3
 * 		ddddd = 6 = b'00110'
 *		   bbb = 3 = b'011'
 *		1111 1010 0110 0011
 *		  f      a     6      3
 */
void test_AvrOperatorTable_given_bst_r6_3(void)
{
	uint8_t codeMemory[] = {
		0x63, 0xfa,
	};
	uint8_t *progCounter = codeMemory;

	r[6] = 0x94;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->T);
}

/**
 * Instruction:
 * 		BST Rd, b
 *		1111 101d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 *
 * Simulate bst R6, 7
 * 		ddddd = 6 = b'00110'
 *		   bbb = 7 = b'111'
 *		1111 1010 0110 0111
 *		 f    a    6    7
 */
void test_AvrOperatorTable_given_bst_r6_7(void)
{
	uint8_t codeMemory[] = {
		0x67, 0xfa,
	};
	uint8_t *progCounter = codeMemory;

	r[6] = 0x94;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->T);
}

/**
 * Instruction:
 * 		BLD Rd, b
 *		1111 100d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 *
 * Simulate bld R6, 3
 * 		ddddd = 6 = b'00110'
 *		   bbb = 3 = b'011'
 *		1111 1000 0110 0011
 *		  f      8     6      3
 */
void test_AvrOperatorTable_given_bld_r6_3(void)
{
	uint8_t codeMemory[] = {
		0x63, 0xf8,
	};
	uint8_t *progCounter = codeMemory;

	r[6] = 0x94;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x9c, r[6]);
}

/**
 * Instruction:
 * 		BLD Rd, b
 *		1111 100d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 *
 * Simulate bld R6, 7
 * 		ddddd = 6 = b'00110'
 *		   bbb = 3 = b'111'
 *		1111 1000 0110 0111
 *		  f      8     6      7
 */
void test_AvrOperatorTable_given_bld_r6_7(void)
{
	uint8_t codeMemory[] = {
		0x67, 0xf8,
	};
	uint8_t *progCounter = codeMemory;

	r[6] = 0x94;

	sreg->T = 0;
	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x14, r[6]);
}

/**
 * Instruction:
 * 		SEC None
 *		1001 0100 0000 1000
 *
 * Simulate sec
 *		1001 0100 0000 1000
 *		  9      4     0      8
 */
void test_AvrOperatorTable_given_sec(void)
{
	uint8_t codeMemory[] = {
		0x08, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->C);
}

/**
 * Instruction:
 * 		CLC None
 *		1001 0100 1000 1000
 *
 * Simulate clc
 *		1001 0100 1000 1000
 *		  9      4     8      8
 */
void test_AvrOperatorTable_given_clc(void)
{
	uint8_t codeMemory[] = {
		0x88, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->C);
}

/**
 * Instruction:
 * 		SEN None
 *		1001 0100 0010 1000
 *
 * Simulate sen
 *		1001 0100 0010 1000
 *		  9      4     2      8
 */
void test_AvrOperatorTable_given_sen(void)
{
	uint8_t codeMemory[] = {
		0x28, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->N);
}

/**
 * Instruction:
 * 		CLN None
 *		1001 0100 1010 1000
 *
 * Simulate cln
 *		1001 0100 1010 1000
 *		  9      4     a      8
 */
void test_AvrOperatorTable_given_cln(void)
{
	uint8_t codeMemory[] = {
		0xa8, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->N);
}

/**
 * Instruction:
 * 		SEZ None
 *		1001 0100 0001 1000
 *
 * Simulate sez
 *		1001 0100 0001 1000
 *		  9      4     1      8
 */
void test_AvrOperatorTable_given_sez(void)
{
	uint8_t codeMemory[] = {
		0x18, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->Z);
}

/**
 * Instruction:
 * 		CLZ None
 *		1001 0100 1001 1000
 *
 * Simulate clz
 *		1001 0100 1001 1000
 *		  9      4     9      8
 */
void test_AvrOperatorTable_given_clz(void)
{
	uint8_t codeMemory[] = {
		0x98, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->Z);
}

/**
 * Instruction:
 * 		SEI None
 *			1001 0100 0111 1000
 *
 * Simulate sei
 *			1001 0100 0111 1000
 *			  9      4     7      8
 */
void test_AvrOperatorTable_given_sei(void)
{
	uint8_t codeMemory[] = {
		0x78, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->I);
}

/**
 * Instruction:
 * 		CLI None
 *			1001 0100 1111 1000
 *
 * Simulate cli
 *			1001 0100 1111 1000
 *			  9      4     f      8
 */
void test_AvrOperatorTable_given_cli(void)
{
	uint8_t codeMemory[] = {
		0xf8, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->I);
}

/**
 * Instruction:
 * 		SES None
 *			1001 0100 0100 1000
 *
 * Simulate ses
 *			1001 0100 0100 1000
 *			  9      4     4      8
 */
void test_AvrOperatorTable_given_ses(void)
{
	uint8_t codeMemory[] = {
		0x48, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * Instruction:
 * 		CLS None
 *			1001 0100 1100 1000
 *
 * Simulate cls
 *			1001 0100 1100 1000
 *		 	 9      4     c      8
 */
void test_AvrOperatorTable_given_cls(void)
{
	uint8_t codeMemory[] = {
		0xc8, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->S);
}

/**
 * Instruction:
 * 		SEV None
 *			1001 0100 0011 1000
 *
 * Simulate sev
 *			1001 0100 0011 1000
 *			  9      4     3      8
 */
void test_AvrOperatorTable_given_sev(void)
{
	uint8_t codeMemory[] = {
		0x38, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->V);
}

/**
 * Instruction:
 * 		CLV None
 *			1001 0100 1011 1000
 *
 * Simulate clv
 *			1001 0100 1011 1000
 *			  9      4     b      8
 */
void test_AvrOperatorTable_given_clv(void)
{
	uint8_t codeMemory[] = {
		0xb8, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->V);
}

/**
 * Instruction:
 * 		SET None
 *			1001 0100 0110 1000
 *
 * Simulate set
 *			1001 0100 0110 1000
 *			  9      4     6      8
 */
void test_AvrOperatorTable_given_set(void)
{
	uint8_t codeMemory[] = {
		0x68, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->T);
}

/**
 * Instruction:
 * 		CLT None
 *			1001 0100 1110 1000
 *
 * Simulate clt
 *			1001 0100 1110 1000
 *			  9      4     e      8
 */
void test_AvrOperatorTable_given_clt(void)
{
	uint8_t codeMemory[] = {
		0xe8, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->T);
}

/**
 * Instruction:
 * 		SEH None
 *			1001 0100 0101 1000
 *
 * Simulate seh
 *		1001 0100 0101 1000
 *		 9    4    5   8
 */
void test_AvrOperatorTable_given_seh(void)
{
	uint8_t codeMemory[] = {
		0x58, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->H);
}

/**
 * Instruction:
 * 		CLH None
 *			1001 0100 1101 1000
 *
 * Simulate clh
 *		1001 0100 1101 1000
 *		  9   4    d    8
 */
void test_AvrOperatorTable_given_clh(void)
{
	uint8_t codeMemory[] = {
		0xd8, 0x94,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->H);
}

/**
 * Instruction:
 * 		MOV Rd, Rr
 *			0010 11rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *			0 <= rrrrr <= 31
 *
 * Simulate mov
 *			0010 1110 1101 0001
 *			  2       e      d      1
 */
void test_AvrOperatorTable_given_mov_r13_r17(void)
{
	uint8_t codeMemory[] = {
		0xd1, 0x2e,
	};
	uint8_t *progCounter = codeMemory;

	r[17] = 0x54;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0x54, r[13]);
}

/**
 * Instruction:
 * 		RJMP k
 *			1100 kkkk kkkk kkkk
 * where
 *			-2K <= kkkkkkkkkkkk <= 2K
 *
 * Simulate rjmp -2
 *			1100 1111 1111 1110
 *			 c   	 f  	   f  	  e
 */
void test_AvrOperatorTable_given_rjmp_0xffe(void)
{
  uint32_t k;
	uint8_t codeMemory[] = {
		0xfe, 0xcf,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

	k = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_INT32(-2, k);
}

/**
 * Instruction:
 * 		JMP k
 *			1001 010k kkkk 110k
 *			kkkk kkkk kkkk kkkk
 * where
 *			0 <= kkkkkkkkkkkk <= 4M
 *
 * Simulate jmp 4
 *			1001 0100 0000 1100
 *				9				4			0			c
 *			0000 0000 0000 0100
 *			 	0 	 		  0   	  0  	 	 4
 */
void test_AvrOperatorTable_given_jmp_4(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
		0x0c, 0x94, 0x04, 0x00,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

	k = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_INT32(8, k);
}

/**
 * Instruction:
 * 		JMP k
 *			1001 010k kkkk 110k
 *			kkkk kkkk kkkk kkkk
 * where
 *			0 <= kkkkkkkkkkkk <= 4M
 *
 * Simulate jmp 1
 *			1001 0100 0000 1100
 *				9				4			0			c
 *			0000 0000 0000 0001
 *			 	0 	 		  0   	  0  	 	 1
 */
void test_AvrOperatorTable_given_jmp_1(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
	  // start:
		0x03, 0x95,														// inc r16
		// here:
		0xfe, 0xcf,															// rjmp start
		0x12, 0x2c,															// mov r1, r2
		0x0c, 0x94, 0x01, 0x00,				// jmp here
	};
	uint8_t *progCounter = &codeMemory[6];
	flash = codeMemory;

	k = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_INT32(-4, k);
}

/**
 * Instruction:
 * 		JMP k
 *			1001 010k kkkk 110k
 *			kkkk kkkk kkkk kkkk
 * where
 *			0 <= kkkkkkkkkkkk <= 4M
 *
 * Simulate jmp 4 * 1024 * 1024 - 2
 *			1001 0101 1111 1101
 *				9			4			f		 d
 *			1111 1111 1111 1110
 *			 	f 	 	 f   	f  	 e
 */
void test_AvrOperatorTable_given_jmp_4x1024x1024minus2(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
	  // start:
		0x03, 0x95,														// inc r16
		// here:
		0xfe, 0xcf,															// rjmp start
		0x12, 0x2c,															// mov r1, r2
		0xfd, 0x95, 0xfe, 0xff,				// jmp 4 * 1024 *1024 -2
	};
	uint8_t *progCounter = &codeMemory[6];
	flash = codeMemory;

	k = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_INT32((0x3ffffe*2)-0x6, k);
}

/**
 * Instruction:
 * 		BRBS s, k
 *			1111 00kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbs 4, 61
 *			1111 0001 1110 1100
 *			 f 	  1    e  	c
 */
void test_AvrOperatorTable_given_brbs_4_61(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
		0xec, 0xf1,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

  sreg->S = 1;
	k = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_INT32(124, k);
}

/**
 * Instruction:
 * 		BRBS s, k
 *			1111 00kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbs 5, 61
 *			1111 0001 1110 1101
 *			 f 	  1    e  	d
 */
void test_AvrOperatorTable_given_brbs_5_61(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
		0xed, 0xf1,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

  sreg->H = 0;
	k = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_INT32(2, k);
}

/**
 * Instruction:
 * 		BRBC s, k
 *			1111 01kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbs 5, 61
 *			1111 0101 1110 1101
 *			 f 	  5    e  	d
 */
void test_AvrOperatorTable_given_brbc_5_61(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
		0xed, 0xf5,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

  sreg->H = 0;
	k = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_INT32(124, k);
}

/**
 * Instruction:
 * 		BRBC s, k
 *			1111 01kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbs 4, 61
 *			1111 0101 1110 1100
 *			 f 	  5    e  	c
 */
void test_AvrOperatorTable_given_brbc_4_61(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
		0xec, 0xf5,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

  sreg->S = 1;
	k = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_INT32(2, k);
}

/**
 * Instruction:
 * 		NOP None
 *			0000 0000 0000 0000
 *       0    0    0    0
 */
void test_AvrOperatorTable_given_nop(void)
{
	uint8_t pc;
	uint8_t codeMemory[] = {
		0x00, 0x00,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

	pc = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(2, pc);
}

/**
 * Instruction:
 * 		WDR None
 *			1001 0101 1010 1000
 *       9    5    a    8
 */
void test_AvrOperatorTable_given_wdr(void)
{
  CEXCEPTION_T e;
	uint8_t codeMemory[] = {
		0xa8, 0x95,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

	Try {
    simulateOneInstruction(progCounter);
    TEST_FAIL_MESSAGE("Expect WATCHDOG_EXCEPTION. But no exception thrown.");
  } Catch(e) {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(WATCHDOG_EXCEPTION, e->errorCode);
    freeError(e);
  }
}

/**
 * Instruction:
 * 		BREAK None
 *			1001 0101 1001 1000
 *       9    5    9    8
 */
void test_AvrOperatorTable_given_break(void)
{
  CEXCEPTION_T e;
	uint8_t codeMemory[] = {
		0x98, 0x95,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

  Try {
    simulateOneInstruction(progCounter);
    TEST_FAIL_MESSAGE("Expect BREAK_EXCEPTION. But no exception thrown.");
  } Catch(e) {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(BREAK_EXCEPTION, e->errorCode);
    freeError(e);
  }
}

/**
 * Instruction:
 * 		SLEEP None
 *			1001 0101 1000 1000
 *       9    5    8    8
 */
void test_AvrOperatorTable_given_sleep(void)
{
  CEXCEPTION_T e;
	uint8_t codeMemory[] = {
		0x88, 0x95,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

  Try {
    simulateOneInstruction(progCounter);
    TEST_FAIL_MESSAGE("Expect SLEEP_EXCEPTION. But no exception thrown.");
  } Catch(e) {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(SLEEP_EXCEPTION, e->errorCode);
    freeError(e);
  }
}

/**
 * Instruction:
 * 		CP Rd, Rr
 *			0001 01rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *      0 <= rrrrr <= 31
 *
 * Simulate cp R4, R21
 *			0001 0110 0100 0101
 *			 1 	  6    4  	5
 */
void test_AvrOperatorTable_given_cp_r4_r21(void)
{
  uint8_t codeMemory[] = {
		0x45, 0x16,
	};
	uint8_t *progCounter = codeMemory;

	r[4]  = 0x9c;
	r[21] = 0x52;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
	TEST_ASSERT_EQUAL(1, sreg->H);
}

/**
 * Instruction:
 * 		CPC Rd, Rr
 *			0000 01rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *      0 <= rrrrr <= 31
 *
 * Simulate cpc R17, R23
 *			0000 0111 0001 0111
 *			 0 	  7    1  	7
 */
void test_AvrOperatorTable_given_cpc_r17_r23(void)
{
  uint8_t codeMemory[] = {
		0x17, 0x07,
	};
	uint8_t *progCounter = codeMemory;

	r[17] = 0x9c;
	r[23] = 0x52;
	sreg->C = 0;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
	TEST_ASSERT_EQUAL(1, sreg->H);
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
 *
 * Simulate cpi R18, 211
 *			0011 1101 0010 0011
 *			 3 	  d    2  	3
 */
void test_AvrOperatorTable_given_cpi_r18_211(void)
{
  uint8_t codeMemory[] = {
		0x23, 0x3d,
	};
	uint8_t *progCounter = codeMemory;

	r[18] = 0x9c;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
	TEST_ASSERT_EQUAL(1, sreg->H);
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
 *
 * Simulate ldi R31, 211
 *			1110 1101 1111 0011
 *			 e 	  d    f  	3
 */
void test_AvrOperatorTable_given_cpi_r31_211(void)
{
  uint8_t codeMemory[] = {
		0xf3, 0xed,
	};
	uint8_t *progCounter = codeMemory;

	simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(211, r[31]);
}
