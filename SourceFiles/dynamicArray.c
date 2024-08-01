#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/dynamicArray.h"

static dynamicMachineCodeLinesArray *instructionsArray;
static dynamicIntArray *dataArray;


void insertToInstructionsArray(machineCodeLine *lines[3], int size) {
    int i;
    instructionsArray->arr = (machineCodeLine *)realloc(instructionsArray->arr, (instructionsArray->size + size) * sizeof(machineCodeLine));
    if (instructionsArray->arr == NULL) {
        /*ERROR NOT ENOUGH SPACE*/
    }

    for (i=0;i<size;i++) {
        instructionsArray->arr[instructionsArray->size + i] = *lines[i];
        free(lines[i]);
    }
    instructionsArray->size += size;
}

void insertToDataArray(int num) {
    dataArray->size++;
    dataArray->arr = (int *)realloc(dataArray->arr, (dataArray->size) * sizeof(int));
    if (dataArray->arr == NULL) {
        /*ERROR NOT ENOUGH SPACE*/
    }
    *(dataArray->arr + (dataArray->size - 1) ) = num;
}

void initializeInstructionsArray() {
    instructionsArray = (dynamicMachineCodeLinesArray *)malloc(sizeof(dynamicMachineCodeLinesArray));
    instructionsArray->arr = (machineCodeLine *)malloc(sizeof(machineCodeLine));
    instructionsArray->size = 0;
    initializeMachineCodeLine(instructionsArray->arr);
}

void initializeDataArray() {
    dataArray = (dynamicIntArray *)malloc(sizeof(dynamicIntArray));
    dataArray->size = 0;
    dataArray->arr = NULL;
}

int getFromDataArray(int location) {/*assumes the call is legal*/
    return *(dataArray->arr + location);
}

machineCodeLine getFromInstructionsArray(int location) {/*assumes the call is legal*/
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
