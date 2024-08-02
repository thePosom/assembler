#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/firstPassScript.h"

bool endsWithColon(char *str);

void firstPass(char *filesName) {

    char amFileName[MAX_LINE_SIZE];
    FILE *amFile;

    machineCodeLine *machinecodeline[3];
    bool isSymbolDef;
    char buffer[MAX_LINE_SIZE];
    int wordNumModifier;
    int IC, DC;
    int size;
    
    IC = DC = 0;
    isSymbolDef = false;

    strcpy(amFileName, filesName);
    strcat(amFileName, ".am");
    amFile = fopen(amFileName, "r");

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

        if (currentLine->wordsCount != 0 && currentLine->wordsArray[0][0] != ';') {

            if (endsWithColon(currentLine->wordsArray[0])) {
                isSymbolDef = true;
                if (currentLine->wordsCount >= 1) {
                    /*ERORRERE*/
                }
                wordNumModifier = -1;
            }
            else {
                isSymbolDef = false;
                wordNumModifier = 0;
            }

            if (strcmp(currentLine->wordsArray[-wordNumModifier], ".extern") == 0) {
                if (currentLine->wordsCount + wordNumModifier > 2 ) {
                    /*ERRORINO*/
                }
                /* support multiple stuff parse commas*/
                insertToSymbolTable(currentLine->wordsArray[-wordNumModifier + 1], ".external", 0);
            }
            else if (isSymbolDef) {
                DC = newSymbol (currentLine, -wordNumModifier, DC, IC);
            }

            if (isInstruction(currentLine->wordsArray[-wordNumModifier])) {
                size = lineToMachineCode(currentLine, -wordNumModifier, machinecodeline);
                insertToInstructionsArray(machinecodeline, size);
                printf("\n");
                IC+=size;
            }

            else if ( (strcmp(currentLine->wordsArray[-wordNumModifier], ".data") != 0) && (strcmp(currentLine->wordsArray[-wordNumModifier], ".string") != 0) && 
            (strcmp(currentLine->wordsArray[-wordNumModifier], ".entry") != 0) && (strcmp(currentLine->wordsArray[-wordNumModifier], ".extern") != 0) ) {
                /*ERRORRERE*/
            }
            
        }
    }
    addToAllDataInSymbolTable(IC+100);

}

int newSymbol(line *currentLine, int startingWord, int DC, int IC) {
    int size;
    char *temp;
    int *dataNums;
    dataNums = NULL;

    if ( (!isNameLegal (currentLine->wordsArray[0]) ) || (!isMacroName (currentLine->wordsArray[0]) ) || (getSymbolValue (currentLine->wordsArray[0]) == -1) ) { /*needs to check for extern data string*/
        /*ERRRORRR*/
    }

    if ( (strcmp(currentLine->wordsArray[startingWord], ".data") == 0) || (strcmp(currentLine->wordsArray[startingWord], ".string") == 0) ) {


        if (strcmp(currentLine->wordsArray[startingWord], ".data") == 0) {
            size = parseDataArray(currentLine, &dataNums);
            insertArrayToDataArray(dataNums, size);
            insertArrayToInstructionsArray(dataNums, size);
            free(dataNums);
        }
        else {
            temp = removeQuotions(currentLine->wordsArray[startingWord+1]);
            size = strlen(temp) + 1;
            insertStringToInstructionsArray(temp);
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

    return DC;
}

bool endsWithColon(char *str) {
    return *(str + strlen(str) - 1) == ':';
}

int parseDataArray(line *currentLine, int **dataNums) {
    int i;
    int num;
    int size;
    char *str;
    char **dataString;

    dataString = NULL;
    
    size = parseCommas(currentLine, 1, &dataString);
    *dataNums = (int *)malloc(size * sizeof(int));

    for (i = 0; i < size; i++) {
        str = *(dataString+i);
        num = stringToNum(str); /*check too big too small*/
        if (num == INT_MIN) {
            /*ERRORR*/
        }

        (*dataNums)[i] = num;
    }

    if (size == 0) {
        /* error */
    }

    return size;
}

int parseCommas(line *currentLine, int startingWord, char ***output) {
    int i;
    int size;
    char *token;
    char *str;
    char *delimiter = ",";
    char temp[MAX_LINE_SIZE];
    
    *output = (char **)malloc(currentLine->wordsCount * sizeof(char*));
    temp[0] = '\0';



    str = currentLine->wordsArray[currentLine->wordsCount-1];
    if ( *(str + strlen(str) - 1) == ',' ) {
        /*EROORRR*/
    }

    for (i = startingWord + 1; i<currentLine->wordsCount; i++) {
        str = currentLine->wordsArray[i];
        if ( (i<currentLine->wordsCount - 1) && (*(str+strlen(str)-1) != ',') && ( *(currentLine->wordsArray[i+1]) != ',' ) ) {
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
        *(*(output)+size) = my_strdup(token);
        size++;
        token = strtok(NULL, delimiter);
    }

    *output = (char **)realloc(*output, size * sizeof(char*));

    return size;
}

char* removeQuotions(char *str) {
    char *copy;
    copy  = my_strdup(str+1);
    *(copy + ( strlen(copy) - 1 ) ) = '\0';
    return copy;
}

int lineToMachineCode(line *currentLine, int startingWord, machineCodeLine *lines[3]) {
    int i;
    int size;
    int sizeModifier;
    char **arguments;
    bool firstIsRegister;

    arguments = NULL;

    size = parseCommas(currentLine, startingWord, &arguments);

    initializeMachineCodeLines(lines, 3);
    insertOpcodeToMachineCodeLine(lines[0], instructionToOpcode(currentLine->wordsArray[startingWord])); /*nope*/
    setMachineCodeARE(lines[0], 2, true); /*nope*/

    if (size != getArgumentsCount(currentLine->wordsArray[startingWord]) ) {
        /* Error */
    }

    firstIsRegister = false;
    sizeModifier = 0;

    for (i = 0; i < size; i++) {
        int num;
        int addressingMethod;


        if (**(arguments+i) == '#') {
            addressingMethod = 0;
            num = stringToNum(*(arguments+i));
            if (num == INT_MIN) { /*check if too small or big somewhere*/
                /*AEROE*/
            }
            /* numTo */

            setMachineCodeARE(lines[i+1], 2, true);
            insertValuesToMachineCodeLine(lines[i+1], num); /*make work good with neg*/
        }

        else if ( (**(arguments+i) == '*') && (isRegister (*(arguments+i) + 1 ) ) ) {
            addressingMethod = 2;
            num = getRegisterNum(*(arguments+i) + 1);

            sizeModifier = ifRegister (lines, num, i, &firstIsRegister, &size);
        }

        else if (isRegister (*(arguments+i) ) ) {
            addressingMethod = 3;
            num = getRegisterNum(*(arguments+i));

            sizeModifier = ifRegister (lines, num, i, &firstIsRegister, &size);
        }

        else 
            addressingMethod = 1;
        
        if (i + 1 == size) 
            setMachineCodeValues(lines[0], 0 + addressingMethod, true);
        else
            setMachineCodeValues(lines[0], 4 + addressingMethod, true);
    }



    return size + sizeModifier + 1;
}

int ifRegister (machineCodeLine *lines[3], int num, int i, bool *firstIsRegister, int *size) {
    int sizeModifier;
    sizeModifier = 0;
    if (*firstIsRegister) {
        sizeModifier = -1;
    }
        
    if (i + 1 == *size)
        setMachineCode3To5Values(lines[i + sizeModifier + 1], num);
    else 
        setMachineCode6To8Values(lines[i + sizeModifier + 1], num);

    setMachineCodeARE(lines[i+1], 2, true);
    
    *firstIsRegister = true;

    return sizeModifier;
}



