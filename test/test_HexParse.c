#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "unity.h"
#include "HexParse.h"
#include "Error.h"
#include "Exception.h"
#include "CException.h"

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

void test_getByteCount_given_byte_count_0B_hex_convert_into_decimal(void)
{
  char *line = "0B";

  TEST_ASSERT_EQUAL(11,getByteCount(&line));
}

void test_getByteCount_given_byte_count_20_hex_convert_into_decimal(void)
{
  char *line = "20";

  TEST_ASSERT_EQUAL(32,getByteCount(&line));
}
void test_getByteCount_given_byte_count_00_hex_convert_into_decimal(void)
{
  char *line = "00";

  TEST_ASSERT_EQUAL(00,getByteCount(&line));
}

void test_getByteCount_given_byte_count_1a_hex_convert_into_decimal(void)
{
  char *line = "1a";

  TEST_ASSERT_EQUAL(26,getByteCount(&line));
}

void test_extractAddress_given_0000(void)
{
  char *line = "0000";
  char *addressReturn = extractAddress(line);
  TEST_ASSERT_EQUAL_STRING("0000",addressReturn);
}

void test_extractAddress_given_0012(void)
{
  char *line = "0012";
  char *addressReturn = extractAddress(line);
  TEST_ASSERT_EQUAL_STRING("0012",addressReturn);
}

void test_extractAddress_given_000A(void)
{
  char *line = "000A";
  char *addressReturn = extractAddress(line);
  TEST_ASSERT_EQUAL_STRING("000A",addressReturn);
}

void test_extractAddress_given_ffff_return_cap_letter(void)
{
  char *line = "ffff";
  char *addressReturn = extractAddress(line);
  TEST_ASSERT_EQUAL_STRING("FFFF",addressReturn);
}

void test_extractRecordType_given_00(void)
{
  char *line = "00";
  char *recordTypeReturn = extractRecordType(line);
  TEST_ASSERT_EQUAL_STRING("00",recordTypeReturn);
}

void test_extractRecordType_given_02(void)
{
  char *line = "02";
  char *recordTypeReturn = extractRecordType(line);
  TEST_ASSERT_EQUAL_STRING("02",recordTypeReturn);
}

void test_extractRecordType_given_04(void)
{
  char *line = "04";
  char *recordTypeReturn = extractRecordType(line);
  TEST_ASSERT_EQUAL_STRING("04",recordTypeReturn);
}

void test_extractData_given_FFFF_without_checksum_yet(void)
{
  char *line = ":02000004FFFF";
  char *dataReturn = extractData(line);
  TEST_ASSERT_EQUAL_STRING("FFFF",dataReturn);
}

void test_verifyHexLine_with_16_byte_data_return_true(void)
{
  char *line = ":10010000214601360121470136007EFE09D2190140";

  TEST_ASSERT_TRUE(verifyHexLine(&line));
}

/*----------------------test with exception-----------------------*/
void test_checkColon_without_colon_sign_and_throw_ERR_COLON_MISSING(void)
{
  CEXCEPTION_T e;
  char *line = "02000004FFFF";

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

void test_verifyHexLine_with_unregconised_data_and_throw_ERR_UNREGCONISED_DATA(void)
{
  CEXCEPTION_T e;
  char *line = ":1001000021460 360121470136007EFE09E2190140";

  Try{
    verifyHexLine(&line);
    TEST_FAIL_MESSAGE("Expect ERR_UNREGCONISED_DATA. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_UNREGCONISED_DATA, e->errorCode);
    freeError(e);
  }
}
