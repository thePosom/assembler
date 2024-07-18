#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY


typedef struct 
{
    int size;
    int *arr;
} dynamicIntArray;

void insertToDynamicArray(dynamicIntArray *array, int num);

void insertToInstructionsArray(int num);

void insertToDataArray(int num);

void initializeInstructionsArray();

void initializeDataArray();

int getFromDataArray(int location); /*assumes the call is legal*/

int getFromInstructionsArray(int location); /*assumes the call is legal*/

void insertStringToDataArray(char *str);

void insertArrayToDataArray(int *arr, int size);


#endif