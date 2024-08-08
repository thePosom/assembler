#ifndef FIRST_PASS
#define FIRST_PASS

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "machinecodeline.h"
#include "dynamicArray.h"
#include "hashmap.h"
#include "globals.h"
#include "errors.h"
#include "util.h"

#define MAX_LABEL_SIZE 32

#define IMMEDIATE_ADDRESING_METHOD 0
#define DIRECT_ADDRESING_METHOD 1
#define INDIRECT_REGISTER_ADDRESING_METHOD 2
#define DIRECT_REGISTER_ADDRESING_METHOD 3

/*
 * firstPass
 *
 * This is the main function of the first pass of the assembler. It takes an
 * input file name, opens the input file (*.am), and processes the input file,
 * generating most of the machine code and updating the symbol table.
 *
 * The function initializes the data array, instructions array, and symbol
 * table, then processes each line of the input file, calling the appropriate
 * helper functions to handle labels, directives, and instructions. Finally,
 * it adds the data addresses to the symbol table and closes the input file.
 *
 * Parameters:
 *   filesName - The name of the input file (without the .am extension).
 *
 * Returns:
 *   True if the first pass was successful (i.e., no errors occurred), false
 *   otherwise.
 */
bool firstPass(char *filesName);

/*
 * parseDataArray
 *
 * This function parses a line of input containing .data and
 * converts the comma-separated values into an array of integers.
 *
 * Parameters:
 *   currentLine - The current line of the input assembly code.
 *   startingWord - The index of the first word in the line that needs
 *                  to be processed.
 *   dataNums - A double pointer to the output array of integers.
 *   filename - The name of the input file.
 *
 * Returns:
 *   The number of data values parsed, or ERROR if an error occurred.
 */
int parseDataArray(line *currentLine, int startingWord, int **dataNums, char *filename);

/*
 * removeQuotions
 *
 * This function removes the quotation marks from a string.
 *
 * Parameters:
 *   str - The input string.
 *
 * Returns:
 *   A new string with the quotation marks removed.
 */
char* removeQuotions(char *str);

/*
 * parseCommas
 *
 * This function parses a line of input containing comma-separated values and
 * stores the individual values in an array.
 *
 * Parameters:
 *   currentLine - The current line of the input assembly code.
 *   startingWord - The index of the first word in the line that needs
 *                  to be processed.
 *   output - A double pointer to the output array of strings.
 *   filename - The name of the input file.
 *
 * Returns:
 *   The number of values parsed, or ERROR if an error occurred.
 */
int parseCommas(line *currentLine, int startingWord, char ***output, char *filename);

#endif