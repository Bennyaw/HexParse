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
//#include "Error.h"
#include "ErrorSimulator.h"
#define k 1024

uint8_t flashMemory[256*k];

void setUp(void)
{
}

void tearDown(void)
{
}

void test_HexParseAndSimulator_add_r16_r17_(void)
{
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
   simulate(flashMemory);
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
