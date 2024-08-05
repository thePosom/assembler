#ifndef UTIL
#define UTIL

#include "globals.h"

typedef enum { false, true } bool;

typedef struct {
    int wordsCount;
    char *wordsArray[10];
} line;

void lineToArrayOfWords(char *str, line *currentLine);

/* 
gets a clean word of an operation and returns how may operands 
it has. return -1 if it's not an operation
*/
int getArgumentsCount(char *operation); 

bool isNameLegal(char *name);

bool isInstruction(char *name);

char *my_strdup(const char *src);

bool endsWithQuotationMark (char *str);

int instructionToOpcode (char *instruction);

bool unsignedNumToBoolArray(unsigned int num, bool arr[], int size); /*nope*/

int my_pow(int a, unsigned int b);

void freeStringArray (char **arr, int size);

int stringToNum(char *str);

bool isRegister(char *str);

int getRegisterNum (char *str);

bool endsWithColon(char *str);

FILE* openFile(char *fileName, char *fileType, char* openType);

void removeFile(char *fileName, char *fileType);

#endif