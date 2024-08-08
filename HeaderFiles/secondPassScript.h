#ifndef SECOND_PASS
#define SECOND_PASS

#include "firstPassScript.h"
#include "machinecodeline.h"
#include "dynamicArray.h"
#include "hashmap.h"
#include "globals.h"
#include "errors.h"
#include "util.h"

#define SIZE_OF_ADDRESS 4
#define ADDRRESS_BASE 10
#define SIZE_OF_VALUE_IN_OB 5
#define VALUE_IN_OB_BASE 8

/*
* secondPass
*
* Performs the second pass of the assembly process.
*
* Parameters:
*   filesName - The base name of the input file.
*/
void secondPass(char *filesName);

/*
* intToCharsInNBase
*
* Converts an integer value to a character array in the specified base.
*
* Parameters:
*   num - The integer value to be converted.
*   arr - The character array to hold the converted value.
*   size - The size of the character array.
*   base - The base to be used for the conversion (e.g., 8, 10, 16).
*/
void intToCharsInNBase(int num, char arr[], int size, int base);

#endif