#ifndef _HEXPARSE_H
#define _HEXPARSE_H

int openfile (void);
int getByteCount(char **linePtr);
int hexParser(char **linePtr);
char *extractAddress (char *linePtr);
char *extractRecordType(char *linePtr);
char *extractData(char *linePtr);

#endif // _HEXPARSE_H
