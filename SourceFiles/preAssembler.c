#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/preAssembler.h"

/*
 * startOfMacroDefinition
 *
 * This function is called when the pre-assembler encounters the start of a
 * macro definition (indicated by the 'macr' keyword). It initializes the data
 * structures needed to store the macro definition, and sets the 'macro' flag
 * to indicate that the pre-assembler is currently processing a macro
 * definition.
 *
 * Parameters:
 *   currentLine - The current line of the input assembly code.
 *   lines       - A pointer to the head of the linked list of lines that make
 *                 up the macro definition.
 *   macroName   - A pointer to the name of the macro being defined.
 *   macro       - A pointer to a boolean flag indicating whether the
 *                 pre-assembler is currently processing a macro definition.
 */
void startOfMacroDefinition(line *currentLine, lineList **lines, char **macroName, bool *macro);

/*
 * insideMacroDefinition
 *
 * This function is called when the pre-assembler is processing the lines that
 * make up a macro definition. It adds each line to the linked list of lines
 * that make up the macro definition, and handles the end of the macro
 * definition (indicated by the 'endmacr' keyword).
 *
 * Parameters:
 *   currentLine - The current line of the input assembly code.
 *   lines       - The linked list of lines that make up the macro definition.
 *   macroName   - The name of the macro being defined.
 *   buffer      - The current line of the input assembly code as a string.
 *   macro       - A pointer to a boolean flag indicating whether the
 *                 pre-assembler is currently processing a macro definition.
 */
void insideMacroDefinition (line *currentLine, lineList *lines, char *macroName, char* buffer, bool *macro);


/*
 * callForMacro
 *
 * This function is called when the pre-assembler encounters a line in the
 * input assembly code that calls a macro. It retrieves the lines that make up
 * the macro definition from the macro table, and writes them to the output
 * file.
 *
 * Parameters:
 *   currentLine - The current line of the input assembly code.
 *   amFile      - The output file (*.am) where the expanded macro code will
 *                 be written.
 *   buffer      - The current line of the input assembly code as a string.
 */
void callForMacro(line *currentLine, FILE *amFile, char *buffer);


void expandMacros(char *filesName) {
    FILE *asFile;
    FILE *amFile;
    char buffer[MAX_LINE_SIZE];
    char *macroName;
    bool macro = false;
    lineList *lines;

    lines = NULL;
    macroName = NULL;

    /* Open the input and output files */
    asFile = openFile(filesName, ".as", "r");
    amFile = openFile(filesName, ".am", "w");

    /* Check if the files were opened successfully */
    if (asFile == NULL) {
        printGeneralError(ERROR_CODE_2);
        exit(EXIT_FAILURE);
    }

    if (amFile == NULL) {
        printGeneralError(ERROR_CODE_24);
        exit(EXIT_FAILURE);
    }

    /* Initialize the macro table and macro names table */
    initializeMacroTable();
    initializeMacroNamesTable();

    /* Process each line of the input file */
    while (fgets(buffer, sizeof(buffer), asFile) != NULL) {
        line *currentLine = (line *)malloc(sizeof(line));
        if (currentLine == NULL) {
            printGeneralError(ERROR_CODE_4);
            exit(EXIT_FAILURE);
        }

        lineToArrayOfWords(buffer, currentLine, filesName, false);

        if (currentLine->wordsCount != 0 && currentLine->wordsArray[0][0] != ';') {

            /* Handle the start of a macro definition */
            if (strcmp(currentLine->wordsArray[0], "macr") == 0)
                startOfMacroDefinition(currentLine, &lines, &macroName, &macro);

            /* Handle the lines inside a macro definition */
            else if (macro)
                insideMacroDefinition(currentLine, lines, macroName, buffer, &macro);

            /* Handle a call to a macro */
            else if (currentLine->wordsCount == 1)
                callForMacro(currentLine, amFile, buffer);
                
            /* Write the line to the output file */
            else
                fprintf(amFile, "%s", buffer);
        }

        freeLine(currentLine);
    }

    /* Free the macro table and close the input and output files */
    freeMacroTable();
    fflush(amFile);
    fclose(asFile);
    fclose(amFile);
}

void startOfMacroDefinition(line *currentLine, lineList **lines, char **macroName, bool *macro) {
    /* Set the macro flag to true to indicate that we are processing a macro definition */
    *macro = true;

    /* Allocate memory for the macro name and copy the name from the current line */
    *macroName = my_strdup(currentLine->wordsArray[1]);

    /* Allocate memory for the linked list of lines that make up the macro definition */
    *lines = (lineList *)malloc(sizeof(lineList));
    if (*lines == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    /* Initialize the linked list */
    (*lines)->head = NULL;
    (*lines)->end = NULL;
}

void insideMacroDefinition (line *currentLine, lineList *lines, char *macroName, char* buffer, bool *macro) {
    /* Check if the current line is the end of the macro definition */
    if (strcmp(currentLine->wordsArray[0], "endmacr") == 0) {
        /* Set the macro flag to false */
        *macro = false;

        /* Insert the macro definition into the macro table */
        insertToMacroTable(lines, macroName);

        /* Free the memory allocated for the macro name */
        free(macroName);
    }
    else {
        char *lineText;
        lineNode *pNode;

        /* Allocate memory for a new line node */
        pNode = (lineNode *)malloc(sizeof(lineNode));
        if (pNode == NULL) {
            printGeneralError(ERROR_CODE_4);
            exit(EXIT_FAILURE);
        }

        /* Duplicate the current line and add it to the linked list */
        lineText = my_strdup(buffer);
        pNode->lineText = lineText;
        pNode->next = NULL;
        insertLineNodeToEnd(lines, pNode);
    }
}

void callForMacro(line *currentLine, FILE *amFile, char *buffer) {
    lineList *macroLines;
    lineNode *macroLineNode;

    /* Retrieve the lines that make up the macro definition */
    macroLines = getMacroLines(currentLine->wordsArray[0]);

    /* If the macro definition is found, write the lines to the output file */
    if (macroLines)
        macroLineNode = macroLines->head;
    else {
        /* If the macro definition is not found, write the original line to the output file */
        fprintf(amFile, "%s", buffer);
        macroLineNode = NULL;
    }

    /* Write the macro definition lines to the output file */
    while (macroLineNode) {
        fprintf(amFile, "%s", macroLineNode->lineText);
        macroLineNode = macroLineNode->next;
    }
}

