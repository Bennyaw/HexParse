#include "Exception.h"
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include "CException.h"
//#include "unity.h"
#include "Error.h"


void throwSimpleError(int errorCode, char *errorMsg) {
  Exception *ex = (Exception *)malloc(sizeof(Exception));
	ex->errorCode = errorCode;
	ex->errorMsg = errorMsg;
	Throw(ex);
}

void throwError(int errorCode, char *format, ... ) {
  va_list valist;
  char *buffer;
  int i, needed_size;
  Exception *ex;


  va_start(valist, format);
  ex = (Exception *) malloc(sizeof(Exception));
  needed_size = vsnprintf(NULL, 0, format, valist) + 1;
  buffer = malloc (needed_size);
  needed_size = vsnprintf(buffer, needed_size, format, valist) + 1;
  va_end(valist);

  ex->errorCode = errorCode;
  ex->errorMsg = buffer;
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
