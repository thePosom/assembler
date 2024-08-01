#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../HeaderFiles/hashmap.h"




static macroList macroHashmap[HASHMAPSIZE];
static symbolList macroNamesHashmap[HASHMAPSIZE];
static symbolList symbolsHashmap[HASHMAPSIZE];


int hash(unsigned char *str) {
    unsigned long hash = HASH_STARTING_VALUE;
    int c;

    while ((c = *str++) != '\0')
        hash = ((hash << HASH_MOVE_BITS_BY) + hash) + c;

    return (hash % HASHMAPSIZE);
}


void initializeMacroTable() {
    int i;
    for (i = 0; i < HASHMAPSIZE; i++) 
        macroHashmap[i].head = NULL;
}

void initializeSymbolTable() {
    int i;
    for (i = 0; i < HASHMAPSIZE; i++) 
        symbolsHashmap[i].head = NULL;
}

void initializeMacroNamesTable() {
    int i;
    for (i = 0; i < HASHMAPSIZE; i++) 
        macroNamesHashmap[i].head = NULL;
}


void insertToMacroTable(lineList *lines, char *name) {

    int location;
    macroNode *pNode;

    location = hash ((unsigned char *) name);
    pNode = (macroNode *)malloc(sizeof(macroNode)); 

    pNode->name = my_strdup(name);
    pNode->linesList = lines;

    pNode->next = macroHashmap[location].head;
    macroHashmap[location].head = pNode;

    insertToMacroNamesTable(name);
}

void insertToSymbolTable(char *name, char *type, int value) { 

    int location;
    symbolNode *pNode;

    location = hash ((unsigned char *) name);
    pNode = (symbolNode *)malloc(sizeof(symbolNode)); 

    pNode->name = my_strdup(name);
    pNode->type = my_strdup(type);

    pNode->next = symbolsHashmap[location].head;
    symbolsHashmap[location].head = pNode;
}

void insertToMacroNamesTable(char *name) {

    int location;
    symbolNode *pNode;

    location = hash ((unsigned char *) name);
    pNode = (symbolNode *)malloc(sizeof(symbolNode)); 

    pNode->name = my_strdup(name);
    pNode->type = NULL;
    pNode->value = 0;

    pNode->next = symbolsHashmap[location].head;
    symbolsHashmap[location].head = pNode;
}


lineList* getMacroLines(char *name) {
    int location;
    macroNode *pNode;

    location = hash((unsigned char *) name);
    pNode = macroHashmap[location].head;

    while (pNode) {
        if (strcmp ( (pNode->name), name ) == 0 )
            return pNode->linesList;
        pNode = pNode->next;
    }
    
    return NULL;
}

int getSymbolValue(char *name) {
    int location;
    symbolNode *pNode;

    location = hash((unsigned char *) name);
    pNode = symbolsHashmap[location].head;

    while (pNode) {
        if (strcmp ( (pNode->name), name ) == 0 )
            return pNode->value;
        pNode = pNode->next;
    }
    
    return ERROR;
}

char* getSymbolType(char *name) {
    int location;
    symbolNode *pNode;

    location = hash((unsigned char *) name);
    pNode = symbolsHashmap[location].head;

    while (pNode) {
        if (strcmp ( (pNode->name), name ) == 0 ) {
            if (pNode->type != NULL)
                return pNode->type;
            else 
                return NULL;
        }
        pNode = pNode->next;
    }
    
    return NULL;
}

bool isMacroName(char *name) {
    int location;
    symbolNode *pNode;

    location = hash((unsigned char *) name);
    pNode = macroNamesHashmap[location].head;

    while (pNode) {
        if (strcmp ( (pNode->name), name ) == 0 )
            return true;
        pNode = pNode->next;
    }
    
    return false;
}


void freeMacroTable() {
    int i;
    for (i = 0; i < HASHMAPSIZE; i++) {
        if (macroHashmap[i].head) {
            macroNode *pNode = macroHashmap[i].head;
            while (pNode) {
                macroNode* next;
                next = pNode->next;
                free(pNode->name);
                freeList(pNode->linesList);
                free(pNode);
                pNode = next;
            }
        }
    }
}

void freeSymbolTable() {
    int i;
    for (i = 0; i < HASHMAPSIZE; i++) {
        if (symbolsHashmap[i].head) {
            symbolNode *pNode = symbolsHashmap[i].head;
            while (pNode) {
                symbolNode* next;
                next = pNode->next;
                free(pNode->name);
                free(pNode->type);
                free(pNode);
                pNode = next;
            }
        }
    }
}

void freeMacroNamesTable() {
    int i;
    for (i = 0; i < HASHMAPSIZE; i++) {
        if (macroNamesHashmap[i].head) {
            symbolNode *pNode = macroNamesHashmap[i].head;
            while (pNode) {
                symbolNode* next;
                next = pNode->next;
                free(pNode->name);
                free(pNode);
                pNode = next;
            }
        }
    }
}

void freeList(lineList *lines) {
    lineNode *nextNode;
    lineNode *current;
    
    if (lines == NULL)
        return;

    current = lines->head;

    while (current) {
        nextNode = current->next;
        free(current->lineText);
        free(current);
        current = nextNode;
    }

    lines->head = NULL;
    lines->end = NULL;
    free(lines); 
}

void insertLineNodeToEnd(lineList *lines, lineNode *pNode) {
    if (lines->head == NULL) 
        lines->head = pNode;
    else 
        lines->end->next = pNode;
    lines->end = pNode;
}


void addToAllDataInSymbolTable(int num) {
    int i;
    for (i = 0; i < HASHMAPSIZE; i++) {
        if (symbolsHashmap[i].head) {
            symbolNode *pNode = symbolsHashmap[i].head;
            while (pNode) {
                symbolNode* next;
                next = pNode->next;
                if (pNode->type != NULL && strcmp(pNode->type, ".data") == 0) /*segment*/
                    pNode->value+=num;
                pNode = next;
            }
        }
    }
}
