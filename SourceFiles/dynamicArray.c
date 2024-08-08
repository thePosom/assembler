#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../HeaderFiles/dynamicArray.h"

/* Global variables for dynamic arrays of machine code lines and integers */
static dynamicMachineCodeLinesArray *instructionsArray;
static dynamicIntArray *dataArray;


void insertToInstructionsArray(machineCodeLine *lines[MAX_BINARY_WORDS_IN_LINE], int size) {
    int i;

    /* Reallocate memory for the instructions array to accommodate new lines */
    instructionsArray->arr = (machineCodeLine *)realloc(instructionsArray->arr, (instructionsArray->size + size) * sizeof(machineCodeLine));
    if (instructionsArray->arr == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    /* Copy new lines into the array and free the memory for the original lines */
    for (i = 0; i < size; i++) {
        instructionsArray->arr[instructionsArray->size + i] = *lines[i];
        free(lines[i]);
    }

    /* Free remaining memory for unused lines */
    for (i = size; i < MAX_BINARY_WORDS_IN_LINE; i++)
        free(lines[i]);

    instructionsArray->size += size;
}

void insertToDataArray(int num) {

    /* Increase the size of the data array */
    dataArray->size++;
    dataArray->arr = (int *)realloc(dataArray->arr, (dataArray->size) * sizeof(int));
    if (dataArray->arr == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    /* Insert the new integer into the array */
    *(dataArray->arr + (dataArray->size - 1) ) = num;
}

void initializeInstructionsArray() {
    instructionsArray = (dynamicMachineCodeLinesArray *)malloc(sizeof(dynamicMachineCodeLinesArray));
    if (instructionsArray == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    instructionsArray->arr = (machineCodeLine *)malloc(sizeof(machineCodeLine));
    if (instructionsArray->arr == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    instructionsArray->size = 0;
    initializeMachineCodeLine(instructionsArray->arr);
}

externNode* insertToExternArray(externNode *node, char *name, int address) {
    externNode *pNode;

    /* Allocate memory for the new externNode */
    pNode = (externNode *)malloc(sizeof(externNode)); 
    if (pNode == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    pNode->name = my_strdup(name);
    pNode->address = address;

    /* Insert the new node at the beginning of the list */
    pNode->next = node;
    return pNode;
}

void freeExternArray(externNode *node) {
    externNode *next;
    
    while (node != NULL) {
        next = node->next;
        free(node->name);
        free(node);
        node = next;
    }
}

void initializeDataArray() {
    dataArray = (dynamicIntArray *)malloc(sizeof(dynamicIntArray));
    if (dataArray == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    
    dataArray->size = 0;
    dataArray->arr = NULL;
}

int getFromDataArray(int DC) {
    if (DC > dataArray->size)
        return INT_MIN;
    return *(dataArray->arr + DC);
}

void insertStringToDataArray(char *str) {
    char c;
    while ((c = *str++) != '\0')
        insertToDataArray((int)c);
    insertToDataArray('\0');
}

void insertArrayToDataArray(int *arr, int size) {
    int i;
    for (i=0; i<size; i++)
        insertToDataArray( *(arr + i) );
}

machineCodeLine* getFromInstructionsArray(int location) {
    if (location > instructionsArray->size)
        return NULL;
    return (instructionsArray->arr + location);
}

int getDataArraySize() {
    return dataArray->size;
}

void freeInstructionsArray() {
    free(instructionsArray->arr);
    free(instructionsArray);
}

void freeDataArray() {
    free(dataArray->arr);
    free(dataArray);
}
