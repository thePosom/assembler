#ifndef UTIL
#define UTIL

#include "globals.h"

#define MAX_WORDS_IN_LINE 10

#define MOV_OPCODE 0
#define CMP_OPCODE 1
#define ADD_OPCODE 2
#define SUB_OPCODE 3
#define LEA_OPCODE 4
#define CLR_OPCODE 5
#define NOT_OPCODE 6
#define INC_OPCODE 7
#define DEC_OPCODE 8
#define JMP_OPCODE 9
#define BNE_OPCODE 10
#define RED_OPCODE 11
#define PRN_OPCODE 12
#define JSR_OPCODE 13
#define RTS_OPCODE 14
#define STOP_OPCODE 15

/*
 * bool
 *
 * A simple enum to represent boolean values in C.
 *
 * Values:
 *   false - Represents the boolean value false.
 *   true  - Represents the boolean value true.
 */
typedef enum { false, true } bool;

/*
 * line
 *
 * A structure representing a line of assembly code, broken down into words.
 *
 * Fields:
 *   wordsCount - The number of words in the line.
 *   wordsArray - An array of strings, each representing a word in the line.
 */
typedef struct line{
    int wordsCount;
    char *wordsArray[MAX_WORDS_IN_LINE];
} line;


/*
 * lineToArrayOfWords
 *
 * This function takes a string representing a line of text, and splits it into individual words,
 * which are then stored in a `line` structure.
 *
 * Parameters:
 *   str            - The input string to be split.
 *   currentLine    - A pointer to a `line` structure where the words will be stored.
 *   filename       - The name of the file being processed.
 *   checkForErrors - A boolean flag indicating whether to check for errors.
 *
 */
void lineToArrayOfWords(char *str, line *currentLine, char *filename, bool checkForErrors);

/*
 * getArgumentsCount
 *
 * Determines the number of arguments required by a given assembly operation.
 *
 * Parameters:
 *   operation - A string representing the operation name.
 *
 * Returns:
 *   The number of arguments expected by the operation, or ERROR if the operation is not recognized.
 */
int getArgumentsCount(char *operation); 

/*
 * isNameLegal
 *
 * Checks if a given name is a legal label or register name in the assembly code.
 *
 * Parameters:
 *   name - A string representing the name to check.
 *
 * Returns:
 *   true if the name is legal, false otherwise.
 */
bool isNameLegal(char *name);

/*
 * isInstruction
 *
 * Checks if a given name corresponds to a valid assembly instruction.
 *
 * Parameters:
 *   name - A string representing the name to check.
 *
 * Returns:
 *   true if the name is an instruction, false otherwise.
 */
bool isInstruction(char *name);

/*
 * my_strdup
 *
 * Duplicates a string by allocating memory and copying the content of the source string.
 *
 * Parameters:
 *   src - The source string to duplicate.
 *
 * Returns:
 *   A pointer to the newly allocated string, or exits the program if memory allocation fails.
 */
char *my_strdup(const char *src);

/*
 * instructionToOpcode
 *
 * Converts an instruction name to its corresponding opcode.
 *
 * Parameters:
 *   instruction - A string representing the instruction name.
 *
 * Returns:
 *   The opcode associated with the instruction, or ERROR if the instruction is not recognized.
 */
int instructionToOpcode (char *instruction);

/*
 * my_pow
 *
 * Computes the power of a given integer base raised to an unsigned integer exponent.
 *
 * Parameters:
 *   a - The base.
 *   b - The exponent.
 *
 * Returns:
 *   The result of raising the base `a` to the power `b`.
 */
int my_pow(int a, unsigned int b);

/*
 * freeStringArray
 *
 * Frees a dynamically allocated array of strings.
 *
 * Parameters:
 *   arr  - A pointer to the array of strings.
 *   size - The number of strings in the array.
 *
 * Returns:
 *   void
 */
void freeStringArray (char **arr, int size);

/*
 * stringToNum
 *
 * Converts a string representing a number into an integer.
 *
 * Parameters:
 *   str - The input string to convert.
 *
 * Returns:
 *   The integer value represented by the string, or INT_MIN if conversion fails.
 */
int stringToNum(char *str);

/*
 * isRegister
 *
 * Checks if a given string represents a valid register in the assembly language.
 *
 * Parameters:
 *   str - A string representing the register name.
 *
 * Returns:
 *   true if the string is a valid register, false otherwise.
 */
bool isRegister(char *str);

/*
 * getRegisterNum
 *
 * Extracts the register number from a register string.
 *
 * Parameters:
 *   str - A string representing the register name.
 *
 * Returns:
 *   The register number as an integer.
 */
int getRegisterNum (char *str);

/*
 * endsWithColon
 *
 * Checks if a given string ends with a colon character.
 *
 * Parameters:
 *   str - A string to check.
 *
 * Returns:
 *   true if the string ends with a colon, false otherwise.
 */
bool endsWithColon(char *str);

/*
 * openFile
 *
 * Opens a file with a specified file name and mode.
 *
 * Parameters:
 *   fileName  - The name of the file to open.
 *   fileType  - The file extension to append to the file name.
 *   openType  - The mode in which to open the file (e.g., "r", "w").
 *
 * Returns:
 *   A pointer to the opened file, or NULL if the file could not be opened.
 */
FILE* openFile(char *fileName, char *fileType, char* openType);

/*
 * freeLine
 *
 * Frees the memory associated with a line structure, including its words array.
 *
 * Parameters:
 *   lineToFree - A pointer to the line structure to free.
 *
 * Returns:
 *   void
 */
void freeLine(line *lineToFree);

/*
 * findRowInOriginFileByBuffer
 *
 * Finds the row number of a specific buffer string in the original assembly file.
 *
 * Parameters:
 *   filesName - The name of the assembly file.
 *   buffer    - The buffer string to search for.
 *
 * Returns:
 *   The row number where the buffer string is found, or ERROR if not found.
 */
int findRowInOriginFileByBuffer(char *filesName, char *buffer);

/*
 * findRowInOriginFileByCurrentLine
 *
 * Finds the row number of a specific line structure in the original assembly file.
 *
 * Parameters:
 *   filesName   - The name of the assembly file.
 *   lineToFind  - A pointer to the line structure to search for.
 *
 * Returns:
 *   The row number where the line is found, or ERROR if not found.
 */
int findRowInOriginFileByCurrentLine(char *filesName, line *lineToFind);

/*
 * isEqualLines
 *
 * Compares two line structures to determine if they are equal.
 *
 * Parameters:
 *   line1 - A pointer to the first line structure.
 *   line2 - A pointer to the second line structure.
 *
 * Returns:
 *   true if the lines are equal, false otherwise.
 */
bool isEqualLines(line *line1, line *line2);

/*
 * isLetter
 *
 * Checks if a given character is a letter (A-Z or a-z).
 *
 * Parameters:
 *   c - The character to check.
 *
 * Returns:
 *   true if the character is a letter, false otherwise.
 */
bool isLetter(char c);

/*
 * isAnyRegister
 *
 * Checks if a given string represents a register or a register with a prefix (e.g., "*r").
 * Assumes the register num is legel
 *
 * Parameters:
 *   str - A string representing the register.
 *
 * Returns:
 *   true if the string is a valid register or register with prefix, false otherwise.
 */
bool isAnyRegister(char *str);

#endif