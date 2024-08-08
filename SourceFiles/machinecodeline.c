#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/machinecodeline.h"

void initializeMachineCodeLine(machineCodeLine *line) {
    /* Check if the line pointer is not NULL */
    if (line == NULL) return;

    /* Initialize the values array to zero */
    line->values[0] = 0;
    line->values[1] = 0;
}

void initializeMachineCodeLines(machineCodeLine *lines[], int size) {
    int i;

    /* Loop through each element in the lines array */
    for (i = 0; i < size; i++) {

        /* Allocate memory for a machineCodeLine structure */
        lines[i] = (machineCodeLine *)malloc(sizeof(machineCodeLine));
        if (lines[i] == NULL) {
            printGeneralError(ERROR_CODE_4);
            exit(EXIT_FAILURE);
        }

        /* Initialize the newly allocated machineCodeLine structure */
        initializeMachineCodeLine(lines[i]);
    }
}

void setMachineCodeARE(machineCodeLine *line, int location, bool value) {
    if (value)
        /* Set the bit at the specified location to 1 */
        line->values[0] |= 1 << location;
    else
        /* Clear the bit at the specified location */
        line->values[0] &= ~(1 << location);
}

void setMachineCodeValues(machineCodeLine *line, int location, bool value) {
    if (location >= SIZE_OF_ADDRESING_METHODS_IN_WORD) {
        if (value)
            /* Set the bit in the second half of values if the condition is met */
            line->values[1] |= 1 << (location - SIZE_OF_ADDRESING_METHODS_IN_WORD);
        else
            /* Clear the bit in the second half of values */
            line->values[1] &= ~(1 << (location - SIZE_OF_ADDRESING_METHODS_IN_WORD));
    }
    else {
        if (value)
            /* Set the bit in the first half of values if the condition is met */
            line->values[0] |= 1 << (location + SIZE_OF_ADDRESING_METHODS_IN_WORD);
        else
            /* Clear the bit in the first half of values */
            line->values[0] &= ~(1 << (location + SIZE_OF_ADDRESING_METHODS_IN_WORD));
    }
}

bool getMachineCodeARE(machineCodeLine *line, int location) {
    /* Extract and return the bit at the specified location */
    return (line->values[0] >> location) & 1;
}

bool getMachineCodeValues(machineCodeLine *line, int location) {
    if (location >= SIZE_OF_ADDRESING_METHODS_IN_WORD)
        /* Extract and return the bit from the second half of values */
        return (line->values[1] >> (location - SIZE_OF_ADDRESING_METHODS_IN_WORD)) & 1;
    
    /* Extract and return the bit from the first half of values */
    return (line->values[0] >> (location + SIZE_OF_ADDRESING_METHODS_IN_WORD)) & 1;
}

void insertOpcodeToMachineCodeLine(machineCodeLine *line, int opcode) /*set if opcode is ERROR*/ {
    int mask;

    /* Apply a mask to clear the current opcode value */
    mask = ORIGIN_MASK;
    line->values[1] &= mask;

    /* Insert the new opcode value */
    line->values[1] += (opcode << SIZE_OF_OPCODE);
}

void insertValuesToMachineCodeLine(machineCodeLine *line, int values) /*set if opcode is ERROR*/ {
    int mask;
    unsigned char firstHalf;
    unsigned char secondHalf;

    /* Apply a mask to clear the current values in the first half */
    mask = DEST_MASK >> SIZE_OF_ADDRESING_METHODS_IN_WORD;
    line->values[0] &= mask;

    /* Split and insert values into the appropriate halves */
    firstHalf = values << SIZE_OF_ADDRESING_METHODS_IN_WORD;
    line->values[0] += firstHalf;

    secondHalf = values >> SIZE_OF_ADDRESING_METHODS_IN_WORD;
    line->values[1] = secondHalf;
}

void setMachineCode3To5Values(machineCodeLine *line, int value) {
    int mask;

    /* Apply a mask to clear the current 3 to 5 values */
    mask = DEST_REGISTER_MASK;
    line->values[0] &= mask;

    /* Insert the new 3 to 5 values */
    line->values[0] += (value << SIZE_OF_ADDRESING_METHODS_IN_WORD);
}

void setMachineCode6To8Values(machineCodeLine *line, int value) {
    int mask;

    /* Apply a mask to clear the current 6 to 8 values */
    mask = ORIGIN_REGISTER_MASK;
    line->values[1] &= mask;

    /* Insert the new 6 to 8 values */
    line->values[1] += value >> 1;
    setMachineCodeValues(line, 3, value % 2);
}

void setMachineCode(machineCodeLine *line, int value) {
    int mask;

    /* Set the ARE values in the first half of the line */
    mask = ARE_MASK;
    line->values[0] = (unsigned char) value & mask;

    /* Insert the remaining values shifted to the appropriate position */
    line->values[0] += (unsigned char) ((value & ~mask) << 1);
    line->values[1] = (unsigned char) (value >> (SIZE_OF_VALUES_IN_WORD - 1));
}

int machineCodeLineToInt(machineCodeLine *line) {
    int sum;
    int mask;

    /* Calculate the sum of ARE values */
    mask = ARE_MASK;
    sum = line->values[0] & mask;

    /* Add the DEST values */
    mask = DEST_MASK;
    sum += (line->values[0] & mask) >> 1;

    /* Add the remaining values shifted to the appropriate position */
    sum += line->values[1] << (SIZE_OF_VALUES_IN_WORD - 1);

    return sum;
}
