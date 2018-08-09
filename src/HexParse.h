#ifndef _HEXPARSE_H
#define _HEXPARSE_H

#include <stdint.h>

typedef struct HexRecordStructure HexRecordStructure;
struct HexRecordStructure{
  int byteCount;
  uint32_t address;
  int recordType;
};


char *readFile(FILE *fileLocation);
int openFile(void);
int getByteCount(char **linePtr);
int checkColon(char **linePtr);
int extractAddress (char **linePtr);
int extractRecordType(char **linePtr);
int verifyHexLine(char **linePtr);
int convertHexToDec(char **linePtr, int decimal, int p, int base);
void hexParse(char *linePtr,uint8_t *flashMemory);
void loadData(char *linePtr,HexRecordStructure HexRecordStructure, uint8_t *flashMemory);

#endif // _HEXPARSE_H
