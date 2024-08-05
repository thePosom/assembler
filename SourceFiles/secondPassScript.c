#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/secondPassScript.h"

void secondPass(char *filesName) {

    int IC;
    int DC;
    int startingWord;

    bool entryExists;

    externNode *externArray;

    FILE *amFile;

    char buffer[MAX_LINE_SIZE];
    
    amFile = openFile(filesName, ".am", "r");

    IC = 0;
    DC = 0;

    entryExists = false;

    externArray = NULL;

    if (amFile == NULL) {
        perror("Error opening file"); /*ERROR, DO SOMETHING COOL*/
        exit(1);
    }


    while (fgets (buffer, sizeof(buffer), amFile) != NULL ) {

        line *currentLine = (line *)malloc(sizeof(line));
        if (currentLine == NULL) {
            perror("Error, not enough memory");
            exit(EXIT_FAILURE);
        }

        lineToArrayOfWords(buffer, currentLine);

        if (currentLine->wordsCount != 0 && currentLine->wordsArray[0][0] != ';') {
                
            if (endsWithColon(currentLine->wordsArray[0]))
                startingWord = 1;
            else 
                startingWord = 0;


            if (strcmp(currentLine->wordsArray[startingWord], ".entry") == 0) {
                int value;

                if (currentLine->wordsCount - startingWord != 2 ) {
                    /*ERRORINO*/
                }
                /*maybe put values in prob*/
                entryExists = true;
                value = getSymbolValue(currentLine->wordsArray[startingWord + 1]);
                if (value == ERROR) {
                    /*Erero*/
                }
                insertToSymbolTable(currentLine->wordsArray[startingWord + 1], ".entry", value);
            }
            
            else if ( (strcmp(currentLine->wordsArray[startingWord], ".data") != 0) && (strcmp(currentLine->wordsArray[startingWord], ".string") != 0) && strcmp(currentLine->wordsArray[startingWord], ".extern") != 0 ) {
                int i;
                int size;
                int value;
                char **arguments;
                machineCodeLine *line;

                arguments = NULL;

                size = parseCommas(currentLine, startingWord, &arguments);

                if (bothRegisters(arguments, size))
                    IC += size;  /* than the var size plus 1 won't reflect the actual amount of words, also in this case we are sure the words are complete  */
                else {
                    for (i = 1; i <= size; i++) {
                        line = getFromInstructionsArray(IC + i);
                        if (line->values[0] == 0) { /* meaning that ARE is empty so we didn't fill it yet because it used a label */
                            value = getSymbolValue(arguments[i - 1]);
                            if (value == ERROR) {
                                /*errp*/
                            }

                            insertValuesToMachineCodeLine(line, value);

                            if (strcmp(getSymbolType(arguments[i - 1]), ".external") == 0) {
                                externArray = insertToExternArray(externArray, arguments[i - 1], IC + i + 100);
                                setMachineCodeARE(line, 0, true);
                            }
                            else
                                setMachineCodeARE(line, 1, true);
                            
                        }
                    }
                    line = getFromInstructionsArray(IC + 3);

                    IC += size + 1;
                }
            }

            else if (strcmp(currentLine->wordsArray[startingWord], ".extern") != 0) { /*change ordering so it makes more sense*/
                int size;
                char *temp;
                int *dataNums;
                dataNums = NULL;

                if (strcmp(currentLine->wordsArray[startingWord], ".data") == 0) {
                    size = parseDataArray(currentLine, startingWord, &dataNums);
                    free(dataNums);
                }
                else {
                    temp = removeQuotions(currentLine->wordsArray[startingWord+1]);
                    size = strlen(temp) + 1;
                    free(temp);
                }

                DC += size;
            }
        }
    }

    /*prob free stuff*/
    buildFiles(filesName, entryExists, externArray, IC, DC);

}

bool bothRegisters(char **arguments, int size) {
    return size == 2 && isAnyRegister(arguments[0]) && isAnyRegister(arguments[1]);
}

bool isAnyRegister(char *str) {
    return isRegister(str) || ( (*str == '*') && (isRegister (str + 1 ) ) );
}

void buildFiles(char *filesName, bool entryExists, externNode *externArray, int ICTotal, int DCTotal) {

    int IC;
    int DC;
    int size;
    int value;
    char address[5];
    char octalLine[6];

    machineCodeLine *line;
    machineCodeLine *nextLine;
    
    FILE *obFile;
    FILE *entFile;
    FILE *extFile;

    obFile = openFile(filesName, ".ob", "w");
    if (entryExists) {
        entFile = openFile(filesName, ".ent", "w");
        printToFileByType(entFile, ".entry");
    }
    if (externArray != NULL) {
        extFile = openFile(filesName, ".ext", "w");
        printExternArrayToFile(externArray, extFile);
        freeExternArray(externArray);
    }


    fprintf(obFile, "%d %d\n", ICTotal, DCTotal);

    IC = 0;
    line = getFromInstructionsArray(IC);
    nextLine = getFromInstructionsArray(IC+1);
    while (nextLine != NULL) {
        intToCharsInNBase(IC + 100, address, 4, 10);
        intToCharsInNBase(machineCodeLineToInt(line), octalLine, 5, 8);

        fprintf(obFile, "%s %s\n", address, octalLine);

        IC++;
        line = getFromInstructionsArray(IC);
        nextLine = getFromInstructionsArray(IC+1);
    }

    size = getDataArraySize();
    for (DC = 0; DC < size; DC++) {
        value = getFromDataArray(DC);
        intToCharsInNBase(DC + IC + 100, address, 4, 10);
        intToCharsInNBase(value, octalLine, 5, 8);

        fprintf(obFile, "%s %s\n", address, octalLine);
    }
    
}

void intToCharsInNBase(int num, char arr[], int size, int base) {
    int i;
    unsigned int u_num;

    u_num = (unsigned int)num;

    for (i = 0; i < size; i++) {
        arr[size - i - 1] = (u_num % base) + '0';
        u_num /= base;
    }
    arr[size] = '\0';
}

void printExternArrayToFile(externNode *node, FILE *file) {
    
    while (node != NULL) {
        char address[5];
        intToCharsInNBase(node->address, address, 4, 10);
        fprintf(file, "%s %s\n", node->name, address);
        
        node = node->next;
    }
}




