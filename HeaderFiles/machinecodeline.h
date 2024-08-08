#ifndef MACHINE_CODE_LINE
#define MACHINE_CODE_LINE

#include "errors.h"
#include "util.h"

#define MAX_BINARY_WORDS_IN_LINE 3
#define UNSIGNED_CHARS_IN_MACHINE_CODE_LINE 2
#define SIZE_OF_ADDRESING_METHODS_IN_WORD 4
#define SIZE_OF_VALUES_IN_WORD 8
#define SIZE_OF_OPCODE 4

#define ARE_MASK 0x7
#define DEST_MASK 0xf0
#define ORIGIN_MASK 0xf
#define OPCODE_MASK 0xf0
#define DEST_REGISTER_MASK 0x8f
#define ORIGIN_REGISTER_MASK 0xfc

#define ARE_SIZE 3
#define A_IN_ARE 2
#define R_IN_ARE 1
#define E_IN_ARE 0


/*
 * machineCodeLine
 *
 * This structure represents a line of machine code, with two bytes used to
 * store various fields, including addressing methods, opcode, and values.
 *
 * Members:
 *   values - An array of two unsigned chars. 
 *            char[0] holds ARE (0-2 bits) and Dest (4-7 bits), 
 *            char[1] holds Origin (0-3 bits) and Opcode (4-7 bits).
 */
typedef struct {
    unsigned char values[UNSIGNED_CHARS_IN_MACHINE_CODE_LINE];
} machineCodeLine;


/*
 * initializeMachineCodeLine
 *
 * This function initializes a `machineCodeLine` structure by setting all
 * values to zero.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure to be initialized.
 */
void initializeMachineCodeLine(machineCodeLine *line);

/*
 * initializeMachineCodeLines
 *
 * This function initializes an array of `machineCodeLine` structures.
 *
 * Parameters:
 *   lines - An array of pointers to `machineCodeLine` structures.
 *   size - The number of `machineCodeLine` structures to initialize.
 */
void initializeMachineCodeLines(machineCodeLine *lines[], int size);

/*
 * setMachineCodeARE
 *
 * This function sets a specific ARE bit in a `machineCodeLine` structure.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure.
 *   location - The bit position to set.
 *   value - The value to set (true for 1, false for 0).
 */
void setMachineCodeARE(machineCodeLine *line, int location, bool value);

/*
 * setMachineCodeValues
 *
 * This function sets a specific value bit in a `machineCodeLine` structure.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure.
 *   location - The bit position to set.
 *   value - The value to set (true for 1, false for 0).
 */
void setMachineCodeValues(machineCodeLine *line, int location, bool value);

/*
 * getMachineCodeARE
 *
 * This function retrieves the value of a specific ARE bit from a
 * `machineCodeLine` structure.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure.
 *   location - The bit position to retrieve.
 *
 * Returns:
 *   The value of the specified ARE bit (true for 1, false for 0).
 */
bool getMachineCodeARE(machineCodeLine *line, int location);

/*
 * getMachineCodeValues
 *
 * This function retrieves the value of a specific value bit from a
 * `machineCodeLine` structure.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure.
 *   location - The bit position to retrieve.
 *
 * Returns:
 *   The value of the specified value bit (true for 1, false for 0).
 */
bool getMachineCodeValues(machineCodeLine *line, int location);

/*
 * insertOpcodeToMachineCodeLine
 *
 * This function inserts an opcode into a `machineCodeLine` structure.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure.
 *   opcode - The opcode to insert.
 */
void insertOpcodeToMachineCodeLine(machineCodeLine *line, int opcode);

/*
 * insertValuesToMachineCodeLine
 *
 * This function inserts values into a `machineCodeLine` structure.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure.
 *   values - The values to insert.
 */
void insertValuesToMachineCodeLine(machineCodeLine *line, int values);

/*
 * setMachineCode3To5Values
 *
 * This function sets values in the 3 to 5 bit positions of a `machineCodeLine`
 * structure.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure.
 *   value - The value to set.
 */
void setMachineCode3To5Values(machineCodeLine *line, int value);

/*
 * setMachineCode6To8Values
 *
 * This function sets values in the 6 to 8 bit positions of a `machineCodeLine`
 * structure.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure.
 *   value - The value to set.
 */
void setMachineCode6To8Values(machineCodeLine *line, int value);

/*
 * setMachineCode
 *
 * This function sets the entire machine code in a `machineCodeLine` structure.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure.
 *   value - The value to set.
 */
void setMachineCode(machineCodeLine *line, int value);

/*
 * machineCodeLineToInt
 *
 * This function converts a `machineCodeLine` structure to an integer value.
 *
 * Parameters:
 *   line - A pointer to the `machineCodeLine` structure.
 *
 * Returns:
 *   The integer representation of the machine code line.
 */
int machineCodeLineToInt(machineCodeLine *line);


#endif