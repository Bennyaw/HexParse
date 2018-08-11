#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "HexParse.h"
#include "Error.h"
#include "Exception.h"
#include "CException.h"

#define k 1024

uint32_t segmentAddress;
uint32_t linearAddress;
uint32_t start32BitAddress;
int enableSegmentAddress = 0;
int enableLinearAddress = 0;

/*    all printf is to debug code, for checing purpose  */
/* -----------------------RECORD TYPE---------------------
*   00 - data
*   01 - End Of FILE
*   02 - Extended Segment Address
*   03 - Start Segment Address
*   04 - Extended Linear Address
*   05 - Start Linear Address
*----------------------------------------------------------*/


/* ------------------------Hex Format---------------------
*   :llaaaatt[dd...]cc
* : is colone that starts every Intel HEX recordType
*
* ll is the record-length field that represents the number of data bytes (dd) in the record.
*
* aaaa is the address field that represents the starting address
* for subsequent data in the record.
*
* tt is the field that represents the HEX record type, which may be one of the following:
*
* dd is a data field that represents one byte of data. A record may have multiple
* data bytes. The number of data bytes in the record must match the number specified by the ll field.
*
* cc is the checksum field that represents the checksum of the record.
* The checksum is calculated by summing the values of all hexadecimal digit pairs in the record
* modulo 256 and taking the two's complement.
* -------------------------------------------------------*/

//-----------------testing open file--------------------
int openFile(void)
{
  FILE * fp;

  fp = fopen ("myfile.txt", "w+");
  fprintf(fp, "%s %d", "we are in ", 2018);

  fclose(fp);

  return(0);
}

char *readFile(FILE *fileLocation)
{
  int hexLinelength;
  char *hexLine = malloc(k);
  if(fgets (hexLine, k, fileLocation) != NULL){
    hexLinelength = strlen(hexLine);
    hexLine[hexLinelength-1] = '\0';//replace '\n' at the end of the line
  }
  else{
    return NULL;
  }
  //printf("hexLine[%d] : %c\n",43,hexLine[43] );
  //printf("hexLinelength : %d\n",hexLinelength );
  //printf("In readFile function : %s\n", hexLine);
  return hexLine;
}
//----------------------------------------------------------

//-------------functions for analyzing hex line-------------
int checkColon(char **linePtr)
{
  if(**linePtr == ':')
  {
    return 1;
  }
  else
  {
    throwSimpleError(ERR_COLON_MISSING,"Error : ':' is missing");
    return 0;
  }
}

int getByteCount(char **linePtr)
{
  int decimal = 0,count = 0;
  int base = 1;
  int power = 1;

  while(count<2)
  {
    base = (int)pow((double)16,power);  //16^1
    decimal = convertHexToDec(linePtr, decimal, power, base);
    power--; //decrement power
    count++;
  }

  return decimal;
}

int extractAddress (char **linePtr)
{
  int decimal = 0,count = 0;
  int base = 1;
  int power = 3;
  int intaddress = 0;

  while(count<4)
  {
    base = (int)pow((double)16,power);  //start with 16^3
    intaddress = convertHexToDec(linePtr, intaddress, power, base);
    power--;  //decrement power
    count++;
  }
  return intaddress;
}

uint64_t extract4BytesAddress(char **linePtr)
{
  int decimal = 0,count = 0;
  int base = 1;
  int power = 7;
  uint64_t intaddress = 0;

  while(count<8)
  {
    base = (int)pow((double)16,power);  // start with 16^7
    intaddress = convertHexToDec(linePtr, intaddress, power, base);
    power--;  //decrement power
    count++;
  }
  return intaddress;
}

int extractRecordType(char **linePtr)
{
  int decimal = 0,count = 0;
  int base = 1;
  int power = 1;
  int intRecordType = 0;
  int errorFlag = 0;

  while(count<2){

    if(**linePtr <'0' || **linePtr >'5')//check if is in 0-5
    {
      errorFlag = 1;
    }

    base = (int)pow((double)16,power);  //16^3
    intRecordType = convertHexToDec(linePtr, intRecordType, power, base);
    power--;  //decrement power
    count++;
  }

  if(errorFlag == 1)
  {
    throwError(ERR_UNKNOWN_RECORD_TYPE,"Error : Unknown recordtype digit '%x' found in the line.",intRecordType);
  }
  else
  {
    return intRecordType;
  }

}

int verifyHexLine(char **linePtr)
{
  uint16_t getData = 0,addData = 0;
  uint8_t  verifyData;

  if(checkColon(linePtr))
  {
    while(**linePtr == ':')
    {
      (*linePtr)++;
    }

    while(**linePtr != ':')//loop until next hexline
    {
      if(**linePtr == '\0')//check whether reach end of line
      {
        break;
      }
      else
      {
        getData = getByteCount(linePtr);//get 2 byte count
        addData += getData ; //keep adding untill last hex digit
      }
    }

    //printf("verifyData(non-zero means error data) : %u\n", verifyData);//checking hex digit whether is 00

    if((addData & 0xff) == 0)//if not zero then is error data
    {
      return 1;
    }
    else
    {
      throwError(ERR_DATA_CORRUPTED,"Error : Data corrupted by %d in decimal",verifyData);
      return 0;
    }

  }
  else
  {
    printf("In function verifyHexLine, checkColon fail.\n");
    return 0;
  }
}

int convertHexToDec(char **linePtr, int decimal, int p, int base)
{
  char errorChar;

  if(**linePtr >= '0' && **linePtr <= '9')//convert to int from hex
  {
    decimal = decimal + (**linePtr - 48)*base;
    (*linePtr)++; //move to next digit
  }
  else if((toupper(**linePtr)) >= 'A' && (toupper(**linePtr)) <= 'F')//convert to int from hex
  {
    decimal = decimal + (toupper(**linePtr) - 55)*base;
    (*linePtr)++; //move to next hexdigit
  }
  else
  {
    //printf("errorChar : %c\n", **linePtr);
    throwError(ERR_UNKNOWN_DATA,"Error : Unknown data '%c' found in the line.",**linePtr);
  }

  return decimal;
}

//---------------------main function--------------------------
uint8_t *hexParse(char *linePtr)
{
  HexRecordStructure HexRecordStructure;
  char *ptrForVerify = linePtr;

  if (verifyHexLine(&ptrForVerify))
  {
    while(*linePtr == ':')
    {
      linePtr++;//move pointer to address
    }

    HexRecordStructure.byteCount = getByteCount(&linePtr);//get size of data
    HexRecordStructure.address = extractAddress(&linePtr);
    HexRecordStructure.recordType = extractRecordType(&linePtr);//error thrown in the function
// linePtr now is pointing to data field

  if(HexRecordStructure.recordType == 2)
  {
    enableSegmentAddress = 1;
    enableLinearAddress = 0;
  }
  else if(HexRecordStructure.recordType == 4)
  {
    enableSegmentAddress = 0;
    enableLinearAddress = 1;
  }

  interpretHexLine(linePtr,HexRecordStructure);

  return flashMemory;
  }
  else
  {
    return 0;//error thrown inside verifyHexLine function
  }
}

void interpretHexLine(char *linePtr, HexRecordStructure HexRecordStructure)
{
  uint32_t tempAddress;
  uint32_t tempAddress2;

  switch(HexRecordStructure.recordType){
    case 0: loadData(linePtr,HexRecordStructure);
      break;
    case 1: enableLinearAddress = 0;
            enableSegmentAddress = 0;
            exit -1;
      break;
    case 2: segmentAddress = extractAddress(&linePtr) * 0x10;//extended segment address is recorded in data field
      break;
    case 4: linearAddress = extractAddress(&linePtr) * 0x10000;//extended linear address is recorded in data field
      break;
    case 3:
    case 5: tempAddress = extractAddress(&linePtr) * 0x10000;
            tempAddress2 = extractAddress(&linePtr);
            start32BitAddress = tempAddress + tempAddress2;//extract 32 bit address and save it in global variable
            printf("32BitAddress : %x\n", start32BitAddress);
      break;
  }

}

void loadData(char *linePtr,HexRecordStructure HexRecordStructure)
{

  int byteCount = HexRecordStructure.byteCount;//for loading every two bytes in to memory

  while(byteCount!=0)
  {
    int count = 0;
    int base = 1;
    int power = 1;
    int data = 0;

    while(count<2)
    {
      base = (int)pow((double)16,power);  //16^3
      data = convertHexToDec(&linePtr, data, power, base);
      power--;  //decrement power
      count++;
    }

    if(enableSegmentAddress == 1)
    {
      flashMemory[segmentAddress + HexRecordStructure.address] = data;//load data in to memory
      printf("flashMemory[%x] = %x\n",segmentAddress + HexRecordStructure.address,data);
      HexRecordStructure.address++;
      byteCount--;
    }
    else if(enableLinearAddress == 1)
    {
      flashMemory[linearAddress + HexRecordStructure.address] = data;//load data in to memory
      printf("flashMemory[%x] = %x\n",linearAddress + HexRecordStructure.address,data);
      HexRecordStructure.address++;
      byteCount--;
    }
    else
    {
      flashMemory[HexRecordStructure.address] = data;//load data in to memory
      printf("flashMemory[%x] = %x\n",HexRecordStructure.address,data);
      HexRecordStructure.address++;
      byteCount--;
    }

  }

}
