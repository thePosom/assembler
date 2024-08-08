#ifndef PRE_ASSEMBLER
#define PRE_ASSEMBLER

#include "hashmap.h"
#include "globals.h"
#include "errors.h"
#include "util.h"

/*
 * expandMacros
 *
 * This is the main function of the pre-assembler. It takes an input file name,
 * opens the input file (*.as) and the output file (*.am), and processes the
 * input file, handling macro definitions and expansions.
 *
 * The function initializes the macro table and macro names table, then
 * processes each line of the input file, calling the appropriate helper
 * functions to handle macro definitions and expansions. Finally, it frees the
 * macro table and closes the input and output files.
 *
 * Parameters:
 *   filesName - The name of the input file (without the .as extension).
 */
void expandMacros(char *filesName);

#endif