#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../HeaderFiles/errors.h"

/* The Following array specifies the error code numbers and the corresponding error message. */
Error errors[] = {
        {ERROR_CODE_0,  "No Error"},
        {ERROR_CODE_1,  "Didn't include files name as input"},
        {ERROR_CODE_2,  ".as file is missing"},
        {ERROR_CODE_3,  "Error - while opening .am file"},
        {ERROR_CODE_4,  "Error - not enough memory"},
        {ERROR_CODE_5,  "Quote marks set incorrectly"},
        {ERROR_CODE_6,  "Missing command after label"},
        {ERROR_CODE_7,  "Missing label after .extern"},
        {ERROR_CODE_8,  "Illegel label name"},
        {ERROR_CODE_9,  "Label name defined twice"},
        {ERROR_CODE_10,  "Missing values in .data"},
        {ERROR_CODE_11,  "Non-number in place of a number"},
        {ERROR_CODE_12,  "Num too big or small"},
        {ERROR_CODE_13,  "Extra comma at the end"},
        {ERROR_CODE_14,  "Extra comma at the start"},
        {ERROR_CODE_15,  "Missing comma between arguments"},
        {ERROR_CODE_16,  "Double commas"},
        {ERROR_CODE_17,  "Incorrect number of arguments"},
        {ERROR_CODE_18,  "Unknown label"},
        {ERROR_CODE_19,  "Invalid command"},
        {ERROR_CODE_20,  "Extra text after .extern name"},
        {ERROR_CODE_21,  "Missing label after .entry"},
        {ERROR_CODE_22,  "Extra text after .entry name"},
        {ERROR_CODE_23,  ".entry for a label that's isn't in file"},
        {ERROR_CODE_24,  "Failed creating file"},
        {ERROR_CODE_25,  "Illegel addresing method for this opcode"},
        {ERROR_CODE_26,  "Error - while opening .as file"},
        {ERROR_CODE_27,  "Extra text after .string quotes"},
        {ERROR_CODE_28,  "Label too long"},
        {ERROR_CODE_29,  "Empty Label"},
        {ERROR_CODE_30,  "Label doesn't start with a letter"}

};

void printErrorByBuffer(int error_code, char *filename, char *buffer) {
    int row;

    /* Find the row number corresponding to the buffer content */
    row = findRowInOriginFileByBuffer(filename, buffer);

    /* Print the error message with error code, filename, and row number */
    printf("ERROR: ID: %d in %s.as at line: %d | %s\n", error_code, filename, row, errors[error_code].error_msg);
}

void printErrorByCurrentLine(int error_code, char *filename, line *lineToFind) {
    int row;

    /* Find the row number corresponding to the current line */
    row = findRowInOriginFileByCurrentLine(filename, lineToFind);

    /* Print the error message with error code, filename, and row number */
    printf("ERROR: ID: %d in %s.as at line: %d | %s\n", error_code, filename, row, errors[error_code].error_msg);
}

void printGeneralError(int error_code) {
    /* Print the general error message with error code */
    printf("ERROR: ID: %d | %s\n", error_code, errors[error_code].error_msg);
}