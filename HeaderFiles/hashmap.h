#ifndef HASHMAP
#define HASHMAP

#include "secondPassScript.h"
#include "globals.h"
#include "errors.h"
#include "util.h"

#define HASHMAPSIZE 256


/*
 * lineNode
 *
 * Represents a node in a linked list of lines.
 */
typedef struct lineNode {
    char *lineText;          /* Text of the line */
    struct lineNode *next;  /* Pointer to the next node in the list */
} lineNode;

/*
 * lineList
 *
 * Represents a list of lines using linked list nodes.
 */
typedef struct {
    lineNode *head;  /* Pointer to the first node in the list */
    lineNode *end;   /* Pointer to the last node in the list */
} lineList;

/*
 * macroNode
 *
 * Represents a node in a linked list of macros.
 */
typedef struct macroNode {
    char *name;           /* Name of the macro */
    lineList *linesList;  /* Pointer to the list of lines in the macro */
    struct macroNode *next;  /* Pointer to the next node in the list */
} macroNode;

/*
 * macroList
 *
 * Represents a list of macros using linked list nodes.
 */
typedef struct {
    macroNode *head;  /* Pointer to the first node in the list */
} macroList;




/*
 * symbolNode
 *
 * Represents a node in a linked list of symbols.
 */
typedef struct symbolNode {
    int value;          /* Value associated with the symbol */
    char *name;         /* Name of the symbol */
    char *type;         /* Type of the symbol */
    struct symbolNode *next;  /* Pointer to the next node in the list */
} symbolNode;

/*
 * symbolList
 *
 * Represents a list of symbols using linked list nodes.
 */
typedef struct {
    symbolNode *head;  /* Pointer to the first node in the list */
} symbolList;




/*
 * initializeMacroTable
 *
 * Initializes the macro hash table by setting all heads to NULL.
 */
void initializeMacroTable();

/*
 * initializeSymbolTable
 *
 * Initializes the symbol hash table by setting all heads to NULL.
 */
void initializeSymbolTable();

/*
 * initializeMacroNamesTable
 *
 * Initializes the macro names hash table by setting all heads to NULL.
 */
void initializeMacroNamesTable();


/*
 * insertToMacroTable
 *
 * Inserts a macro into the macro hash table.
 *
 * Parameters:
 *   lines - A pointer to the list of lines associated with the macro.
 *   name  - The name of the macro.
 */
void insertToMacroTable(lineList *lines, char *name);

/*
 * insertToSymbolTable
 *
 * Inserts a symbol into the symbol hash table.
 *
 * Parameters:
 *   name  - The name of the symbol.
 *   type  - The type of the symbol.
 *   value - The value associated with the symbol.
 */
void insertToSymbolTable(char *name, char *type, int value);

/*
 * insertToMacroNamesTable
 *
 * Inserts a macro name into the macro names hash table.
 *
 * Parameters:
 *   name - The name of the macro.
 */
void insertToMacroNamesTable(char *name);


/*
 * getMacroLines
 *
 * Retrieves the list of lines associated with a given macro name.
 *
 * Parameters:
 *   name - The name of the macro.
 *
 * Returns:
 *   A pointer to the lineList associated with the macro, or NULL if not found.
 */
lineList* getMacroLines(char *name);

/*
 * getSymbolValue
 *
 * Retrieves the value associated with a given symbol name.
 *
 * Parameters:
 *   name - The name of the symbol.
 *
 * Returns:
 *   The value of the symbol, or ERROR if not found.
 */
int getSymbolValue(char *name);

/*
 * getSymbolType
 *
 * Retrieves the type associated with a given symbol name.
 *
 * Parameters:
 *   name - The name of the symbol.
 *
 * Returns:
 *   The type of the symbol as a string, or NULL if not found or the type is unset.
 */
char* getSymbolType(char *name);


/*
 * freeMacroTable
 *
 * Frees all memory allocated for the macro hash table.
 */
void freeMacroTable();

/*
 * freeSymbolTable
 *
 * Frees all memory allocated for the symbol hash table.
 */
void freeSymbolTable();

/*
 * freeMacroNamesTable
 *
 * Frees all memory allocated for the macro names hash table.
 */
void freeMacroNamesTable();


/*
 * insertLineNodeToEnd
 *
 * Inserts a lineNode at the end of a lineList.
 *
 * Parameters:
 *   lines - A pointer to the lineList.
 *   pNode - A pointer to the lineNode to insert.
 */
void insertLineNodeToEnd(lineList *lines, lineNode *pNode);

/*
 * freeList
 *
 * Frees all memory allocated for a list of lines.
 *
 * Parameters:
 *   lines - A pointer to the lineList to free.
 */
void freeList(lineList *lines);


/*
 * isMacroName
 *
 * Checks if a given name is a macro name in the macro names table.
 *
 * Parameters:
 *   name - The name to check.
 *
 * Returns:
 *   true if the name is a macro name, false otherwise.
 */
bool isMacroName(char *name);

/*
 * addToAllDataInSymbolTable
 *
 * Increments the value of all symbols with type ".data" by a given number.
 *
 * Parameters:
 *   num - The number to add to each ".data" symbol's value.
 */
void addToAllDataInSymbolTable(int num);

/*
 * printToFileByType
 *
 * Prints symbols of a specific type to a file.
 *
 * Parameters:
 *   file - The file to print to.
 *   type - The type of symbols to print.
 */
void printToFileByType(FILE *file, char *type);


#endif