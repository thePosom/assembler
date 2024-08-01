#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/machinecodeline.h"

void initializeMachineCodeLine(machineCodeLine *line) {
    if (line == NULL) return;
    line->values[0] = 0;
    line->values[1] = 0;
}

void initializeMachineCodeLines(machineCodeLine *lines[3]) {
    int i;
    for (i = 0; i < 3; i++) {
        lines[i] = (machineCodeLine *)malloc(sizeof(machineCodeLine));
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
    
    line->values[0] = secondHalf;
}

void setMachineCodeDestValue (machineCodeLine *line, int value) {
    int mask;
    mask = 135; /* mask for 00001111*/
    line->values[0] &= mask;
    line->values[0] += (value << 4);
}

void setMachineCodeOriginValue (machineCodeLine *line, int value) {
    int mask;

    mask = 240; /* mask for 11110000*/
    line->values[0] &= mask;
    line->values[0] += value;
}
