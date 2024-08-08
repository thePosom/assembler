#ifndef ERRORS
#define ERRORS


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

/* Error structure to hold error information */
typedef struct Error {
    int error_id;      /* Unique identifier for the error */
    char *error_msg;   /* Description of the error */
} Error;

/* Enumeration of error codes */
typedef enum ERROR_CODES {
    ERROR_CODE_0 = 0,
    ERROR_CODE_1,
    ERROR_CODE_2,
    ERROR_CODE_3,
    ERROR_CODE_4,
    ERROR_CODE_5,
    ERROR_CODE_6,
    ERROR_CODE_7,
    ERROR_CODE_8,
    ERROR_CODE_9,
    ERROR_CODE_10,
    ERROR_CODE_11,
    ERROR_CODE_12,
    ERROR_CODE_13,
    ERROR_CODE_14,
    ERROR_CODE_15,
    ERROR_CODE_16,
    ERROR_CODE_17,
    ERROR_CODE_18,
    ERROR_CODE_19,
    ERROR_CODE_20,
    ERROR_CODE_21,
    ERROR_CODE_22,
    ERROR_CODE_23,
    ERROR_CODE_24,
    ERROR_CODE_25,
    ERROR_CODE_26,
    ERROR_CODE_27,
    ERROR_CODE_28,
    ERROR_CODE_29,
    ERROR_CODE_30
} ERROR_CODES;


/*
 * printGeneralError
 *
 * Prints a general error message based on the error code.
 *
 * Parameters:
 *   error_code - The error code that specifies the error.
 */
void printGeneralError(int error_code);

/*
 * printErrorByCurrentLine
 *
 * Prints an error message based on the error code, filename, and finding the row with the current line.
 *
 * Parameters:
 *   error_code - The error code that specifies the error.
 *   filename - The name of the file where the error occurred.
 *   lineToFind - A pointer to the line structure that represents the current line.
 */
void printErrorByCurrentLine(int error_code, char *filename, line *lineToFind);

/*
 * printErrorByBuffer
 *
 * Prints an error message based on the error code, filename, and finding the row with the buffer content.
 *
 * Parameters:
 *   error_code - The error code that specifies the error.
 *   filename - The name of the file where the error occurred.
 *   buffer - The content of the buffer where the error occurred.
 */
void printErrorByBuffer(int error_code, char *filename, char *buffer);

#endif