#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../HeaderFiles/dynamicArray.h"

static dynamicMachineCodeLinesArray *instructionsArray;
static dynamicIntArray *dataArray;


void insertToInstructionsArray(machineCodeLine *lines[3], int size) {
    int i;
    instructionsArray->arr = (machineCodeLine *)realloc(instructionsArray->arr, (instructionsArray->size + size) * sizeof(machineCodeLine));
    if (instructionsArray->arr == NULL) {
        perror("Error, not enough memory");
        exit(EXIT_FAILURE);
    }

    for (i=0;i<size;i++) {
        instructionsArray->arr[instructionsArray->size + i] = *lines[i];
        /* machineCodeLineToString(lines[i]); */
        free(lines[i]);
    }
    instructionsArray->size += size;
}

void insertToDataArray(int num) {
    dataArray->size++;
    dataArray->arr = (int *)realloc(dataArray->arr, (dataArray->size) * sizeof(int));
    if (dataArray->arr == NULL) {
        perror("Error, not enough memory");
        exit(EXIT_FAILURE);
    }

    *(dataArray->arr + (dataArray->size - 1) ) = num;
}

void initializeInstructionsArray() {
    instructionsArray = (dynamicMachineCodeLinesArray *)malloc(sizeof(dynamicMachineCodeLinesArray));
    if (instructionsArray == NULL) {
        perror("Error, not enough memory");
        exit(EXIT_FAILURE);
    }

    instructionsArray->arr = (machineCodeLine *)malloc(sizeof(machineCodeLine));
    if (instructionsArray->arr == NULL) {
        perror("Error, not enough memory");
        exit(EXIT_FAILURE);
    }

    instructionsArray->size = 0;
    initializeMachineCodeLine(instructionsArray->arr);
}

externNode* insertToExternArray(externNode *node, char *name, int address) {
    externNode *pNode;

    pNode = (externNode *)malloc(sizeof(externNode)); 
    if (pNode == NULL) {
        perror("Error, not enough memory");
        exit(EXIT_FAILURE);
    }

    pNode->name = my_strdup(name);
    pNode->address = address;

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
        perror("Error, not enough memory");
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

void insertStringToInstructionsArray(char *str) {
    int i;
    int size;
    char c;
    char *newStr;
    machineCodeLine **lines;
    newStr = str;
    size = 1;

    while ((c = *newStr++) != '\0')
        size++;
    
    lines = (machineCodeLine **)malloc(size * sizeof(machineCodeLine *));
    if (lines == NULL) {
        perror("Error, not enough memory");
        exit(EXIT_FAILURE);
    }

    initializeMachineCodeLines(lines, size);
    instructionsArray->arr = (machineCodeLine *)realloc(instructionsArray->arr, (instructionsArray->size + size) * sizeof(machineCodeLine));
    if (instructionsArray->arr == NULL) {
        perror("Error, not enough memory");
        exit(EXIT_FAILURE);
    }

    for (i=0; (c = *str++) != '\0'; i++) {
        setMachineCode(lines[i], (int) c );
        if (i == 0) 
            setMachineCodeIsStart (lines[i], true);
        instructionsArray->arr[instructionsArray->size + i] = *lines[i];
        /* machineCodeLineToString(lines[i]); */
    }

    setMachineCode(lines[i], '\0');
    if (i == 1) 
        setMachineCodeIsStart (lines[i], true);
    instructionsArray->arr[instructionsArray->size + i] = *lines[i];
    /* machineCodeLineToString(lines[i]); */

    instructionsArray->size += size;
    free (lines);
}

void insertArrayToDataArray(int *arr, int size) {
    int i;
    for (i=0; i<size; i++)
        insertToDataArray( *(arr + i) );
}

void insertArrayToInstructionsArray (int *arr, int size) {
    int i;
    machineCodeLine **lines;
    lines = (machineCodeLine **)malloc(size * sizeof(machineCodeLine *));
    if (lines == NULL) {
        perror("Error, not enough memory");
        exit(EXIT_FAILURE);
    }

    initializeMachineCodeLines(lines, size);
    
    instructionsArray->arr = (machineCodeLine *)realloc(instructionsArray->arr, (instructionsArray->size + size) * sizeof(machineCodeLine));
    if (instructionsArray->arr == NULL) {
        perror("Error, not enough memory");
        exit(EXIT_FAILURE);
    }

    for (i=0; i<size; i++) {
        setMachineCode(lines[i], *(arr + i));
        if (i == 0) 
            setMachineCodeIsStart (lines[i], true);
        instructionsArray->arr[instructionsArray->size + i] = *lines[i];
        /* machineCodeLineToString(lines[i]); */
    }

    instructionsArray->size += size;
    free (lines);
}

void getInstructionsArray(machineCodeLine **arr) {
    *arr = instructionsArray->arr; 
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
    int i;
    for (i = 0; i < instructionsArray->size; i++)
        free(*(instructionsArray->arr + i));
    free(instructionsArray->arr);
    free(instructionsArray);
}

void freeDataArray() {
    free(dataArray->arr);
    free(dataArray);
}
