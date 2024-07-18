#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/dynamicArray.h"

dynamicIntArray *instructionsArray;
dynamicIntArray *dataArray;


void insertToDynamicArray(dynamicIntArray *array, int num) {
    array->size++;
    array->arr = (int *)realloc(array->arr, (array->size) * sizeof(int));
    if (array->arr == NULL) {
        /*ERROR NOT ENOUGH SPACE*/
    }
    *(array->arr + (array->size - 1) ) = num;
}

void insertToInstructionsArray(int num) {
    insertToDynamicArray(instructionsArray, num);
}

void insertToDataArray(int num) {
    insertToDynamicArray(dataArray, num);
}

void initializeInstructionsArray() {
    instructionsArray->size = 0;
    instructionsArray->arr = NULL;
}

void initializeDataArray() {
    dataArray->size = 0;
    dataArray->arr = NULL;
}

int getFromDataArray(int location) {/*assumes the call is legal*/
    return *(dataArray->arr + location);
}

int getFromInstructionsArray(int location) {/*assumes the call is legal*/
    return *(instructionsArray->arr + location);
}

void insertStringToDataArray(char *str) {
    char c;
    while ((c = *str++) != '\0')
        insertToDataArray((int)c);
}

void insertArrayToDataArray(int *arr, int size) {
    int i;
    for (i=0; i<size; i++)
        insertToDataArray( *(arr + i) );
}