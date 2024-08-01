#ifndef PRE_ASSEMBLER
#define PRE_ASSEMBLER

#include "hashmap.h"
#include "globals.h"
#include "util.h"

void expandMacros(char *filesName);

void startOfMacroDefinition(line *currentLine, lineList **lines, char **macroName, bool *macro);

void insideMacroDefinition (line *currentLine, lineList *lines, char *macroName, char* buffer, bool *macro);

void callForMacro(line *currentLine, FILE *amFile, char *buffer);

#endif