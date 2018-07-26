#include "Exception.h"
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include "CException.h"
#include "unity.h"
#include "Error.h"


void throwSimpleError(int errorCode, char *errorMsg) {
  Exception *ex = (Exception *)malloc(sizeof(Exception));
	ex->errorCode = errorCode;
	ex->errorMsg = errorMsg;
	Throw(ex);
}

void freeError(Exception *ex)
{
  if(ex)
  {
    if(ex->errorMsg)
    {
      //free(ex->errorMsg);
      free(ex);
    }

  }
}
