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

void insertToDynamicArray(dynamicIntArray *array, int num);

void insertToInstructionsArray(machineCodeLine *lines[3], int size);

void insertToDataArray(int num);

void initializeInstructionsArray();

void initializeDataArray();

int getFromDataArray(int location); /*assumes the call is legal*/

machineCodeLine getFromInstructionsArray(int location); /*assumes the call is legal*/

void insertStringToDataArray(char *str);

void insertStringToInstructionsArray(char *str);

void insertArrayToDataArray(int *arr, int size);

void insertArrayToInstructionsArray (int *arr, int size);


#endif