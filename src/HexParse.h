#ifndef _HEXPARSE_H
#define _HEXPARSE_H

#include <stdint.h>

typedef struct HexRecordStructure HexRecordStructure;
struct HexRecordStructure{
  int byteCount;
  int address;
  uint8_t data;
  int recordType;
};


char *readFile(FILE *fileLocation);
int openFile(void);
int getByteCount(char **linePtr);
int checkColon(char **linePtr);
int extractAddress (char **linePtr);
int extractRecordType(char **linePtr);
void extractData(char *linePtr,HexRecordStructure HexRecordStructure);
int verifyHexLine(char **linePtr);
int convertHexToDec(char **linePtr, int decimal, int p, int base);
uint8_t *hexParse(char *linePtr);
void interpretHexLine(char *linePtr, HexRecordStructure HexRecordStructure);

#endif // _HEXPARSE_H
