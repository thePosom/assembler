#ifndef FIRST_PASS
#define FIRST_PASS

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "dynamicArray.h"
#include "hashmap.h"
#include "globals.h"
#include "util.h"

void firstPass(char *filesName);

int parseDataArray(line *currentLine, int *dataNums);

char* removeQuotions(char *str);

bool endsWithColon(char *str);

#endif