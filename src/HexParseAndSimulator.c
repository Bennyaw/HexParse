#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>
//#include "unity.h"
#include "Exception.h"
#include "CException.h"
#include "HexParse.h"
#include "HexParseAndSimulator.h"
#include "Simulator.h"
#include "Error.h"
#include "ErrorSimulator.h"

#define ESC     27


void simulate(uint8_t *codePtr)
{
  uint32_t i = 0;
  int incr;
  char ch;

  initSimulator();

  printf("Press any key to step. Press 'd' to dump the whole SRAM content.\n"
         "Press 0...8 key to dump selective portion of the SRAM content.\n"
         "Press ESC key to break.\n");
  while(1) {
    if((ch = getch()) == ESC) {
      break;
    } else if(ch == 'd' || ch == 'D') {
      dumpSram(0, 0x900);
      printf("\n");
    } else if(ch >= '0' && ch <= '8') {
      dumpSram((int)(ch - '0') << 8, 0x100);
      printf("\n");
    } else {
      incr = simulateOneInstruction(codePtr);
      if(is2wordInstruction(codePtr)) {
        printf("0x%08x(0x%08x)\t\t\t0x%08x\t(step=%d)\n",getPc(codePtr), getPc(codePtr)/2,*(uint32_t*)codePtr, i);
      } else {
        printf("0x%08x(0x%08x)\t\t\t0x%04x\t(step=%d)\n",getPc(codePtr), getPc(codePtr)/2,*(uint16_t*)codePtr, i);
      }
      dumpRegisters();
      codePtr += incr;
      i++;
    }
  }
}

//#define SIMULATED_STEPS   13030
#define SIMULATED_STEPS   14100

void simulate1(uint8_t *codePtr)
{
  uint32_t i = SIMULATED_STEPS;
  int incr;
  char ch;

  printf("Simulating.\n");
  while(i--)
  {
    incr = simulateOneInstruction(codePtr);
    codePtr += incr;
    printf("0x%08x\n", getPc(codePtr)/2);
    if(0x22 == getPc(codePtr)/2) {
      printf("i = %d\n", i);
      break;
    }
  }
  printf("Simulated %d instructions.\n", SIMULATED_STEPS);
}


/*
incr = simulateOneInstruction(codePtr);

if(getch() == 'ESC')
{
  break;
}
else
{
  if(incr == 2)
  {
    printf("0x%08x\t\t\t0x%04x\n",getPc(codePtr),*(uint16_t*)codePtr);
  }
  else
  {
    printf("0x%08x\t\t\t0x%08x\n",getPc(codePtr),*(uint32_t*)codePtr);
  }

  codePtr += incr;*/
