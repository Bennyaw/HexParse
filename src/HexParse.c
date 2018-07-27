#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "HexParse.h"

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

  fp = fopen("assemblerApp.hex","r");
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
  else if((**linePtr) == '\0')
  {
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
    address[i] = *linePtr;
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
    printf("Error");
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
/*
int verifyHexLine(char **linePtr)
{
  uint16_t addData;
  int sizeHexLine;

  if(checkColon(linePtr))
  {
    while(**linePtr == ':')
    {
      (*linePtr)++;
    }

    //sizeHexLine = strlen(*linePtr);

    while(**linePtr != ':')//loop until next hexline
    {
      if(**linePtr == '\0')
      {
        break;
      }

      while(countHexDigit != 0){
        int countHexDigit = 2;//loop every two byte
        if(**linePtr >= '0' && **linePtr <= '9')//convert to int from hex
        {
          base = (int)pow((double)16,p);  //16^1
          decimal = decimal + (**linePtr - 48)*base;
          countHexDigit++;
          p--;  //move from power 1 to 0
          (*linePtr)++; //move to next digit
        }
        else if((toupper(**linePtr)) >= 'A' && (toupper(**linePtr)) <= 'F')//convert to int from hex
        {
          base = (int)pow((double)16,p);  //16^1
          decimal = decimal + (toupper(**linePtr) - 55)*base;
          countHexDigit++;
          p--;  //move from power 1 to 0
          (*linePtr)++; //move to next hexdigit
        }
      }
    }


  }

//  printf("%d\n",sizeHexLine );
  return 0;
}
*/
int convertHexToDec(char **linePtr, int decimal, int p, int base)
{

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

  return decimal;
}
