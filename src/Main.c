#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>
#include "Exception.h"
#include "CException.h"
#include "HexParse.h"
#include "HexParseAndSimulator.h"
#include "Simulator.h"
#include "Error.h"
#include "ErrorSimulator.h"

int main(void)
{
  CEXCEPTION_T e;
  FILE *fp;
  char *hexLineRead;
  fp = fopen("data/test/testBubbleSort.hex","r");
  uint8_t expectedData[] ={
    255,252,250,243,241,239,233,231,231,229,
    216,216,215,205,198,196,188,187,184,179,
    174,172,172,172,171,170,168,157,133,126,
    121,120,114,109,108,107,102,100,98,80,
    78,78,76,69,63,62,55,48,48,45,
    44,43,43,42,39,34,17,12,8,0
  };

  if(fp == NULL){
    perror("Error opening file");
  }

  while((hexLineRead = readFile(fp)) != NULL)
  {
    hexParse(hexLineRead,flashMemory);
  }
  initialiseSram();
  Try{
    simulate(flashMemory);
  } Catch(e){
    
  }
}
