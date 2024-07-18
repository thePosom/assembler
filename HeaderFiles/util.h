#ifndef UTIL
#define UTIL

typedef enum { false, true } bool;

typedef struct {
    int wordsCount;
    char *wordsArray[10];
} line;

typedef struct {
    bool are[3];
    bool dest[4];
    bool origin[4];
    bool opcode[4];
} machineCodeLine;

void lineToArrayOfWords(char *str, line *currentLine);

/* 
gets a clean word of an operation and returns how may operands 
it has. return -1 if it's not an operation
*/
int getOperandsCount(char *operation); 

bool isNameLegal(char *name);

bool isInstruction(char *name);

char *my_strdup(const char *src);

bool endsWithQuotationMark (char *str);

void initializeMachineCodeLine(machineCodeLine *word);

void instructionToOpcode (char *instruction, bool opcode[4]);

bool unsignedNumToBoolArray(unsigned int num, bool arr[], int size);

int my_pow(int a, unsigned int b);

#endif