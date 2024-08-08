#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include "machinecodeline.h"
#include "errors.h"
#include "util.h"

/* Dynamic array for integers.
 * Contains a size field to keep track of the number of elements in the array,
 * and a pointer to the dynamically allocated integer array.
 */
typedef struct {
    int size;   /* The number of elements currently in the array */
    int *arr;   /* Pointer to the dynamically allocated array of integers */
} dynamicIntArray;

/* Dynamic array for machine code lines.
 * Contains a size field to keep track of the number of machine code lines in the array,
 * and a pointer to the dynamically allocated array of machineCodeLine structures.
 */
typedef struct {
    int size;   /* The number of machine code lines currently in the array */
    machineCodeLine *arr;   /* Pointer to the dynamically allocated array of machineCodeLine structures */
} dynamicMachineCodeLinesArray;

/* Node for a linked list of external symbols.
 * Contains the name of the external symbol, its address, and a pointer to the next node in the list.
 */
typedef struct externNode {
    char *name;   /* The name of the external symbol */
    int address;  /* The address associated with the external symbol */
    struct externNode *next;   /* Pointer to the next node in the linked list */
} externNode;


/* 
 * Inserts an array of machine code lines into the dynamic instructions array.
 * Allocates or reallocates memory to accommodate the new lines, and updates the array.
 * 
 * Parameters:
 *   lines - Array of pointers to machineCodeLine structures to be inserted.
 *   size - Number of lines to insert.
 */
void insertToInstructionsArray(machineCodeLine *lines[MAX_BINARY_WORDS_IN_LINE], int size);

/* 
 * Inserts an integer into the dynamic data array.
 * Reallocates memory to accommodate the new integer.
 * 
 * Parameters:
 *   num - Integer to insert into the data array.
 */
void insertToDataArray(int num);

/* 
 * Initializes the dynamic instructions array.
 * Allocates memory for the instructions array and sets the initial size to 0.
 */
void initializeInstructionsArray();

/* 
 * Initializes the dynamic data array.
 * Allocates memory for the data array and sets the initial size to 0.
 */
void initializeDataArray();

/* 
 * Retrieves an integer from the dynamic data array based on the index.
 * 
 * Parameters:
 *   DC - Index of the integer to retrieve.
 * 
 * Returns:
 *   The integer at the specified index, or INT_MIN if the index is out of bounds.
 */
int getFromDataArray(int DC);

/* 
 * Inserts a string into the dynamic data array by converting each character to its integer representation.
 * 
 * Parameters:
 *   str - The string to insert into the data array.
 */
void insertStringToDataArray(char *str);

/* 
 * Inserts an array of integers into the dynamic data array.
 * 
 * Parameters:
 *   arr - The array of integers to insert.
 *   size - The number of integers to insert.
 */
void insertArrayToDataArray(int *arr, int size);

/* 
 * Retrieves a machine code line from the dynamic instructions array based on the index.
 * 
 * Parameters:
 *   location - Index of the machine code line to retrieve.
 * 
 * Returns:
 *   A pointer to the machine code line at the specified index, or NULL if the index is out of bounds.
 */
machineCodeLine* getFromInstructionsArray(int location);

/* 
 * Retrieves the size of the dynamic data array.
 * 
 * Returns:
 *   The number of integers currently in the data array.
 */
int getDataArraySize();

/* 
 * Creates and inserts a new externNode into a linked list of external symbols.
 * 
 * Parameters:
 *   node - The head of the existing linked list.
 *   name - The name of the external symbol.
 *   address - The address associated with the external symbol.
 * 
 * Returns:
 *   A pointer to the new head of the linked list.
 */
externNode* insertToExternArray(externNode *node, char *name, int address);


/* 
 * Frees the memory allocated for a linked list of external symbols.
 * 
 * Parameters:
 *   node - The head of the linked list to be freed.
 */
void freeExternArray(externNode *node);

/* 
 * Frees the memory allocated for the dynamic instructions array.
 */
void freeInstructionsArray();

/* 
 * Frees the memory allocated for the dynamic data array.
 */
void freeDataArray();


#endif