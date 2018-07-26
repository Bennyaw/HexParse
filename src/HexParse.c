#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <ctype.h>
#include "HexParse.h"


int hexParser(char **linePtr)
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
  int p = 1;

  if((**linePtr) == ':') //check whetehr got colon sign
  {
    (*linePtr)++;
  }
  else
  {
    return 0;
  }

  while(count<2) // after
  {
    if(**linePtr >= '0' && **linePtr <= '9')
    {
      base = (int)pow((double)16,p);  //16^1
      decimal = decimal + (**linePtr - 48)*base;
      count++;
      p--;  //move from power 1 to 0
      (*linePtr)++; //move to next digit
    }
    else if((toupper(**linePtr)) >= 'A' && (toupper(**linePtr)) <= 'F')
    {
      base = (int)pow((double)16,p);  //16^1
      decimal = decimal + (toupper(**linePtr) - 55)*base;
      count++;
      p--;  //move from power 1 to 0
      (*linePtr)++; //move to next hexdigit
    }

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

  if(hexParser(&linePtr))
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

  while(byteCount != 0){
    while(isalpha(*linePtr) || isdigit(*linePtr))
    {
      dataExtracted[i] = *linePtr;
      linePtr++;
      byteCount--;
      i++;
    }
    if(linePtr == '\0')
    {
      break;
    }
  }
  dataExtracted[i] = '\0';
  return dataExtracted;
}
