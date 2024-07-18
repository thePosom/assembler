#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/firstPassScript.h"

bool endsWithColon(char *str);

void firstPass(char *filesName) {

    char amFileName[MAX_LINE_SIZE];
    char obFileName[MAX_LINE_SIZE];
    FILE *amFile;
    FILE *obFile;

    machineCodeLine *machinecodeline;
    bool isSymbolDef;
    char buffer[MAX_LINE_SIZE];
    char **currentInstruction;
    char *temp;
    int *dataNums;
    int wordNumModifier;
    int IC, DC;
    int size;
    
    IC = DC = 0;
    isSymbolDef = false;
    dataNums = (int *)malloc(sizeof(int));
    currentInstruction = (char **)malloc(MAX_WORDS_IN_LINE * sizeof(char));

    strcpy(amFileName, filesName);
    strcat(amFileName, ".am");

    strcpy(obFileName, filesName);
    strcat(obFileName, ".ob");

    amFile = fopen(amFileName, "r");
    obFile = fopen(amFileName, "w");

    if (amFile == NULL) {
        perror("Error opening file"); /*ERROR, DO SOMETHING COOL*/
        exit(1);
    }

    initializeDataArray();
    initializeInstructionsArray();
    initializeSymbolTable();

    while (fgets (buffer, sizeof(buffer), amFile) != NULL ) {

        line *currentLine = (line *)malloc(sizeof(line));
        lineToArrayOfWords(buffer, currentLine);

        if (currentLine->wordsCount != 0) {
            if (endsWithColon(currentLine->wordsArray[0])) {
                isSymbolDef = true;
                if (currentLine->wordsCount >= 1) {
                    /*ERORRERE*/
                }
                *currentInstruction = currentLine->wordsArray[1];
                wordNumModifier = -1;
            }

            else {
                isSymbolDef = false;
                wordNumModifier = 0;
                *currentInstruction = currentLine->wordsArray[0]; 
            }

            if (strcmp(*currentInstruction, ".extern") == 0) {
                if (currentLine->wordsCount + wordNumModifier != 2 ) {
                    /*ERRORINO*/
                }
                insertToSymbolTable(*(currentInstruction + 1), ".external", 0);
            }

            else if (isSymbolDef) {
                if ( (!isNameLegal (currentLine->wordsArray[0]) ) || (!isMacroName (currentLine->wordsArray[0]) ) || (getSymbolValue (currentLine->wordsArray[0]) == -1) ) { /*needs to check for extern data string*/
                    /*ERRRORRR*/
                }
                if (currentLine->wordsCount >= 2) {
                    /*ERRPR*/
                }

                if ( (strcmp(*currentInstruction, ".data") == 0) || (strcmp(*currentInstruction, ".string") == 0) ) {


                    if (strcmp(*currentInstruction, ".data") == 0) {
                        size = parseDataArray(currentLine, dataNums);
                        insertArrayToDataArray(dataNums, size);
                    }
                    else {
                        temp = removeQuotions(*(currentInstruction + 1));
                        size = strlen(temp) + 1;
                        insertStringToDataArray(temp);
                        free(temp);
                    }
                    insertToSymbolTable(currentLine->wordsArray[0], ".data", DC);
                    DC += size;
                }
            
                else {
                    temp = my_strdup(currentLine->wordsArray[0]);
                    *(temp + strlen(temp) - 1) = '\0';
                    insertToSymbolTable(temp, ".code", IC+100);
                    free(temp);
                }
            }

            if (isInstruction(*currentInstruction)) {
                /*IDK add to IC*/
                lineToMachineCode(currentInstruction, currentLine->wordsCount+wordNumModifier);
                initializeMachineCodeLine(machinecodeline);
                instructionToOpcode(*currentInstruction, (*machinecodeline).opcode);
            }

            else if ( (strcmp(*currentInstruction, ".data") == 0) && (strcmp(*currentInstruction, ".string") == 0) && 
            (strcmp(*currentInstruction, ".entry") == 0) && (strcmp(*currentInstruction, ".extern") == 0) ) {
                /*ERRORRERE*/
            }
            
        }
    }
    free(dataNums);
    free(currentInstruction);
    IC += 100;

}

bool endsWithColon(char *str) {
    return *(str + strlen(str) - 1) == ':';
}

int parseDataArray(line *currentLine, int *dataNums) {
    int i;
    int num;
    int size;
    char *token;
    char *str;
    char *delimiter = ",";
    char dataString[MAX_LINE_SIZE];
    
    dataString[0] = '\0';
    dataNums = (int *)malloc(MAX_WORDS_IN_LINE * sizeof(int));


    str = currentLine->wordsArray[(currentLine->wordsCount) - 1 ];
    if ( *(str + strlen(str) - 1) == ',' ) {
        /*EROORRR*/
    }

    for (i=2; i<currentLine->wordsCount; i++) {
        str = currentLine->wordsArray[i];
        if ( (i<currentLine->wordsCount - 1) && (*(str+strlen(str)-1) != ',') && ( *(currentLine->wordsArray[i+1]) != ',' ) );
        strcat(dataString, str);
    }

    size = 0;
    if (strstr(dataString, ",,") != NULL) {
        /*ERROR*/
    }

    token = strtok(dataString, delimiter); /*check if it changes og*/

    while (token != NULL && size < MAX_WORDS_IN_LINE) {
        num = atoi(token);
        if (num == 0) {
            if ( (strcmp(token, "0" ) != 0) && (strcmp(token, "+0" ) != 0) && (strcmp(token, "-0" ) != 0) ) {
                /*ERRORR*/
            }
        }
        if (num == INT_MAX || num == INT_MIN) {
            /*ERRORR*/
        }
        /*check if num too big or small*/

        dataNums[size] = num;
        size++;
        token = strtok(NULL, delimiter);
    }

    dataNums = (int *)realloc(dataNums, size * sizeof(int));

    return size;
}

int parseCommas(char **wordsArray, int wordsCount, char **output) {
    int i;
    int num;
    int size;
    char *token;
    char *str;
    char *delimiter = ",";
    
    output = (char **)malloc(MAX_LINE_SIZE * sizeof(char));
    *output = '\0';



    str = wordsArray[wordsCount - 1 ];
    if ( *(str + strlen(str) - 1) == ',' ) {
        /*EROORRR*/
    }

    for (i=2; i<wordsCount; i++) {
        str = wordsArray[i];
        if ( (i<wordsCount - 1) && (*(str+strlen(str)-1) != ',') && ( *(wordsArray[i+1]) != ',' ) );
        strcat(dataString, str);
    }

    size = 0;
    if (strstr(dataString, ",,") != NULL) {
        /*ERROR*/
    }

    token = strtok(dataString, delimiter); /*check if it changes og*/

    while (token != NULL && size < MAX_WORDS_IN_LINE) {
        num = atoi(token);
        if (num == 0) {
            if ( (strcmp(token, "0" ) != 0) && (strcmp(token, "+0" ) != 0) && (strcmp(token, "-0" ) != 0) ) {
                /*ERRORR*/
            }
        }
        if (num == INT_MAX || num == INT_MIN) {
            /*ERRORR*/
        }
        /*check if num too big or small*/

        dataNums[size] = num;
        size++;
        token = strtok(NULL, delimiter);
    }

    dataNums = (int *)realloc(dataNums, size * sizeof(int));

    return size;
}

char* removeQuotions(char *str) {
    char *copy;
    copy  = my_strdup(str+1);
    *(copy + ( strlen(copy) - 1 ) ) = '\0';
    return copy;
}

void lineToMachineCode(char **currentInstruction, int wordCount) {
    
} 
