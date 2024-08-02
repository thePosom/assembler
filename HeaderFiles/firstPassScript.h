#ifndef FIRST_PASS
#define FIRST_PASS

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "machinecodeline.h"
#include "dynamicArray.h"
#include "hashmap.h"
#include "globals.h"
#include "util.h"

void firstPass(char *filesName);

int parseDataArray(line *currentLine, int **dataNums);

char* removeQuotions(char *str);

bool endsWithColon(char *str);

int newSymbol(line *currentLine, int startingWord, int DC, int IC);

int parseCommas(line *currentLine, int startingWord, char ***output);

int lineToMachineCode(line *currentLine, int startingWord, machineCodeLine *lines[3]);

int ifRegister (machineCodeLine *lines[3], int num, int i, bool *firstIsRegister, int *size);

#endif