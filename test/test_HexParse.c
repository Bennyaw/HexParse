#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "unity.h"
#include "HexParse.h"
#include "Error.h"
#include "Exception.h"
#include "CException.h"

#define k 1024

int *baseMemory = malloc(128*k);

void setUp(void){}

void tearDown(void){}

void test_openFile()
{
  openFile();
}

void test_readFile(void)
{
  readFile();
}
/*------------test for code working properly-----------------*/

void test_checkColon_check_for_colon_sign_return_true(void){
    char *line = ":";

    TEST_ASSERT_TRUE(checkColon(&line));
}


void test_getByteCount_given_byte_count_10_hex_convert_into_decimal(void)
{
  char *line = "10";

  TEST_ASSERT_EQUAL(16,getByteCount(&line));
}

void test_getByteCount_given_byte_count_1a_hex_convert_into_decimal(void)
{
  char *line = "1a";

  TEST_ASSERT_EQUAL(26,getByteCount(&line));
}

void test_getByteCount_given_byte_count_0B_hex_convert_into_decimal(void)
{
  char *line = "0B";

  TEST_ASSERT_EQUAL(11,getByteCount(&line));
}

void test_extractAddress_given_000A(void)
{
  char *line = "000A";
  uint16_t addressReturn = extractAddress(line);
  TEST_ASSERT_EQUAL(0x000A,extractAddress(line));
}

void test_extractAddress_given_ffff_return_cap_letter(void)
{
  char *line = "ffff";
  uint16_t addressReturn = extractAddress(line);
  TEST_ASSERT_EQUAL(0xFFFF,addressReturn);
}

void test_extractRecordType_given_00(void)
{
  char *line = "05";
  int recordTypeReturn = extractRecordType(line);
  TEST_ASSERT_EQUAL(5,recordTypeReturn);
}
/*
void test_extractData_given_FFFF_return_true(void)
{
  char *line = "FF";
  int byteCount = 2;
  int dataReturn = extractData(line,byteCount);

  TEST_ASSERT_EQUAL(0xFF,dataReturn);
}
*/
void test_extractData_given_FF3C_and_extract_and_locate_properly_in_memory(void)
{
  int *memory_test;
  char *line = "FF3C";
  int byteCount = 2;

  memory_test = extractData(line,byteCount);

  TEST_ASSERT_EQUAL(0xFF,memory_test[0]);
  TEST_ASSERT_EQUAL(0x3C,memory_test[1]);
}
/*
void test_verifyHexLine_with_16_byte_data_return_true(void)
{
  char *line = ":10010000214601360121470136007EFE09D2190140";

  TEST_ASSERT_TRUE(verifyHexLine(&line));
}
*/
/*----------------------test with exception-----------------------*/

void test_checkColon_without_colon_sign_and_throw_ERR_COLON_MISSING(void)
{
  CEXCEPTION_T e;
  char *line = "02000004FFFFFC";

  Try{
    checkColon(&line);
    TEST_FAIL_MESSAGE("Expect ERR_COLON_MISSING. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_COLON_MISSING, e->errorCode);
    freeError(e);
  }
}

void test_verifyHexLine_with_single_byte_data_error_and_throw_ERR_DATA_CORRUPTED(void)
{
  CEXCEPTION_T e;
  char *line = ":10010000214601360121470136007EFE09E2190140";
//should be :10010000214601360121470136007EFE09(D)2190140,
  Try{
    verifyHexLine(&line);
    TEST_FAIL_MESSAGE("Expect ERR_DATA_CORRUPTED. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_DATA_CORRUPTED, e->errorCode);
    freeError(e);
  }
}

void test_verifyHexLine_with_space_in_hex_line_and_throw_ERR_UNKNOWN_DATA(void)
{
  CEXCEPTION_T e;
  char *line = ":1001000021460 360121470136007EFE09D2190140";

  Try{
    verifyHexLine(&line);
    TEST_FAIL_MESSAGE("Expect ERR_UNKNOWN_DATA. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_UNKNOWN_DATA, e->errorCode);
    freeError(e);
  }
}

void test_verifyHexLine_without_colon_and_throw_ERR_COLON_MISSING(void)
{
  CEXCEPTION_T e;
  char *line = "1001000021460360121470136007EFE09D2190140";

  Try{
    verifyHexLine(&line);
    TEST_FAIL_MESSAGE("Expect ERR_UNKNOWN_DATA. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_COLON_MISSING, e->errorCode);
    freeError(e);
  }
}

void test_extractRecordType_with_56_and_throw_ERR_UNKNOWN_RECORD_TYPE(void)
{
  CEXCEPTION_T e;
  char *line = "56";

  Try{
    extractRecordType(line);
    TEST_FAIL_MESSAGE("Expect ERR_UNKNOWN_RECORD_TYPE. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_UNKNOWN_RECORD_TYPE, e->errorCode);
    freeError(e);
  }
}

void test_extractAddress_with_unregconised_data_and_throw_ERR_UNKNOWN_DATA(void)
{
  CEXCEPTION_T e;
  char *line = "000Z";

  Try{
    extractAddress(line);
    TEST_FAIL_MESSAGE("Expect ERR_UNKNOWN_DATA. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_UNKNOWN_DATA, e->errorCode);
    freeError(e);
  }
}

void test_getByteCount_with_unregconised_data_and_throw_ERR_UNKNOWN_DATA(void)
{
  CEXCEPTION_T e;
  char *line = "1P";

  Try{
    getByteCount(&line);
    TEST_FAIL_MESSAGE("Expect ERR_UNKNOWN_DATA. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_UNKNOWN_DATA, e->errorCode);
    freeError(e);
  }
}

void test_interpretHexLine(void)
{

  char *line = ":0B0010006164647265737320676170A7";

  hexParse(line);
}
