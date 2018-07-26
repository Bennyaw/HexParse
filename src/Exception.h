#ifndef _EXCEPTION_H
#define _EXCEPTION_H


typedef struct Exception Exception; //for c++ and c
struct Exception
{
  int errorCode;
  char *errorMsg;
};

void throwSimpleError(int errorCode, char *errorMsg);
void throwError(int errorCode,char *format,...);
void freeError(Exception *ex);
#endif // _EXCEPTION_H
