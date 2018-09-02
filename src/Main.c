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

int main(int argc, char *argv[])
{
  CEXCEPTION_T e;
  FILE *fp;
  char *hexLineRead;

  uint8_t expectedData[] = {
    255,252,250,243,241,239,233,231,231,229,
    216,216,215,205,198,196,188,187,184,179,
    174,172,172,172,171,170,168,157,133,126,
    121,120,114,109,108,107,102,100,98,80,
    78,78,76,69,63,62,55,48,48,45,
    44,43,43,42,39,34,17,12,8,0
  };

  if(argc < 2) {
    printf("Missing Intel hex file. Please specify one.\n");
    exit(-1);
  }

  fp = fopen(argv[1], "r");
  if(fp == NULL) {
    perror("Error opening file.");
    exit(-1);
  }

  // Load Intel hex file into the simulator flash memory
  while((hexLineRead = readFile(fp)) != NULL) {
    hexParse(hexLineRead,flashMemory);
  }

  Try {
    simulate(flashMemory);
    dumpSram(0, 0x900);
  } Catch(e){
    printf("\nDumping CPU info:\n");
    dumpRegisters();
    printf("Dumping SRAM:\n");
    dumpSram(0, 0x900);
  }
  printf("done.\n");
}
