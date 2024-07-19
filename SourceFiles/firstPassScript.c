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

    machineCodeLine machinecodeline[3];
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
                size = lineToMachineCode(currentInstruction, currentLine->wordsCount+wordNumModifier, machinecodeline);
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
    char *str;
    char **dataString;

    dataString = NULL;
    
    size = parseCommas(currentLine->wordsArray, currentLine->wordsCount, dataString);
    dataNums = (int *)malloc(size * sizeof(int));

    for (i = 0; i < size; i++) {
        str = *(dataString+i);
        num = stringToNum(str);
        if (num == INT_MIN) {
            /*ERRORR*/
        }

        dataNums[size] = num;
    }

    return size;
}

int parseCommas(char **wordsArray, int wordsCount, char **output) {
    int i;
    int size;
    char *token;
    char *str;
    char *delimiter = ",";
    char temp[MAX_LINE_SIZE];
    
    output = (char **)malloc(wordsCount * sizeof(char*));
    temp[0] = '\0';



    str = wordsArray[wordsCount - 1 ];
    if ( *(str + strlen(str) - 1) == ',' ) {
        /*EROORRR*/
    }

    for (i=2; i<wordsCount; i++) {
        str = wordsArray[i];
        if ( (i<wordsCount - 1) && (*(str+strlen(str)-1) != ',') && ( *(wordsArray[i+1]) != ',' ) ) {
            /*error*/
        }
        strcat(temp, str);
    }

    size = 0;
    if (strstr(temp, ",,") != NULL) {
        /*ERROR*/
    }

    token = strtok(temp, delimiter); /*check if it changes og*/

    while (token != NULL) {
        
        output[size] = my_strdup(token);
        size++;
        token = strtok(NULL, delimiter);
    }

    output = (char **)realloc(output, size * sizeof(int*));

    return size;
}

char* removeQuotions(char *str) {
    char *copy;
    copy  = my_strdup(str+1);
    *(copy + ( strlen(copy) - 1 ) ) = '\0';
    return copy;
}

int lineToMachineCode(char **currentInstruction, int wordCount, machineCodeLine lines[3]) {
    int i;
    int size;
    char **arguments;
    bool firstIsRegister;

    arguments = NULL;

    wordCount = parseCommas(currentInstruction + 1, wordCount, arguments);
    size = wordCount;

    initializeMachineCodeLines(lines);
    instructionToOpcode(*currentInstruction, lines[0].values + 8);
    lines[0].are[2] = true;

    if (wordCount != getArgumentsCount(*currentInstruction) ) {
        /* Error */
    }

    firstIsRegister = false;

    for (i = 0; i < wordCount; i++) {
        int num;
        int modifier;
        int addressingMethod;

        if (**(arguments+i) == '#') {
            addressingMethod = 0;
            num = stringToNum(*(arguments+i));
            if (num == INT_MIN) {
                /*AEROE*/
            }

            lines[i+1].are[2] = true;
            unsignedNumToBoolArray(num, lines[i+1].values, 12);
        }

        else if (isRegister (*(arguments+i) ) ) {
            addressingMethod = 2;
            num = getRegisterNum(*(arguments+i));

            if (firstIsRegister) {
                modifier = 0;
                size--;
            }
            else
                modifier = 1;

            lines[i+1].are[2] = true;
            unsignedNumToBoolArray(num, lines[i+modifier].values + (i*3), 3);
            
            firstIsRegister = true;
        }

        else if ( (**(arguments+i) == '*') && (isRegister (*(arguments+i) + 1 ) ) ) {
            addressingMethod = 3;
            num = getRegisterNum(*(arguments+i) + 1);

            if (firstIsRegister) 
                modifier = 0;
            else
                modifier = 1;

            lines[i+1].are[2] = true;
            unsignedNumToBoolArray(num, lines[i+modifier].values + (i*3), 3);
            
            firstIsRegister = true;
        }

        else 
            addressingMethod = 1;
        
        
        lines[0].values[i*3 + addressingMethod] = true;
    }



    return size;
} 
