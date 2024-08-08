#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/firstPassScript.h"

/*
 * isLegalAddresingMethod
 *
 * This function checks whether the given addressing method is legal for the
 * specified opcode.
 *
 * Parameters:
 *   opcode           - The opcode of the instruction.
 *   addressingMethod - The addressing method being used.
 *   isDest           - A boolean flag indicating whether the addressing method
 *                      is being used as a destination.
 *
 * Returns:
 *   True if the addressing method is legal, false otherwise.
 */
bool isLegalAddresingMethod(int opcode, int addressingMethod, bool isDest);

/*
 * labelErrors
 *
 * This function checks for errors related to labels, such as empty labels,
 * illegal label names, and duplicate labels.
 *
 * Parameters:
 *   currentLine - The current line of the input assembly code.
 *   name        - The name of the label.
 *   filename    - The name of the input file.
 *
 * Returns:
 *   True if an error occurred, false otherwise.
 */
bool labelErrors(line *currentLine, char *name, char *filename);

/*
 * lineToMachineCode
 *
 * This function generates most of the machine code for a single line of the input
 * assembly code, not including labels.
 *
 * Parameters:
 *   currentLine       - The current line of the input assembly code.
 *   startingWord      - The index of the first word in the line that needs
 *                       to be processed.
 *   lines             - An array of machine code lines to be populated.
 *   filename          - The name of the input file.
 *
 * Returns:
 *   The number of machine code lines generated for the current line, or
 *   ERROR if an error occurred.
 */
int lineToMachineCode(line *currentLine, int startingWord, machineCodeLine *lines[MAX_BINARY_WORDS_IN_LINE], char *filename);

/*
 * ifRegister
 *
 * This function handles the case where a register is used in the addressing
 * method of an instruction.
 *
 * Parameters:
 *   lines             - An array of machine code lines.
 *   num               - The register number.
 *   i                 - The index of the current argument in the line.
 *   firstIsRegister   - A pointer to a boolean flag indicating whether the
 *                       first argument is a register.
 *   size              - A pointer to the size of the line (in machine code
 *                       lines).
 *
 * Returns:
 *   The size modifier for the machine code.
 */
int ifRegister (machineCodeLine *lines[MAX_BINARY_WORDS_IN_LINE], int num, int i, bool *firstIsRegister, int *size);

/*
 * dataLine
 *
 * This function handles the processing of .data and .string in the
 * input assembly code.
 *
 * Parameters:
 *   currentLine       - The current line of the input assembly code.
 *   startingWord      - The index of the first word in the line that needs
 *                       to be processed.
 *   filename          - The name of the input file.
 *
 * Returns:
 *   The size of the data generated, or ERROR if an error occurred.
 */
int dataLine(line *currentLine, int startingWord, char *filename);

/*
 * externLine
 *
 * This function handles the processing of .extern in the input
 * assembly code.
 *
 * Parameters:
 *   currentLine       - The current line of the input assembly code.
 *   startingWord      - The index of the first word in the line that needs
 *                       to be processed.
 *   IC                - The current instruction counter.
 *   filename          - The name of the input file.
 *
 * Returns:
 *   True if the .extern was processed successfully, false otherwise.
 */
bool externLine(line *currentLine, int startingWord, int IC, char *filename);


bool firstPass(char *filesName) {

    FILE *amFile;
    machineCodeLine *machinecodeline[MAX_BINARY_WORDS_IN_LINE];
    
    bool isSymbolDef;
    bool errorOccurred;

    char buffer[MAX_LINE_SIZE];
    int IC, DC;
    

    IC = DC = 0;
    isSymbolDef = false;
    errorOccurred = false;

    amFile = openFile(filesName, ".am", "r");

    /* Open the input file */
    amFile = openFile(filesName, ".am", "r");
    if (amFile == NULL) {
        printGeneralError(ERROR_CODE_3);
        exit(EXIT_FAILURE);
    }

    /* Initialize the data array, instructions array, and symbol table */
    initializeDataArray();
    initializeInstructionsArray();
    initializeSymbolTable();

    /* Process each line of the input file */
    while (fgets(buffer, sizeof(buffer), amFile) != NULL) {
        int startingWord;
        int size;

        line *currentLine = (line *)malloc(sizeof(line));
        if (currentLine == NULL) {
            printGeneralError(ERROR_CODE_4);
            exit(EXIT_FAILURE);
        }

        lineToArrayOfWords(buffer, currentLine, filesName, true);

        if (currentLine->wordsCount == ERROR) {
            errorOccurred = true;
        } else if (currentLine->wordsCount > 0 && currentLine->wordsArray[0][0] != ';') {

            /* Check if the current line starts with a label */
            if (endsWithColon(currentLine->wordsArray[0])) {
                char *name;
                name = my_strdup(currentLine->wordsArray[0]);
                *(name + strlen(name) - 1) = '\0';

                /* Check for label errors */
                if (labelErrors(currentLine, name, filesName) == true) {
                    errorOccurred = true;
                    free(name);
                    freeLine(currentLine);
                    continue;
                }

                isSymbolDef = true;
                startingWord = 1;
            } else {
                isSymbolDef = false;
                startingWord = 0;
            }

            /* Handle .extern */
            if (strcmp(currentLine->wordsArray[startingWord], ".extern") == 0) {
                if (!externLine(currentLine, startingWord, IC, filesName)) {
                    errorOccurred = true;
                    freeLine(currentLine);
                    continue;
                }
            }

            /* Handle .data and .string */
            else if ((strcmp(currentLine->wordsArray[startingWord], ".data") == 0) || (strcmp(currentLine->wordsArray[startingWord], ".string") == 0)) {
                char *name;
                name = my_strdup(currentLine->wordsArray[0]);
                *(name + strlen(name) - 1) = '\0';

                if (isSymbolDef) {
                    insertToSymbolTable(name, ".data", DC);
                    free(name);
                }
                size = dataLine(currentLine, startingWord, filesName);
                if (size == ERROR)
                    errorOccurred = true;
                else
                    DC += size;
            }

            /* Handle instructions and labels */
            else if (isSymbolDef) {
                char *temp;

                temp = my_strdup(currentLine->wordsArray[0]);
                *(temp + strlen(temp) - 1) = '\0';
                insertToSymbolTable(temp, ".code", IC + MEMORY_START);
                free(temp);
            }

            /* Generate machine code for the current line */
            if (isInstruction(currentLine->wordsArray[startingWord])) {
                size = lineToMachineCode(currentLine, startingWord, machinecodeline, filesName);
                if (size == ERROR) {
                    errorOccurred = true;
                    freeLine(currentLine);
                    continue;
                }

                insertToInstructionsArray(machinecodeline, size);
                IC += size;
            }

            /* Handle unknown commands */
            else if ((strcmp(currentLine->wordsArray[startingWord], ".data") != 0) &&
                     (strcmp(currentLine->wordsArray[startingWord], ".string") != 0) &&
                     (strcmp(currentLine->wordsArray[startingWord], ".entry") != 0) &&
                     (strcmp(currentLine->wordsArray[startingWord], ".extern") != 0)) {
                printErrorByBuffer(ERROR_CODE_19, filesName, buffer);
                errorOccurred = true;
            }
        }

        freeLine(currentLine);
    }

    /* Add the data addresses to the symbol table */
    addToAllDataInSymbolTable(IC + MEMORY_START);
    fclose(amFile);

    return !errorOccurred;
}

bool externLine(line *currentLine, int startingWord, int IC, char *filename) {

    /* Check if the .extern has the correct number of arguments */
    if (currentLine->wordsCount - startingWord < 2) {
        printErrorByCurrentLine(ERROR_CODE_7, filename, currentLine);
        return false;
    } 

    else if (currentLine->wordsCount - startingWord > 2) {
        printErrorByCurrentLine(ERROR_CODE_20, filename, currentLine);
        return false;
    }

    /* Insert the external symbol to the symbol table */
    insertToSymbolTable(currentLine->wordsArray[startingWord + 1], ".external", IC + 1);
    return true;
}

bool labelErrors(line *currentLine, char *name, char *filename) {

    /* Check if the label is empty */
    if (currentLine->wordsCount <= 1) {
        printErrorByCurrentLine(ERROR_CODE_6, filename, currentLine);
        return true;
    }

    /* Check if the label name is legal */
    if (!isNameLegal(name)) {
        printErrorByCurrentLine(ERROR_CODE_8, filename, currentLine);
        return true;
    }

    /* Check if the label is a macro name or already defined in the symbol table */
    else if (isMacroName(name) || (getSymbolValue(name) != ERROR)) {
        printErrorByCurrentLine(ERROR_CODE_9, filename, currentLine);
        return true;
    }

    /* Check if the label name is too long */
    else if (strlen(name) > MAX_LABEL_SIZE) {
        printErrorByCurrentLine(ERROR_CODE_28, filename, currentLine);
        return true;
    }

    /* Check if the label name is empty */
    else if (strlen(name) == 0) {
        printErrorByCurrentLine(ERROR_CODE_29, filename, currentLine);
        return true;
    }

    /* Check if the first character of the label is not a letter */
    else if (!isLetter(name[0])) {
        printErrorByCurrentLine(ERROR_CODE_30, filename, currentLine);
        return true;
    }

    return false;
}

int dataLine(line *currentLine, int startingWord, char *filename) {
    int size;
    char *temp;
    int *dataNums;
    dataNums = NULL;

    /* Handle .data */
    if (strcmp(currentLine->wordsArray[startingWord], ".data") == 0) {

        /* Check if the .data has at least one argument */
        if (currentLine->wordsCount - startingWord <= 1) {
            printErrorByCurrentLine(ERROR_CODE_10, filename, currentLine);
            free(dataNums);
            return ERROR;
        }

        /* Parse the data array and insert it into the data array */
        size = parseDataArray(currentLine, startingWord, &dataNums, filename);
        if (size == ERROR) {
            return ERROR;
        }

        insertArrayToDataArray(dataNums, size);
        free(dataNums);
    }

    /* Handle .string */
    else {

        /* Remove the quotation marks from the string */
        temp = removeQuotions(currentLine->wordsArray[startingWord + 1]);
        size = strlen(temp) + 1;
        insertStringToDataArray(temp);
        free(temp);
    }

    return size;
}

int parseDataArray(line *currentLine, int startingWord, int **dataNums, char *filename) {
    int i;
    int size;
    char **dataString;

    dataString = NULL;

    /* Parse the comma-separated values */
    size = parseCommas(currentLine, startingWord, &dataString, filename);
    if (size == ERROR) {
        freeStringArray(dataString, size);
        *dataNums = NULL;
        return ERROR;
    }

    /* Allocate memory for the data array */
    *dataNums = (int *)malloc(size * sizeof(int));
    if (*dataNums == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    /* Convert the string values to integers and store them in the data array */
    for (i = 0; i < size; i++) {
        int num;
        char *str;

        str = *(dataString + i);
        num = stringToNum(str);
        if (num == INT_MIN) {
            printErrorByCurrentLine(ERROR_CODE_11, filename, currentLine);
            freeStringArray(dataString, size);
            return ERROR;
        }
        if ((num > my_pow(2, BINARY_WORD_SIZE - 1) - 1) || (num < -my_pow(2, BINARY_WORD_SIZE))) {
            printErrorByCurrentLine(ERROR_CODE_12, filename, currentLine);
            freeStringArray(dataString, size);
            return ERROR;
        }

        (*dataNums)[i] = num;
    }

    freeStringArray(dataString, size);

    return size;
}

int parseCommas(line *currentLine, int startingWord, char ***output, char *filename) {
    int i;
    int size;
    char *token;
    char *str;
    char *delimiter = ",";
    char temp[MAX_LINE_SIZE];

    *output = (char **)malloc(currentLine->wordsCount * sizeof(char*));
    if (*output == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    temp[0] = '\0';
    str = currentLine->wordsArray[currentLine->wordsCount - 1];

    /* Check for trailing commas */
    if (*(str + strlen(str) - 1) == ',') {
        printErrorByCurrentLine(ERROR_CODE_13, filename, currentLine);
        return ERROR;
    }

    str = currentLine->wordsArray[startingWord + 1];
    if (str == NULL) {
        free(*output);
        return 0;
    }

    /* Check for leading commas */
    if (*str == ',') {
        printErrorByCurrentLine(ERROR_CODE_14, filename, currentLine);
        return ERROR;
    }

    /* Concatenate the comma-separated values */
    for (i = startingWord + 1; i < currentLine->wordsCount; i++) {
        str = currentLine->wordsArray[i];
        if ((i < currentLine->wordsCount - 1) && (*(str + strlen(str) - 1) != ',') && (*(currentLine->wordsArray[i + 1]) != ',')) {
            printErrorByCurrentLine(ERROR_CODE_15, filename, currentLine);
            return ERROR;
        }
        strcat(temp, str);
    }

    size = 0;
    if (strstr(temp, ",,") != NULL) {
        printErrorByCurrentLine(ERROR_CODE_16, filename, currentLine);
        return ERROR;
    }

    /* Split the string into individual values */
    token = strtok(temp, delimiter);
    while (token != NULL) {
        *(*(output) + size) = my_strdup(token);
        size++;
        token = strtok(NULL, delimiter);
    }

    *output = (char **)realloc(*output, size * sizeof(char*));
    if (*output == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    return size;
}

char* removeQuotions(char *str) {
    char *copy;
    copy  = my_strdup(str + 1);
    *(copy + ( strlen(copy) - 1 ) ) = '\0';
    return copy;
}

int lineToMachineCode(line *currentLine, int startingWord, machineCodeLine *lines[MAX_BINARY_WORDS_IN_LINE], char *filename) {
    int i;
    int size;
    int opcode;
    int sizeModifier;
    char **arguments;
    bool firstIsRegister;

    arguments = NULL;

    /* Parse the arguments in the current line */
    size = parseCommas(currentLine, startingWord, &arguments, filename);
    if (size == ERROR)
        return ERROR;

    /* Initialize the machine code lines */
    initializeMachineCodeLines(lines, MAX_BINARY_WORDS_IN_LINE);
    opcode = instructionToOpcode(currentLine->wordsArray[startingWord]);

    /* Set the opcode in the first machine code line */
    insertOpcodeToMachineCodeLine(lines[0], opcode);
    setMachineCodeARE(lines[0], A_IN_ARE, true);

    /* Check if the number of arguments matches the expected number */
    if (size != getArgumentsCount(currentLine->wordsArray[startingWord])) {
        printErrorByCurrentLine(ERROR_CODE_17, filename, currentLine);
        freeStringArray(arguments, size);
        return ERROR;
    }

    firstIsRegister = false;
    sizeModifier = 0;

    /* Process each argument in the current line */
    for (i = 0; i < size; i++) {
        int num;
        int addressingMethod;

        /* Handle immediate addressing mode */
        if (**(arguments + i) == '#') {
            addressingMethod = IMMEDIATE_ADDRESING_METHOD;
            num = stringToNum(*(arguments + i) + 1);
            if (num == INT_MIN) {
                printErrorByCurrentLine(ERROR_CODE_11, filename, currentLine);
                freeStringArray(arguments, size);
                return ERROR;
            }
            if ((num > my_pow(2, BINARY_WORD_SIZE - ARE_SIZE - 1) - 1) || (num < -my_pow(2, BINARY_WORD_SIZE - ARE_SIZE - 1))) {
                printErrorByCurrentLine(ERROR_CODE_12, filename, currentLine);
                freeStringArray(arguments, size);
                return ERROR;
            }

            setMachineCodeARE(lines[i + 1], A_IN_ARE, true);
            insertValuesToMachineCodeLine(lines[i + 1], num);
        }
        /* Handle indirect register addressing mode */
        else if ((**(arguments + i) == '*') && (isRegister(*(arguments + i) + 1))) {
            addressingMethod = INDIRECT_REGISTER_ADDRESING_METHOD;
            num = getRegisterNum(*(arguments + i) + 1);

            sizeModifier = ifRegister(lines, num, i, &firstIsRegister, &size);
        }
        /* Handle direct register addressing mode */
        else if (isRegister(*(arguments + i))) {
            addressingMethod = DIRECT_REGISTER_ADDRESING_METHOD;
            num = getRegisterNum(*(arguments + i));

            sizeModifier = ifRegister(lines, num, i, &firstIsRegister, &size);
        }
        /* Handle direct addressing mode */
        else {
            if (!isNameLegal(*(arguments + i))) {
                printErrorByCurrentLine(ERROR_CODE_25, filename, currentLine);
                freeStringArray(arguments, size);
                return ERROR;
            }
            addressingMethod = DIRECT_ADDRESING_METHOD;
        }

        /* Check if the addressing method is legal for the current opcode */
        if (isLegalAddresingMethod(opcode, addressingMethod, (i + 1 == size)) == false) {
            printErrorByCurrentLine(ERROR_CODE_8, filename, currentLine);
            freeStringArray(arguments, size);
            return ERROR;
        }

        /* Set the addressing method in the first machine code line */
        if (i + 1 == size)
            setMachineCodeValues(lines[0], 0 + addressingMethod, true);
        else
            setMachineCodeValues(lines[0], SIZE_OF_ADDRESING_METHODS_IN_WORD + addressingMethod, true);
    }

    freeStringArray(arguments, size);

    return size + sizeModifier + 1;
}

int ifRegister(machineCodeLine *lines[MAX_BINARY_WORDS_IN_LINE], int num, int i, bool *firstIsRegister, int *size) {
    int sizeModifier;
    sizeModifier = 0;

    /* Adjust the size modifier if the first argument is a register */
    if (*firstIsRegister)
        sizeModifier = -1;

    /* Set the register number in the appropriate machine code line */
    if (i + 1 == *size)
        setMachineCode3To5Values(lines[i + sizeModifier + 1], num);
    else
        setMachineCode6To8Values(lines[i + sizeModifier + 1], num);

    setMachineCodeARE(lines[i + 1], A_IN_ARE, true);

    *firstIsRegister = true;

    return sizeModifier;
}

bool isLegalAddresingMethod(int opcode, int addressingMethod, bool isDest) {
    switch (addressingMethod) {
    case IMMEDIATE_ADDRESING_METHOD:
        if (isDest == true) 
            return (opcode == CMP_OPCODE || opcode == PRN_OPCODE);
        return (opcode != LEA_OPCODE);

    case DIRECT_ADDRESING_METHOD:
        return true;
      
    case INDIRECT_REGISTER_ADDRESING_METHOD:
        return (opcode != LEA_OPCODE) || (isDest == true);
      
    case DIRECT_REGISTER_ADDRESING_METHOD:
        return ( !(opcode == LEA_OPCODE && isDest == false) && opcode != JSR_OPCODE && opcode != JMP_OPCODE && opcode != BNE_OPCODE );
    
    }

    return false;
}



