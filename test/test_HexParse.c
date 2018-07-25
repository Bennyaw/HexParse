#include <stdio.h>
#include <stdlib.h>
#include "unity.h"
#include "HexParse.h"
void setUp(void){}

void tearDown(void){}

void test_hexParser_check_for_colon_sign_return_true(void){
    char *line = ":";

    TEST_ASSERT_TRUE(hexParser(&line));
}

void test_hexParser_given_byte_count_10_hex_convert_into_decimal(void)
{
  char *line = ":10";

  TEST_ASSERT_EQUAL(16,getByteCount(&line));
}

void test_hexParser_given_byte_count_0B_hex_convert_into_decimal(void)
{
  char *line = ":0B";

  TEST_ASSERT_EQUAL(11,getByteCount(&line));
}

void test_hexParser_given_byte_count_20_hex_convert_into_decimal(void)
{
  char *line = ":20";

  TEST_ASSERT_EQUAL(32,getByteCount(&line));
}
void test_hexParser_given_byte_count_00_hex_convert_into_decimal(void)
{
  char *line = ":00";

  TEST_ASSERT_EQUAL(00,getByteCount(&line));
}

void test_hexParser_given_byte_count_1a_hex_convert_into_decimal(void)
{
  char *line = ":1a";

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

void test_extractAddress_given_ffff(void)
{
  char *line = "ffff";
  char *addressReturn = extractAddress(line);
  TEST_ASSERT_EQUAL_STRING("ffff",addressReturn);
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
