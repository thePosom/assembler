#ifndef HASHMAP
#define HASHMAP

#include "secondPassScript.h"
#include "globals.h"
#include "util.h"

#define HASH_STARTING_VALUE 5381
#define HASH_MOVE_BITS_BY 5
#define HASHMAPSIZE 256

/*///////////////////*/
typedef struct lineNode {
    char *lineText;
    struct lineNode *next;
} lineNode;

typedef struct {
    lineNode *head;
    lineNode *end;
} lineList;

typedef struct macroNode {
    char *name;
    lineList *linesList;
    struct macroNode *next;
} macroNode;

typedef struct {
    macroNode *head;
} macroList;


/*///////////////////*/

/*///////////////////*/
typedef struct symbolNode {
    int value;
    char* name;
    char* type;
    struct symbolNode *next;
} symbolNode;

typedef struct {
    symbolNode *head;
} symbolList;
/*///////////////////*/



int hash(unsigned char *str);

void initializeMacroTable();
void initializeSymbolTable();
void initializeMacroNamesTable();

void insertToMacroTable(lineList *lines, char *name);
void insertToSymbolTable(char *name, char *type, int value);
void insertToMacroNamesTable(char *name);

lineList* getMacroLines(char *name);
int getSymbolValue(char *name);
char* getSymbolType(char *name);
bool isMacroName(char *name);

void freeMacroTable();
void freeSymbolTable();
void freeMacroNamesTable();

void freeList(lineList *lines);
void insertLineNodeToEnd(lineList *lines, lineNode *pNode);
void addToAllDataInSymbolTable(int num);

void printToFileByType(FILE *file, char *type);

#endif