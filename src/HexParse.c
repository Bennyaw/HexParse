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

int *memory;
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

//--------testing open file-------------
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
  fgets (hexLine, k, fileLocation);
  hexLinelength = strlen(hexLine);
  hexLine[hexLinelength-1] = '\0';
  printf("hexLine[%d] : %c\n",43,hexLine[43] );
  printf("hexLinelength : %d\n",hexLinelength );
  printf("In readFile function : %s", hexLine);
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

  while(count<2) // after colon sign
  {
    base = (int)pow((double)16,power);  //16^1
    decimal = convertHexToDec(linePtr, decimal, power, base);
    power--;  //move from power 1 to 0
    count++;
  }

  return decimal;
}

int extractAddress (char *linePtr)
{
  int decimal = 0,count = 0;
  int base = 1;
  int power = 3;
  int intaddress = 0;

  while(count<4) // after colon sign
  {
    base = (int)pow((double)16,power);  //16^3
    intaddress = convertHexToDec(&linePtr, intaddress, power, base);
    power--;  //decrement power
    count++;
  }
  return intaddress;
}

int extractRecordType(char *linePtr)
{
  int decimal = 0,count = 0;
  int base = 1;
  int power = 1;
  int intRecordType = 0;
  int errorFlag = 0;

  while(count<2){

    if(*linePtr <'0' || *linePtr >'5')//check if is in 0-5
    {
      errorFlag = 1;
    }

    base = (int)pow((double)16,power);  //16^3
    intRecordType = convertHexToDec(&linePtr, intRecordType, power, base);
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


int *extractData(char *linePtr,int size)
{
  int *memoryLoading = malloc(size) ;
  int byteCount = size;
  int i = 0;

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

    memoryLoading[i] = data;
    i++;
    byteCount--;
  }

  memoryLoading[i] = '\0';

  return memoryLoading;
}

int verifyHexLine(char **linePtr)
{
  uint16_t getData = 0,addData = 0;
  uint8_t  verifyData;
  int sizeHexLine;
  int countHexDigit = 2;

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

    verifyData = addData & 0xff;//if not zero then is error data
    //printf("verifyData(non-zero means error data) : %u\n", verifyData);//checking hex digit whether is 00

    if(verifyData == 0)
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
/*
//---------------------main function--------------------------
int hexParse(char *linePtr)
{
  char *ptrForVerify = linePtr;
  int getAddress = 0;
  int recordType = 0;
  int byteCount = 0;

  if (verifyHexLine(&ptrForVerify))
  {
    while(*linePtr == ':')
    {
      linePtr = linePtr + 2;//move pointer to address
    }

    byteCount = getByteCount(&linePtr);//get size of data
    getAddress = extractAddress(linePtr);
    recordType = extractRecordType(linePtr);//error thrown in the funciton

    interpretHexLine(linePtr,getAddress,byteCount,recordType);

  }
  else
  {
    return 0;//error thrown inside verifyHexLine function
  }
}

void interpretHexLine(char *linePtr, int dataAddress, int byteCount, int *recordType)
{
  switch(recordType){
    case '00': memory = extractData(linePtr,byteCount);
    case '01':
    case '02':
    case '03':
    case '04':
    case '05':

    default:



  }


}
*/
