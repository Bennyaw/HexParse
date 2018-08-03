#ifndef _HEXPARSE_H
#define _HEXPARSE_H

char *readFile(FILE *fileLocation);
int openFile(void);
int getByteCount(char **linePtr);
int checkColon(char **linePtr);
int extractAddress (char *linePtr);
int extractRecordType(char *linePtr);
int *extractData(char *linePtr,int size);
int verifyHexLine(char **linePtr);
int convertHexToDec(char **linePtr, int decimal, int p, int base);

#endif // _HEXPARSE_H
