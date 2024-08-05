#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/preAssembler.h"

void expandMacros(char *filesName) {

    FILE *asFile, *amFile;
    char buffer[MAX_LINE_SIZE];
    char *macroName;
    bool macro = false;
    lineList *lines;
    
    lines = NULL;
    macroName = NULL;

    asFile = openFile(filesName, ".as", "r");
    amFile = openFile(filesName, ".am", "w");

    if (asFile == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    initializeMacroTable();
    initializeMacroNamesTable();


    while (fgets (buffer, sizeof(buffer), asFile) != NULL ) {

        line* currentLine = (line *)malloc(sizeof(line));
        if (currentLine == NULL) {
            perror("Error, not enough memory");
            exit(EXIT_FAILURE);
        }
        
        lineToArrayOfWords(buffer, currentLine);

        if (currentLine->wordsCount != 0 && currentLine->wordsArray[0][0] != ';') {

            if ( strcmp(currentLine->wordsArray[0], "macr") == 0 )
                startOfMacroDefinition(currentLine, &lines, &macroName, &macro);

            else if (macro)
                insideMacroDefinition(currentLine, lines, macroName, buffer, &macro);

            else if (currentLine->wordsCount == 1 ) 
                callForMacro(currentLine, amFile, buffer);

            else
                fprintf(amFile, "%s",buffer);
        }
        

        free(currentLine);
    }

    freeMacroTable();
    fflush(amFile);
    fclose(asFile);
    fclose(amFile);
}

void startOfMacroDefinition(line *currentLine, lineList **lines, char **macroName, bool *macro) {
    if ((currentLine->wordsCount != 2)) {
        /* do errory things*/
    }
    else if ( isNameLegal (currentLine->wordsArray[1]) ) {
        *macro = true;
        *macroName = my_strdup(currentLine->wordsArray[1]);
        *lines = (lineList *)malloc(sizeof(lineList));
        if (*lines == NULL) {
            perror("Error, not enough memory");
            exit(EXIT_FAILURE);
        }

        (*lines)->head = NULL;
        (*lines)->end = NULL;
    }
    else {
        /* do errory things*/
    }
}

void insideMacroDefinition (line *currentLine, lineList *lines, char *macroName, char* buffer, bool *macro) {
    char *lineText;
    lineNode *pNode;

    if ( strcmp(currentLine->wordsArray[0], "endmacr") == 0 ){
        if (currentLine->wordsCount == 1) {
            *macro = false;
            insertToMacroTable(lines, macroName);
            free(macroName);
        }
        else {
            free(macroName);
            /* do errory things*/
        }
    }
    else {
        pNode = (lineNode *)malloc(sizeof(lineNode));
        if (pNode == NULL) {
            perror("Error, not enough memory");
            exit(EXIT_FAILURE);
        }

        lineText = my_strdup(buffer);
        pNode->lineText = lineText;
        pNode->next = NULL;

        insertLineNodeToEnd(lines, pNode);
    }
}

void callForMacro(line *currentLine, FILE *amFile, char *buffer) {
    lineList *macroLines;
    lineNode *macroLineNode;
    macroLines = getMacroLines (currentLine->wordsArray[0]);

    if (macroLines)
        macroLineNode = macroLines->head;
    else { 
        fprintf(amFile, "%s",buffer);
        macroLineNode = NULL;
    }

    while (macroLineNode) {
        fprintf(amFile, "%s",macroLineNode->lineText);
        macroLineNode = macroLineNode->next;
    }
}

