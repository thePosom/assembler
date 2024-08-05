#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include "machinecodeline.h"
#include "util.h"

typedef struct 
{
    int size;
    int *arr;
} dynamicIntArray;

typedef struct 
{
    int size;
    machineCodeLine *arr;
} dynamicMachineCodeLinesArray;

typedef struct externNode 
{
    char *name;
    int address;
    struct externNode *next;
} externNode;

void insertToDynamicArray(dynamicIntArray *array, int num);

void insertToInstructionsArray(machineCodeLine *lines[3], int size);

void insertToDataArray(int num);

void initializeInstructionsArray();

void initializeDataArray();

int getFromDataArray(int DC);

void insertStringToDataArray(char *str);

void insertStringToInstructionsArray(char *str);

void insertArrayToDataArray(int *arr, int size);

void insertArrayToInstructionsArray (int *arr, int size);

void getInstructionsArray(machineCodeLine **arr);

machineCodeLine* getFromInstructionsArray(int location);

int getDataArraySize();

externNode* insertToExternArray(externNode *node, char *name, int address);

void freeExternArray(externNode *node);

#endif