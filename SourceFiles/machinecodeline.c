#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/machinecodeline.h"

void initializeMachineCodeLine(machineCodeLine *line) {
    if (line == NULL) return;
    line->values[0] = 0;
    line->values[1] = 0;
}

void initializeMachineCodeLines(machineCodeLine *lines[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        lines[i] = (machineCodeLine *)malloc(sizeof(machineCodeLine));
        if (lines[i] == NULL) {
            perror("Error, not enough memory");
            exit(EXIT_FAILURE);
        }
        

        initializeMachineCodeLine(lines[i]);
    }
}

void setMachineCodeARE(machineCodeLine *line, int location, bool value) {
    if (value)
        line->values[0] |= 1<<location;
    else
        line->values[0] &= ~(1<<location);
}

void setMachineCodeValues(machineCodeLine *line, int location, bool value) {
    if (location >= 4) {
        if (value)
            line->values[1] |= 1 << (location - 4);
        else
            line->values[1] &= ~(1 << (location - 4));
    }
    else {
        if (value)
            line->values[0] |= 1 << (location + 4);
        else
            line->values[0] &= ~(1 << (location + 4));
    }
}

void setMachineCodeIsStart(machineCodeLine *line, bool value) {
    int mask;
    mask = 247; /* mask for 11110111*/
    line->values[0] &= mask;
    line->values[0] += (value << 3);
}

bool getMachineCodeIsStart(machineCodeLine *line) {
    return (line->values[0] >> 3) & 1 ;
}

bool getMachineCodeARE(machineCodeLine *line, int location) {
    return (line->values[0] >> location) & 1;
}

bool getMachineCodeValues(machineCodeLine *line, int location) {
    if (location >= 4)
        return ( line->values[1] >> (location - 4) ) & 1;
    return ( line->values[0] >> (location + 4) ) & 1;
}

int machineCodeToNum(machineCodeLine *line) { /*needed?*/
    int i;
    int pow;
    int num;

    num = 0;
    pow = 1;
    for (i = 0; i < 3; i++) {
        if (getMachineCodeARE(line, i) == true)
            num=+pow;
        pow *= 2;
    }

    for (i = 0; i < 15-3; i++) {
        if (getMachineCodeValues(line, i) == true)
            num=+pow;
        pow *= 2;
    }


    return num;
}

void insertOpcodeToMachineCodeLine (machineCodeLine *line, int opcode) /*set if opcode is ERROR*/ {
    int mask;
    mask = 15; /* mask for 00001111*/
    line->values[1] &= mask;
    line->values[1] += (opcode << 4);
}

void insertValuesToMachineCodeLine (machineCodeLine *line, int values) /*set if opcode is ERROR*/ {
    int mask;
    unsigned char firstHalf;
    unsigned char secondHalf;
    mask = 15; /* mask for 00001111*/
    line->values[0] &= mask;
    firstHalf = values << 4;
    line->values[0] += firstHalf;

    secondHalf = values >> (8 - 4);
    
    line->values[1] = secondHalf;
}

void setMachineCode3To5Values (machineCodeLine *line, int value) {
    int mask;
    mask = 143; /* mask for 10001111*/
    line->values[0] &= mask;
    line->values[0] += (value << 4);
}

void setMachineCode6To8Values (machineCodeLine *line, int value) {
    int mask;

    mask = 252; /* mask for 11111100*/
    line->values[1] &= mask;
    line->values[1] += value >> 1;
    setMachineCodeValues(line, 3, value%2);
}

void setMachineCode (machineCodeLine *line, int value) {
    int mask;

    mask = 7; /* mask for 00000111*/
    line->values[0] = (unsigned char) value & mask;
    line->values[0] += (unsigned char) ((value & ~mask) << 1);
    line->values[1] = (unsigned char) (value >> 7);
}

void machineCodeLineToString(machineCodeLine *line) {
    int i;
    for (i=11;i>7;i--) {
        printf("%d", getMachineCodeValues(line, i));
    }

    printf(" ");
    for (;i>3;i--) {
        printf("%d", getMachineCodeValues(line, i));
    }
    
    printf(" ");
    for (;i>=0;i--) {
        printf("%d", getMachineCodeValues(line, i));
    }

    printf("  ");
    for (i=2;i>=0;i--) {
        printf("%d", getMachineCodeARE(line, i));
    }

    printf("\n");
}

int machineCodeLineToInt(machineCodeLine *line) {
    int sum;
    int mask;

    mask = 7; /* mask for 00000111*/
    sum = line->values[0] & mask;

    mask = 240; /* mask for 11110000*/
    sum += (line->values[0] & mask) >> 1;

    sum += line->values[1] << 7;

    return sum;
}


