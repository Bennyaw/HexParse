#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "unity.h"
#include "HexParse.h"
#include "Error.h"
#include "Exception.h"
#include "CException.h"

#define k 1024

uint8_t flashMemory[256*k];//256*1024

void setUp(void){
  initHexParser();
}

void tearDown(void){}
/*
void test_openFile()
{
  openFile();
}
*/

/*--------------exampleHex.hex----------------
*:10C00000576F77212044696420796F7520726561CC
*:10C010006C6C7920676F207468726F756768206137
*:10C020006C6C20746869732074726F75626C652023
*:10C03000746F207265616420746869732073747210
*:04C040007696E673FF
*:00000001FF
*/

void test_readFile(void)
{
  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/exampleHex.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }

  hexLineRead = readFile(fp);
  //printf("In test function : %s", hexLineRead);
  TEST_ASSERT_EQUAL_STRING(":10C00000576F77212044696420796F7520726561CC",hexLineRead);
}

void test_readFile_read_3rd_hex_line_from_hex_file(void)
{
  FILE *fp;
  char *hexLineRead;
  int i;
  fp = fopen("data/test/exampleHex.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }
  for(i = 0; i < 3;i++)
  {
    hexLineRead = readFile(fp);
  }

  //printf("In test function : %s", hexLineRead);
  TEST_ASSERT_EQUAL_STRING(":10C020006C6C20746869732074726F75626C652023",hexLineRead);
}

void test_readFile_read_every_hex_line_from_hex_file(void)
{
  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/exampleHex.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }

  while((hexLineRead = readFile(fp)) != NULL)
  {
    printf("In test function : %s\n", hexLineRead);
  }
}

void test_readFile_from_non_existing_file_and_print_error_msg(void)
{
  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/noThisFile.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }

  while((hexLineRead = readFile(fp)) != NULL)
  {
    printf("In test function : %s\n", hexLineRead);
  }
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
  uint16_t addressReturn = extractAddress(&line);
  TEST_ASSERT_EQUAL(0x000A,addressReturn);
}

void test_extractAddress_given_ffff_return_cap_letter(void)
{
  char *line = "ffff";
  uint16_t addressReturn = extractAddress(&line);
  TEST_ASSERT_EQUAL(0xFFFF,addressReturn);
}

void test_extractRecordType_given_00(void)
{
  char *line = "05";
  int recordTypeReturn = extractRecordType(&line);
  TEST_ASSERT_EQUAL(5,recordTypeReturn);
}


void test_hexParse_read_1st_hex_line_from_file_and_extract_data_correctly(void)
{
  uint8_t expectedData[] = {
    0x57, 0x6f, 0x77, 0x21, 0x20, 0x44, 0x69, 0x64,
    0x20, 0x79, 0x6F, 0x75, 0x20, 0x72, 0x65, 0x61
    };

  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/exampleHex.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }

  hexLineRead = readFile(fp);

  hexParse(hexLineRead,flashMemory);

  TEST_ASSERT_EQUAL_STRING(":10C00000576F77212044696420796F7520726561CC",hexLineRead);
  TEST_ASSERT_EQUAL_MEMORY(expectedData,&flashMemory[0xC000],16);
}

void test_hexParse_read_all_hex_line_from_file_and_return_true(void)
{
  char *hexFile[] = {
    ":10C00000576F77212044696420796F7520726561CC",
    ":10C010006C6C7920676F207468726F756768206137",
    ":10C020006C6C20746869732074726F75626C652023",
    ":10C03000746F207265616420746869732073747210",
    ":04C040007696E67397",
    ":00000001FF"
  };

  int i = 0;
  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/exampleHex.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }

  while((hexLineRead = readFile(fp)) != NULL)
  {
    TEST_ASSERT_EQUAL_STRING(hexFile[i],hexLineRead);
    i++;
  }
}

void test_hexParse_read_assemblerApp_related_to_segmentAddress_file_and_return_true(void)
{
  /**----------------assemblerApp.hex------------------
   *:020000022BC011
   *:1012340054686973207061727420697320696E2028
   *:00000001FF
   *-------------------------------------------------**/
   uint8_t expectedData[] = {
     0x54, 0x68, 0x69, 0x73, 0x20, 0x70, 0x61,
     0x72, 0x74, 0x20, 0x69, 0x73, 0x20, 0x69,
     0x6E, 0x20
   };

  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/assemblerAppSeg.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }

  while((hexLineRead = readFile(fp)) != NULL)
  {
    hexParse(hexLineRead,flashMemory);
  }
  TEST_ASSERT_EQUAL_MEMORY(expectedData,&flashMemory[0x2CE34],16);
}

void test_hexParse_read_assemblerApp_related_to_lineartAddress_file_and_return_true(void)
{
  /**----------------assemblerApp.hex------------------
   *:020000040003F7
   *:1012340054686973207061727420697320696E2028
   *:00000001FF
   *-------------------------------------------------**/
   uint8_t expectedData[] = {
     0x54, 0x68, 0x69, 0x73, 0x20, 0x70, 0x61,
     0x72, 0x74, 0x20, 0x69, 0x73, 0x20, 0x69,
     0x6E, 0x20
   };

  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/assemblerApp.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }

  while((hexLineRead = readFile(fp)) != NULL)
  {
    hexParse(hexLineRead,flashMemory);
  }
  TEST_ASSERT_EQUAL_MEMORY(expectedData,&flashMemory[0x31234],16);
}

void test_hexParse_read_assemblerApp_related_to_lineartAddress_and_segmentAddress_file_and_return_true(void)
{
  /**----------------linearAndSegment.hex------------------
   *:10C00000576F77212044696420796F7520726561CC
   *:020000040002F8
   *:1012340054686973207061727420697320696E2028
   *:020000022BC011
   *:1012340054686973207061727420697320696E2028
   *:00000001FF
   *-------------------------------------------------**/
   uint8_t expectedData00Type[] = {
     0x57, 0x6f, 0x77, 0x21, 0x20, 0x44, 0x69, 0x64,
     0x20, 0x79, 0x6F, 0x75, 0x20, 0x72, 0x65, 0x61
     };

   uint8_t expectedData04Type[] = {
     0x54, 0x68, 0x69, 0x73, 0x20, 0x70, 0x61,
     0x72, 0x74, 0x20, 0x69, 0x73, 0x20, 0x69,
     0x6E, 0x20
   };

   uint8_t expectedData02Type[] = {
     0x54, 0x68, 0x69, 0x73, 0x20, 0x70, 0x61,
     0x72, 0x74, 0x20, 0x69, 0x73, 0x20, 0x69,
     0x6E, 0x20
   };

  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/linearAndSegment.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }

  while((hexLineRead = readFile(fp)) != NULL)
  {
    hexParse(hexLineRead,flashMemory);
  }
  TEST_ASSERT_EQUAL_MEMORY(expectedData00Type,&flashMemory[0xC000],16);
  TEST_ASSERT_EQUAL_MEMORY(expectedData04Type,&flashMemory[0x21234],16);
  TEST_ASSERT_EQUAL_MEMORY(expectedData02Type,&flashMemory[0x2CE34],16);
}

void test_hexParse_read_recordtype_03_and_save_in_global_variable_start32BitAddress(void)
{
  char *line = ":0400000300003800C1";

  hexParse(line,flashMemory);
}
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

void test_verifyHexLine_with_double_colon_in_hex_line_and_throw_ERR_UNKNOWN_DATA(void)
{
  CEXCEPTION_T e;
  char *line = ":：1001000021460360121470136007EFE09D2190140";

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
    TEST_FAIL_MESSAGE("Expect ERR_COLON_MISSING. But no exception thrown.");
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
    extractRecordType(&line);
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
    extractAddress(&line);
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

void test_hexParse_with_hex_line_read_from_file_1st_line_with_a_space_throw_ERR_UNKNOWN_DATA(void)
{
  /*--------------testErrorData.hex----------------
   *:10C00000576F77212044696420796F752072656 1CC
   *:10C010006C6C7920676F207468726F756768206137
   *10C020006C6C20746869732074726F75626C652023
   *:10C03000746F207265616420746869732073747210
   *:04C040007696E673FF
   *:00000001FF
   */
  CEXCEPTION_T e;
  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/testErrorData.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }
  Try{
  hexLineRead = readFile(fp);
  hexParse(hexLineRead,flashMemory);
  TEST_FAIL_MESSAGE("Expect ERR_UNKNOWN_DATA. But no exception thrown.");
  }
  Catch(e)
  {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_UNKNOWN_DATA, e->errorCode);
    freeError(e);
  }
}

void test_readFile_read_3rd_hex_line_from_hex_file_and_throw_ERR_COLON_MISSING(void)
{
  /*--------------testErrorColon.hex----------------
   *:10C00000576F77212044696420796F7520726561CC
   *:10C010006C6C7920676F207468726F756768206137
   *10C020006C6C20746869732074726F75626C652023
   *:10C03000746F207265616420746869732073747210
   *:04C040007696E673FF
   *:00000001FF
   */
  CEXCEPTION_T e;
  FILE *fp;
  int i;
  char *hexLineRead;
  fp = fopen("data/test/testErrorColon.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }
  Try{
    for(i = 0; i < 3;i++)
    {
      hexLineRead = readFile(fp);
    }
    hexParse(hexLineRead,flashMemory);
    TEST_FAIL_MESSAGE("Expect ERR_COLON_MISSING. But no exception thrown.");
  }
  Catch(e)
  {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_COLON_MISSING, e->errorCode);
    freeError(e);
  }

}

void test_readFile_read_hex_line_from_hex_file_and_throw_ERR_UNKNOWN_RECORD_TYPE(void)
{
  /*--------------testErrorRecordType.hex----------------
   *:10C03000746F207265616420746869732073747210
   *:020000100000EE
   *:00000001FF
   */
  CEXCEPTION_T e;
  FILE *fp;
  int i;
  char *hexLineRead;
  fp = fopen("data/test/testErrorRecordType.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }
  Try{
    while((hexLineRead = readFile(fp)) != NULL)
    {
      hexParse(hexLineRead,flashMemory);
    }
    TEST_FAIL_MESSAGE("Expect ERR_UNKNOWN_RECORD_TYPE. But no exception thrown.");
  }
  Catch(e)
  {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_UNKNOWN_RECORD_TYPE, e->errorCode);
    freeError(e);
  }

}

void test_readFile_read_invalid_hex_line__after_eof_from_hex_file_and_throw_ERR_INVALID_INSTRUCTION_AFTER_EOF(void)
{
  /**--------------testErrorInvalidInstructionAfterEOF.hex----------------
   *:10C00000576F77212044696420796F7520726561CC
   *:10C010006C6C7920676F207468726F756768206137
   *:10C020006C6C20746869732074726F75626C652023
   *:10C03000746F207265616420746869732073747210
   *:00000001FF
   *:04C040007696E673FF
   */
  CEXCEPTION_T e;
  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/testErrorInvalidInstructionAfterEOF.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }
  Try{
    while((hexLineRead = readFile(fp)) != NULL)
    {
      hexParse(hexLineRead,flashMemory);
    }
    TEST_FAIL_MESSAGE("Expect ERR_INVALID_INSTRUCTION_AFTER_EOF. But no exception thrown.");
  }
  Catch(e)
  {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_INVALID_INSTRUCTION_AFTER_EOF, e->errorCode);
    freeError(e);
  }

}

void test_readFile_read_not_not_equal_number_data_and_throw_ERR_NUMBER_OF_DATA_MISMATCHED(void)
{
  /**--------------testErrorInvalidInstructionAfterEOF.hex----------------
   *:02000004000301F6
   *:1012340054686973207061727420697320696E2028
   *:00000001FF
   */
  CEXCEPTION_T e;
  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/testNumberOfData.hex","r");

  if(fp == NULL){
    perror("Error opening file");
  }
  Try{
    while((hexLineRead = readFile(fp)) != NULL)
    {
      hexParse(hexLineRead,flashMemory);
    }
    TEST_FAIL_MESSAGE("Expect ERR_NUMBER_OF_DATA_MISMATCHED. But no exception thrown.");
  }
  Catch(e)
  {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_NUMBER_OF_DATA_MISMATCHED, e->errorCode);
    freeError(e);
  }

}

void test_loadHexFile_read_hex_file_and_load_data_into_memory(void)
{
  uint8_t expectedData[] = {
    0x54, 0x68, 0x69, 0x73, 0x20, 0x70, 0x61,
    0x72, 0x74, 0x20, 0x69, 0x73, 0x20, 0x69,
    0x6E, 0x20
  };

  CEXCEPTION_T e;
  char *fileNameDirectory = "data/test/assemblerAppSeg.hex";

  loadHexFile(fileNameDirectory,flashMemory,256*k);
  TEST_ASSERT_EQUAL_MEMORY(expectedData,&flashMemory[0x2CE34],16);

}

void test_loadHexFile_read_testNumberOfData_hex_file_and_throw_ERR_NUMBER_OF_DATA_MISMATCHED(void)
{
  CEXCEPTION_T e;
  char *fileNameDirectory = "data/test/testNumberOfData.hex";

  Try{
    loadHexFile(fileNameDirectory,flashMemory,256*k);
    TEST_FAIL_MESSAGE("Expect ERR_NUMBER_OF_DATA_MISMATCHED. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_NUMBER_OF_DATA_MISMATCHED, e->errorCode);
    freeError(e);
  }
}

void test_loadHexFile_read_testInvalidInstructionAfterEOF_hex_file_and_throw_ERR_INVALID_INSTRUCTION_AFTER_EOF(void)
{
  CEXCEPTION_T e;
  char *fileNameDirectory = "data/test/testErrorInvalidInstructionAfterEOF.hex";

  Try{
    loadHexFile(fileNameDirectory,flashMemory,256*k);
    TEST_FAIL_MESSAGE("Expect ERR_INVALID_INSTRUCTION_AFTER_EOF. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_INVALID_INSTRUCTION_AFTER_EOF, e->errorCode);
    freeError(e);
  }
}

void test_loadHexFile_read_testInvalidInstructionAfterEOF_hex_file_and_throw_ERR_MISSING_EOF(void)
{
  CEXCEPTION_T e;
  char *fileNameDirectory = "data/test/testMissingEOF.hex";

  Try{
    loadHexFile(fileNameDirectory,flashMemory,256*k);
    TEST_FAIL_MESSAGE("Expect ERR_MISSING_EOF. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_MISSING_EOF, e->errorCode);
    freeError(e);
  }

}

void test_loadHexFile_bufferSize_given_too_small_and_throw_ERR_BUFFER_SIZE(void)
{
  CEXCEPTION_T e;
  char *fileNameDirectory = "data/test/assemblerAppSeg.hex";

  Try{
    loadHexFile(fileNameDirectory,flashMemory,4*k);
    TEST_FAIL_MESSAGE("Expect ERR_BUFFER_SIZE. But no exception thrown.");
  }
  Catch(e){
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(ERR_BUFFER_SIZE, e->errorCode);
    freeError(e);
  }

}
