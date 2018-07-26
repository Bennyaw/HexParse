#ifndef _HEXPARSE_H
#define _HEXPARSE_H

int readFile(void);
int openFile(void);
int getByteCount(char **linePtr);
int checkColon(char **linePtr);
char *extractAddress (char *linePtr);
char *extractRecordType(char *linePtr);
char *extractData(char *linePtr);

#endif // _HEXPARSE_H
