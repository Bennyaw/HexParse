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

  printf("Press any key to step. Press ESC key to break.\n");
  while(1)
  {
    incr = simulateOneInstruction(codePtr);

    if(getch() == ESC)
    {
      break;
    }
    else
    {
      if(incr == 4)
      {
        printf("0x%08x(0x%08x)\t\t\t0x%08x\t(step=%d)\n",getPc(codePtr), getPc(codePtr)/2,*(uint32_t*)codePtr, i);
        printf("R0 =0x%02x R1 =0x%02x R2 =0x%02x R3 =0x%02x R4 =0x%02x "
               "R5 =0x%02x R6 =0x%02x R7 =0x%02x R8 =0x%02x R9 =0x%02x \n",
                r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8], r[9]);
        printf("R10=0x%02x R11=0x%02x R12=0x%02x R13=0x%02x R14=0x%02x "
               "R15=0x%02x R16=0x%02x R17=0x%02x R18=0x%02x R19=0x%02x \n",
                r[10], r[11], r[12], r[13], r[14], r[15], r[16], r[17], r[18], r[19]);
        printf("R20=0x%02x R21=0x%02x R22=0x%02x R23=0x%02x R24=0x%02x "
               "R25=0x%02x R26=0x%02x R27=0x%02x R28=0x%02x R29=0x%02x \n",
                r[20], r[21], r[22], r[23], r[24], r[25], r[26], r[27], r[28], r[29]);
        printf("R30=0x%02x R31=0x%02x SREG=0x%02x sram[0x9b]=0x%02x\n", r[30], r[31], *sreg, sram[0x9b]);		  
      }
      else
      {
        printf("0x%08x(0x%08x)\t\t\t0x%04x\t(step=%d)\n",getPc(codePtr), getPc(codePtr)/2,*(uint16_t*)codePtr, i);
        printf("R0 =0x%02x R1 =0x%02x R2 =0x%02x R3 =0x%02x R4 =0x%02x "
               "R5 =0x%02x R6 =0x%02x R7 =0x%02x R8 =0x%02x R9 =0x%02x \n",
                r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8], r[9]);
        printf("R10=0x%02x R11=0x%02x R12=0x%02x R13=0x%02x R14=0x%02x "
               "R15=0x%02x R16=0x%02x R17=0x%02x R18=0x%02x R19=0x%02x \n",
                r[10], r[11], r[12], r[13], r[14], r[15], r[16], r[17], r[18], r[19]);
        printf("R20=0x%02x R21=0x%02x R22=0x%02x R23=0x%02x R24=0x%02x "
               "R25=0x%02x R26=0x%02x R27=0x%02x R28=0x%02x R29=0x%02x \n",
                r[20], r[21], r[22], r[23], r[24], r[25], r[26], r[27], r[28], r[29]);
        printf("R30=0x%02x R31=0x%02x SREG=0x%02x sram[0x9b]=0x%02x\n", r[30], r[31], *sreg, sram[0x9b]);
      }

      codePtr += incr;
      i++;
    }
  }
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
