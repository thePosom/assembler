#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/secondPassScript.h"

/*
* entryLine
*
* Processes the .entry line and updates the symbol table accordingly.
*
* Parameters:
*   currentLine - The current line being processed.
*   startingWord - The starting word index for the current line.
*   filename - The name of the input file.
*
* Returns:
*   true if the .entry line was processed successfully, false otherwise.
*/
bool entryLine(line *currentLine, int startingWord, char *filename);

/*
* buildFiles
*
* Builds the output files (object file, entry file, and external file) based on the information gathered during the second pass.
*
* Parameters:
*   filesName - The base name of the input file.
*   entryExists - True if an .entry directive was found, false otherwise.
*   externArray - The array of external symbols.
*   ICTotal - The total number of instructions.
*   DCTotal - The total number of data words.
*/
void buildFiles(char *filesName, bool entryExists, externNode *externArray, int ICTotal, int DCTotal);

/*
* bothRegisters
*
* Checks if the given arguments represent two registers.
*
* Parameters:
*   arguments - The array of arguments to check.
*   size - The size of the arguments array.
*
* Returns:
*   true if the arguments represent two registers, false otherwise.
*/
bool bothRegisters(char **arguments, int size);

/*
* printExternArrayToFile
*
* Prints the external array to the specified file.
*
* Parameters:
*   node - The head of the external array.
*   file - The file to write the external array to.
*/
void printExternArrayToFile(externNode *node, FILE *file);

/*
* finalizeInstructionsArray
*
* Finalizes the instructions array by filling in the values and address of labels.
*
* Parameters:
*   currentLine - The current line being processed.
*   startingWord - The starting word index for the current line.
*   externArray - A pointer to the external array.
*   IC - The current instruction counter.
*   filename - The name of the input file.
*
* Returns:
*   The number of words added to the instructions array.
*/
int finalizeInstructionsArray(line *currentLine, int startingWord, externNode **externArray , int IC, char *filename);

/*
* printObFile
*
* Prints the object file with the given information.
*
* Parameters:
*   ICTotal - The total number of instructions.
*   DCTotal - The total number of data words.
*   filename - The name of the input file.
*/
void printObFile(int ICTotal, int DCTotal, char *filename);


void secondPass(char *filesName) {
    int IC;
    int DC;
    bool entryExists;
    bool errorOccurred;
    externNode *externArray;
    FILE *amFile;
    char buffer[MAX_LINE_SIZE];
 
    /* Initialize variables */
    IC = 0;
    DC = 0;
    entryExists = false;
    errorOccurred = false;
    externArray = NULL;
 
    /* Open the .am file */
    amFile = openFile(filesName, ".am", "r");
    if (amFile == NULL) {
        printGeneralError(ERROR_CODE_3);
        exit(EXIT_FAILURE);
    }
 
    /* Process each line in the .am file */
    while (fgets(buffer, sizeof(buffer), amFile) != NULL) {
 
        line *currentLine = (line *)malloc(sizeof(line));
        if (currentLine == NULL) {
            printGeneralError(ERROR_CODE_4);
            exit(EXIT_FAILURE);
        }
 
        lineToArrayOfWords(buffer, currentLine, filesName, false);
 
        if (currentLine->wordsCount > 0 && currentLine->wordsArray[0][0] != ';') {
            int startingWord;
 
            /* Determine the starting word index */
            if (endsWithColon(currentLine->wordsArray[0]))
                startingWord = 1;
            else
                startingWord = 0;
 
            /* Process different types of lines */
            if (strcmp(currentLine->wordsArray[startingWord], ".entry") == 0) {
                if (!entryLine(currentLine, startingWord, filesName)) {
                    errorOccurred = true;
                    freeLine(currentLine);
                    continue;
                }
                entryExists = true;
            }
            
            else if (strcmp(currentLine->wordsArray[startingWord], ".extern") == 0) {
                freeLine(currentLine);
                continue;
            }

            else if ((strcmp(currentLine->wordsArray[startingWord], ".data") == 0) || (strcmp(currentLine->wordsArray[startingWord], ".string") == 0)) {
                int size;
                char *temp;
                int *dataNums;
                dataNums = NULL;
 
                if (strcmp(currentLine->wordsArray[startingWord], ".data") == 0) {
                    size = parseDataArray(currentLine, startingWord, &dataNums, filesName);
                    free(dataNums);
                }
                else {
                    temp = removeQuotions(currentLine->wordsArray[startingWord+1]);
                    size = strlen(temp) + 1;
                    free(temp);
                }
 
                DC += size;
            }
            else {
                IC += finalizeInstructionsArray(currentLine, startingWord, &externArray, IC, filesName);
            }
        }
 
        freeLine(currentLine);
    }
 
    /* Build the output files */
    if (!errorOccurred)
        buildFiles(filesName, entryExists, externArray, IC, DC);
    else {
        freeExternArray(externArray);
        freeInstructionsArray();
        freeMacroNamesTable();
        freeSymbolTable();
        freeDataArray();
    }
}

bool bothRegisters(char **arguments, int size) {
    return size == 2 && isAnyRegister(arguments[0]) && isAnyRegister(arguments[1]);
}

int finalizeInstructionsArray(line *currentLine, int startingWord, externNode **externArray, int IC, char *filename) {
    int i;
    int size;
    int addToIC;
    int value;
    char **arguments;
    machineCodeLine *binaryLine;
 
    arguments = NULL;
    addToIC = 0;
 
    /* Parse the arguments from the current line */
    size = parseCommas(currentLine, startingWord, &arguments, filename);
 
    /* If the arguments don't represent two registers, process them */
    if (!bothRegisters(arguments, size)) {
        for (i = 1; i <= size; i++) {
            binaryLine = getFromInstructionsArray(IC + i);
 
            /* Check if the line is a label */
            if (binaryLine->values[0] == 0) {
                value = getSymbolValue(arguments[i - 1]);
                if (value == ERROR) {
                    printErrorByCurrentLine(ERROR_CODE_18, filename, currentLine);
                    continue;
                }
 
                insertValuesToMachineCodeLine(binaryLine, value);
 
                if (strcmp(getSymbolType(arguments[i - 1]), ".external") == 0) {
                    *externArray = insertToExternArray(*externArray, arguments[i - 1], IC + i + MEMORY_START);
                    setMachineCodeARE(binaryLine, E_IN_ARE, true);
                }
                else
                    setMachineCodeARE(binaryLine, R_IN_ARE, true);
            }
        }
        addToIC++;
    }
 
    addToIC += size;
    freeStringArray(arguments, size);
 
    return addToIC;
}

bool entryLine(line *currentLine, int startingWord, char *filename) {
   int value;

   /* Check the format of the .entry line */
   if (currentLine->wordsCount - startingWord < 2) {
       printErrorByCurrentLine(ERROR_CODE_21, filename, currentLine);
       return false;
   }
   else if (currentLine->wordsCount - startingWord > 2) {
       printErrorByCurrentLine(ERROR_CODE_22, filename, currentLine);
       return false;
   }

   /* Get the symbol value and insert it into the symbol table */
   value = getSymbolValue(currentLine->wordsArray[startingWord + 1]);
   if (value == ERROR) {
       printErrorByCurrentLine(ERROR_CODE_23, filename, currentLine);
       return false;
   }

   insertToSymbolTable(currentLine->wordsArray[startingWord + 1], ".entry", value);
   return true;
}

void buildFiles(char *filesName, bool entryExists, externNode *externArray, int ICTotal, int DCTotal) {
    FILE *entFile;
    FILE *extFile;
 
    /* Print the object file */
    printObFile(ICTotal, DCTotal, filesName);
 
    /* If an .entry was found, create the .ent file */
    if (entryExists) {
        entFile = openFile(filesName, ".ent", "w");
        if (entFile == NULL) {
            printGeneralError(ERROR_CODE_24);
            exit(EXIT_FAILURE);
        }
        printToFileByType(entFile, ".entry");
        fflush(entFile);
        fclose(entFile);
    }
 
    /* If there are any external symbols, create the .ext file */
    if (externArray != NULL) {
        extFile = openFile(filesName, ".ext", "w");
        if (extFile == NULL) {
            printGeneralError(ERROR_CODE_24);
            exit(EXIT_FAILURE);
        }
        printExternArrayToFile(externArray, extFile);
        fflush(extFile);
        fclose(extFile);
    }
 
    /* Free the allocated resources */
    freeExternArray(externArray);
    freeInstructionsArray();
    freeMacroNamesTable();
    freeSymbolTable();
    freeDataArray();
} 

void intToCharsInNBase(int num, char arr[], int size, int base) {
    int i;
    unsigned int u_num;
 
    /* Convert the integer value to an unsigned integer */
    u_num = (unsigned int)num;
 
    /* Convert the unsigned integer to a character array in the specified base */
    for (i = 0; i < size; i++) {
        arr[size - i - 1] = (u_num % base) + '0';
        u_num /= base;
    }
    arr[size] = '\0';
} 

void printObFile(int ICTotal, int DCTotal, char *filename) {
    int IC;
    int DC;
    int size;
    int value;
    char address[SIZE_OF_ADDRESS + 1];
    char octalLine[SIZE_OF_VALUE_IN_OB + 1];
 
    FILE *obFile;
    machineCodeLine *binaryLine;
    machineCodeLine *nextLine;
 
    /* Open the object file for writing */
    obFile = openFile(filename, ".ob", "w");
    if (obFile == NULL) {
        printGeneralError(ERROR_CODE_24);

        freeInstructionsArray();
        freeMacroNamesTable();
        freeSymbolTable();
        freeDataArray();
        return;
    }
 
    /* Print the header line with the total number of instructions and data words */
    fprintf(obFile, "%d %d\n", ICTotal, DCTotal);
 
    /* Print the instructions */
    IC = 0;
    binaryLine = getFromInstructionsArray(IC);
    nextLine = getFromInstructionsArray(IC+1);
    while (nextLine != NULL) {
        intToCharsInNBase(IC + MEMORY_START, address, SIZE_OF_ADDRESS, ADDRRESS_BASE);
        intToCharsInNBase(machineCodeLineToInt(binaryLine), octalLine, SIZE_OF_VALUE_IN_OB, VALUE_IN_OB_BASE);
 
        fprintf(obFile, "%s %s\n", address, octalLine);
 
        IC++;
        binaryLine = getFromInstructionsArray(IC);
        nextLine = getFromInstructionsArray(IC+1);
    }
 
    /* Print the data words */
    size = getDataArraySize();
    for (DC = 0; DC < size; DC++) {
        value = getFromDataArray(DC);
        intToCharsInNBase(DC + IC + MEMORY_START, address, SIZE_OF_ADDRESS, ADDRRESS_BASE);
        intToCharsInNBase(value, octalLine, SIZE_OF_VALUE_IN_OB, VALUE_IN_OB_BASE);
 
        fprintf(obFile, "%s %s\n", address, octalLine);
    }
 
    /* Flush and close the object file */
    fflush(obFile);
    fclose(obFile);
}

void printExternArrayToFile(externNode *node, FILE *file) {
    externNode *pNode;
 
    pNode = node;
    while (pNode != NULL) {
        char address[SIZE_OF_ADDRESS + 1];
 
        /* Convert the address to a character array in the specified base */
        intToCharsInNBase(pNode->address, address, SIZE_OF_ADDRESS, ADDRRESS_BASE);
 
        /* Write the external symbol name and its address to the file */
        fprintf(file, "%s %s\n", pNode->name, address);
        
        pNode = pNode->next;
    }
}




