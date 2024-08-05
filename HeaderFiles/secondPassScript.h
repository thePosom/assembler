#ifndef SECOND_PASS
#define SECOND_PASS

#include "firstPassScript.h"
#include "machinecodeline.h"
#include "dynamicArray.h"
#include "hashmap.h"
#include "globals.h"
#include "util.h"

void secondPass(char *filesName);

void buildFiles(char *filesName, bool entryExists, externNode *externArray, int ICTotal, int DCTotal);

void intToCharsInNBase(int num, char arr[], int size, int base);

bool bothRegisters(char **arguments, int size);

bool isAnyRegister(char *str);

void printExternArrayToFile(externNode *node, FILE *file);

#endif