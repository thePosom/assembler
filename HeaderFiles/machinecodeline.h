#ifndef MACHINE_CODE_LINE
#define MACHINE_CODE_LINE

#include "util.h"

typedef struct {
    unsigned char values[2]; /* char[0] in 0-2 ARE, char[0] in 4-7 Dest, char[1] in 0-3 origin, char[1] in 4-7 opcode */
} machineCodeLine;



void setMachineCodeARE(machineCodeLine *line, int location, bool value);

void setMachineCodeValues(machineCodeLine *line, int location, bool value);

bool getMachineCodeARE(machineCodeLine *line, int location);

bool getMachineCodeValues(machineCodeLine *line, int location);

void initializeMachineCodeLine(machineCodeLine *line);

void initializeMachineCodeLines(machineCodeLine *lines[], int size);

void insertOpcodeToMachineCodeLine (machineCodeLine *line, int opcode);

void insertValuesToMachineCodeLine (machineCodeLine *line, int values);

void setMachineCode3To5Values (machineCodeLine *line, int value);

void setMachineCode6To8Values (machineCodeLine *line, int value);

void machineCodeLineToString(machineCodeLine *line);

void setMachineCode (machineCodeLine *line, int value);

#endif