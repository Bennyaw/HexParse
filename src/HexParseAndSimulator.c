#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>
#include "unity.h"
#include "Exception.h"
#include "CException.h"
#include "HexParse.h"
#include "HexParseAndSimulator.h"
#include "Simulator.h"
#include "Error.h"
#include "ErrorSimulator.h"

void simulate(uint8_t *codePtr)
{
  int incr;
  while(1)
  {
    incr = simulateOneInstruction(codePtr);

    if(incr == 2)
    {
      printf("0x%08x\t\t\t0x%04x\n",getPc(codePtr),*(uint16_t*)codePtr);
    }
    else
    {
      if(getch() == 'ESC')
      {
        break;
      }
      else{
        printf("0x%08x\t\t\t0x%08x\n",getPc(codePtr),*(uint32_t*)codePtr);
      }
    }

    codePtr += incr;
  }
}
