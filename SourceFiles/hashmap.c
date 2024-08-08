#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../HeaderFiles/hashmap.h"

#define HASH_STARTING_VALUE 5381
#define HASH_MOVE_BITS_BY 5

/*
 * hash
 *
 * Computes a hash value for a given string using the DJB2 algorithm.
 *
 * Parameters:
 *   str - The string to hash.
 *
 * Returns:
 *   An integer hash value for the string.
 */
int hash(unsigned char *str);


static macroList macroHashmap[HASHMAPSIZE];
static symbolList macroNamesHashmap[HASHMAPSIZE];
static symbolList symbolsHashmap[HASHMAPSIZE];


int hash(unsigned char *str) {
    unsigned long hash = HASH_STARTING_VALUE;
    int c;

    /* Compute the hash value by iterating through the string */
    while ((c = *str++) != '\0')
        hash = ((hash << HASH_MOVE_BITS_BY) + hash) + c;

    return (hash % HASHMAPSIZE);
}


void initializeMacroTable() {
    int i;
    /* Initialize each entry in the macro hash table */
    for (i = 0; i < HASHMAPSIZE; i++) 
        macroHashmap[i].head = NULL;
}

void initializeSymbolTable() {
    int i;
    /* Initialize each entry in the symbols hash table */
    for (i = 0; i < HASHMAPSIZE; i++) 
        symbolsHashmap[i].head = NULL;
}

void initializeMacroNamesTable() {
    int i;
    /* Initialize each entry in the macro names hash table */
    for (i = 0; i < HASHMAPSIZE; i++) 
        macroNamesHashmap[i].head = NULL;
}


void insertToMacroTable(lineList *lines, char *name) {

    int location;
    macroNode *pNode;

    /* Compute the hash location for the given macro name */
    location = hash((unsigned char *) name);

    /* Allocate memory for a new macro node */
    pNode = (macroNode *)malloc(sizeof(macroNode)); 
    if (pNode == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }
     
    /* Set the macro node's properties */
    pNode->name = my_strdup(name);
    pNode->linesList = lines;

    /* Insert the new macro node into the hash table */
    pNode->next = macroHashmap[location].head;
    macroHashmap[location].head = pNode;

    /* Insert the macro name into the macro names hash table */
    insertToMacroNamesTable(name);
}

void insertToSymbolTable(char *name, char *type, int value) { 

    int location;
    symbolNode *pNode;

    /* Compute the hash location for the given symbol name */
    location = hash((unsigned char *) name);

    /* Allocate memory for a new symbol node */
    pNode = (symbolNode *)malloc(sizeof(symbolNode));
    if (pNode == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    /* Set the symbol node's properties */
    pNode->name = my_strdup(name);
    pNode->type = my_strdup(type);
    pNode->value = value;

    /* Insert the new symbol node into the hash table */
    pNode->next = symbolsHashmap[location].head;
    symbolsHashmap[location].head = pNode;
}

void insertToMacroNamesTable(char *name) {

    int location;
    symbolNode *pNode;

    /* Compute the hash location for the given macro name */
    location = hash((unsigned char *) name);

    /* Allocate memory for a new symbol node */
    pNode = (symbolNode *)malloc(sizeof(symbolNode)); 
    if (pNode == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    /* Set the symbol node's properties */
    pNode->name = my_strdup(name);
    pNode->type = NULL; /* Not used for macro names */
    pNode->value = 0;

    /* Insert the new symbol node into the macro names hash table */
    pNode->next = symbolsHashmap[location].head;
    symbolsHashmap[location].head = pNode;
}



lineList* getMacroLines(char *name) {
    int location;
    macroNode *pNode;

    /* Compute the hash location for the given macro name */
    location = hash((unsigned char *) name);
    pNode = macroHashmap[location].head;

    /* Search for the macro node with the matching name */
    while (pNode) {
        if (strcmp((pNode->name), name) == 0)
            return pNode->linesList;
        pNode = pNode->next;
    }
    
    /* Return NULL if no matching macro is found */
    return NULL;
}


int getSymbolValue(char *name) {
    int location;
    symbolNode *pNode;

    /* Compute the hash location for the given symbol name */
    location = hash((unsigned char *) name);
    pNode = symbolsHashmap[location].head;

    /* Search for the symbol node with the matching name */
    while (pNode) {
        if (strcmp((pNode->name), name) == 0)
            return pNode->value;
        pNode = pNode->next;
    }
    
    /* Return ERROR if no matching symbol is found */
    return ERROR;
}

char* getSymbolType(char *name) {
    int location;
    symbolNode *pNode;

    /* Compute the hash location for the given symbol name */
    location = hash((unsigned char *) name);
    pNode = symbolsHashmap[location].head;

    /* Search for the symbol node with the matching name */
    while (pNode) {
        if (strcmp((pNode->name), name) == 0) {
            if (pNode->type != NULL)
                return pNode->type;
            else 
                return NULL;
        }
        pNode = pNode->next;
    }
    
    /* Return NULL if no matching symbol is found */
    return NULL;
}


bool isMacroName(char *name) {
    int location;
    symbolNode *pNode;

    /* Compute the hash location for the given macro name */
    location = hash((unsigned char *) name);
    pNode = macroNamesHashmap[location].head;

    /* Search for the symbol node with the matching macro name */
    while (pNode) {
        if (strcmp((pNode->name), name) == 0)
            return true;
        pNode = pNode->next;
    }
    
    /* Return false if no matching macro name is found */
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

    /* Free each node in the line list */
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
        /* If the line list is empty, set the new node as the head */
        lines->head = pNode;
    else 
        /* Append the new node to the end of the list */
        lines->end->next = pNode;
    /* Update the end pointer to the new node */
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

                /* If the symbol type is ".data", increment its value */
                if (pNode->type != NULL && strcmp(pNode->type, ".data") == 0)
                    pNode->value += num;
                pNode = next;
            }
        }
    }
}

void printToFileByType(FILE *file, char *type) {
    int i;
    for (i = 0; i < HASHMAPSIZE; i++) {
        if (symbolsHashmap[i].head) {
            symbolNode *pNode = symbolsHashmap[i].head;
            while (pNode) {
                symbolNode* next;
                next = pNode->next;

                /* If the symbol type matches the specified type, print it to the file */
                if (pNode->type != NULL && strcmp(pNode->type, type) == 0) {
                    char address[SIZE_OF_ADDRESS + 1];
                    intToCharsInNBase(pNode->value, address, SIZE_OF_ADDRESS, ADDRRESS_BASE);
                    fprintf(file, "%s %s\n", pNode->name, address);
                }
                pNode = next;
            }
        }
    }
}
