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


int openFile(void)
{
  FILE * fp;

  fp = fopen ("myfile.txt", "w+");
  fprintf(fp, "%s %d", "we are in ", 2018);

  fclose(fp);

  return(0);
}

int readFile(void)
{
  FILE *fp;
  int c;

  fp = fopen("testfile.txt","r");
  while(1) {
     c = fgetc(fp);
     if( feof(fp) ) {
        break ;
     }
     printf("%c", c);
  }
  fclose(fp);

  return(0);
}

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

  if((**linePtr) == ':') //check whetehr got colon sign
  {
    (*linePtr)++;
  }

  while(count<2) // after colon sign
  {
    base = (int)pow((double)16,power);  //16^1
    decimal = convertHexToDec(linePtr, decimal, power, base);
    power--;  //move from power 1 to 0
    count++;
  }

  return decimal;
}

char *extractAddress (char *linePtr)
{
  int stepCount = 4;
  char *address;
  address = malloc(4);
  int i = 0;

  while(stepCount != 0){
    address[i] = toupper(*linePtr);
    i++;
    linePtr++;
    stepCount--;
  }
  address[i] = '\0';
  return address;
}

char *extractRecordType(char *linePtr)
{
  int stepCount = 2;
  char *recordType;
  recordType = malloc(2);
  int i = 0;

  while(stepCount!= 0){
    recordType[i] = *linePtr;
    i++;
    linePtr++;
    stepCount--;
  }
  recordType[i] = '\0';
  return recordType;
}

char *extractData(char *linePtr)
{
  int movePtr = 0;
  int byteCount = 0;
  int i= 0;
  char *dataExtracted;
  dataExtracted = malloc(byteCount*2);

  if(checkColon(&linePtr))
  {
    byteCount = (getByteCount(&linePtr))*2;
    for(movePtr = 0; movePtr < 6; movePtr++) //skip address and recordtype to get data
    {
      linePtr++;
    }

  }
  else
  {
    printf("In function extractData, checkColon fail.\n");
  }


  while(isalpha(*linePtr) || isdigit(*linePtr))
  {
    dataExtracted[i] = *linePtr;
    linePtr++;
    byteCount--;
    i++;

    if(byteCount == 0)
    {
      break;
    }
  }


  dataExtracted[i] = '\0';
  return dataExtracted;
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

    //sizeHexLine = strlen(*linePtr);

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
    printf("verifyData(non-zero means error data) : %u\n", verifyData);//checking hex digit whether is 00

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

//  printf("%d\n",sizeHexLine );
  return addData;
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
    errorChar = (**linePtr - 0);
    printf("errorChar : %c\n", errorChar);
    throwError(ERR_UNREGCONISED_DATA,"Error : Unregconised data '%c' found in the line.",errorChar);
  }

  return decimal;
}
